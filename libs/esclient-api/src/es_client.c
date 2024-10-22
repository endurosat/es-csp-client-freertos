/******************************** INCLUDE FILES *******************************/
#include <string.h>
#include "esclient_classes.h"

/*** Transport layer ***/
#include "es_client_tl_iface.h"
/* ES CSP */
#include "es_client_csp.h"

#include "es_tftp_pkt.h"
#include "es_port.h"

#include "es_plf.h"
#include "es_log.h"
/******************************** LOCAL DEFINES *******************************/
#define ES_MODULE_NAME  "es_client"

/*  Structure of our class */
typedef struct _es_client_t {
    /*** Transport layer ***/
    bool_t transport_layer_init_done;

    /* Indicates which transport layer is used by the instance */
    es_client_transport_t transport_layer_type;
    /* Transport Layer abstraction */
    es_client_tl_t *transport_layer;
} es_client_t;


/************************* LOCAL FUNCTIONS DEFINITIONS ************************/

/********************************* LOCAL DATA *********************************/
static es_client_t es_client;

static const char *transport_layers[ES_CLIENT_TL_N] = {
    "CSP",
};

/******************************* INTERFACE DATA *******************************/

/******************************* LOCAL FUNCTIONS ******************************/
static bool_t
es_client_transport_init_done(void)
{
    if (es_client.transport_layer_init_done)
        return TRUE;

    return FALSE;
}

/**************************** INTERFACE FUNCTIONS *****************************/
int8_t es_client_init(void)
{
    ES_LOG_MSG(ES_TRACE, "Initializing ES Client");

    //  Initialize class properties here
    es_client.transport_layer_init_done = FALSE;
    es_client.transport_layer = NULL;

    return 0;
}

/**
 * Deinit es_client object.
 */
void
es_client_deinit (void)
{
    /* If the transport layer is initialized free the object/deinit */
    if (es_client.transport_layer != NULL)
        es_client.transport_layer->deinit();
}

int8_t
es_client_csp_tl_init(uint16_t dest_addr)
{
    int8_t ret = -1;

    if (es_client_transport_init_done())
    {
        ES_LOG_MSG(ES_WARNING, "Transport layer already initialized!");
        ES_LOG_MSG(ES_WARNING, "Current transport layer: %s",
                             transport_layers[es_client.transport_layer_type]);
        return ret;
    }
    es_csp_conf_t csp_conf;
    csp_conf.addr = dest_addr;

    /* Inidicate that the transport layer that will be used is CSP */
    es_client.transport_layer_type = ES_CLIENT_TL_CSP;
    es_client.transport_layer = es_client_csp_get();
    ret = es_client.transport_layer->init(&csp_conf);
    if (0 != ret)
    {
        ES_LOG_MSG(ES_ERROR, "Failed to initialize CSP transport layer");
        es_client.transport_layer_init_done = FALSE;
        es_client.transport_layer = NULL;
        return -1;
    }
    es_client.transport_layer_init_done = TRUE;

    return 0;
}

int8_t
es_client_file_download(const char *src_file, const char *dest_file)
{
    int ret = -1;
    uint8_t buff[ES_TFTP_PKT_MAX_SIZE];
    int32_t buff_size = 0;
    es_tftp_pkt_t *i_pkt = NULL;
    es_port_file_t fp;
    es_client_conn_t conn = es_client_conn_init_default;

    if (!es_client_transport_init_done()) {
        ES_LOG_MSG(ES_ERROR, "Transport layer not initialized!");
        return -1;
    }

    /**
     *  The file is always truncated upon open
     */
    es_port_file_open(&fp, dest_file,
        (ES_PORT_FILE_W_MODE | ES_PORT_FILE_CREATE_ALWAYS));

    conn.dest_port = es_client.transport_layer->get_port(ES_CLIENT_TFPT);
    ret = es_client.transport_layer->connect(&conn);
    if (0 != ret)
    {
        ES_LOG_MSG(ES_ERROR, "Connection to server failed");
        es_port_file_close(&fp);
        return -1;
    }

    /* Send Read request */
    buff_size = es_tftp_opkt_request(buff, ES_TFTP_RRQ, src_file);
    es_client.transport_layer->send(&conn, buff, buff_size);

    uint16_t block_size = 0;
    /* Start receiving:
     *       <--- DATA
     * ACK --->
     *       <--- DATA
     * ACK --->
     *    ...
     *    ...
     *       <--- DATA
     * ACK --->
     * Stop condition: bytes_received [0; 511] or intermediary step failiure
     */
    for (;;)
    {
        ret = es_client.transport_layer->recv(&conn, buff, &buff_size);
        if (0 != ret)
        {
            ES_LOG_MSG(ES_ERROR, "Read failed!");
            break;
        }

        i_pkt = es_tftp_ipkt_get(buff, buff_size);
        if (i_pkt->opcode == ES_TFTP_ERROR)
        {
            ES_LOG_MSG(ES_ERROR, "Error: %s (%d)", i_pkt->err.msg, i_pkt->err.code);
            break;
        }

        /* We expect this to be a data packet */
        if (i_pkt->opcode != ES_TFTP_DATA)
        {
            buff_size = es_tftp_opkt_err(buff, ES_TFTP_ERR_ILLEGAL_TFTP_OPERATION,
                "Expected data block!");
             es_client.transport_layer->send(&conn, buff, buff_size);
             break;
        }

        /* Get data packet's block size */
        block_size = es_tftp_ipkt_data_size(i_pkt, buff_size);
        /* Write data on the disk */
        es_port_file_write(&fp, i_pkt->data.block, block_size);

        /* Send ack */
        buff_size = es_tftp_opkt_ack(buff, i_pkt->data.block_number);
        es_client.transport_layer->send(&conn, buff, buff_size);
        if (block_size < ES_TFTP_BLOCK_SIZE) {
            ES_LOG_MSG(ES_INFO, "Received file %s", dest_file);
            /* Sucess */
            ret = 0;
            break;
        }
    }
    es_port_file_close(&fp);
    es_client.transport_layer->disconnect(&conn);

    return ret;
}

int8_t
es_client_file_upload(const char *src_file, const char *dest_file)
{
    int ret = -1;
    ssize_t bytes_read = 0; /* Bytes read (for a single data transaction) from the file */
    uint16_t block_number = 0; /* Block ID */
    uint8_t buff[ES_TFTP_PKT_MAX_SIZE];
    int32_t buff_size = 0;
    es_tftp_pkt_t *i_pkt = NULL;
    es_client_conn_t conn;
    es_port_file_t fp; /* File handle */

    if (!es_client_transport_init_done()) {
        ES_LOG_MSG(ES_ERROR, "Transport layer not initialized!");
        return -1;
    }

    ret = es_port_file_open(&fp, src_file, ES_PORT_FILE_R_MODE);
    if (0 != ret)
    {
        ES_LOG_MSG(ES_ERROR, "Failed to open file");
        return -1;
    }

    conn.dest_port = es_client.transport_layer->get_port(ES_CLIENT_TFPT);
    ret = es_client.transport_layer->connect(&conn);
    if (0 != ret)
    {
        ES_LOG_MSG(ES_ERROR, "Connection to server failed");
        es_port_file_close(&fp);
        return -1;
    }

    /* Send Read request */
    buff_size = es_tftp_opkt_request(buff, ES_TFTP_WRQ, dest_file);
    es_client.transport_layer->send(&conn, buff, buff_size);
    /* Start sending:
     *       <--- ACK
     * DATA --->
     *       <--- ACK
     *    ...
     *    ...
     * DATA --->
     *       <--- ACK
     * Stop condition: bytes_read [0; 511] or intermediary step failiure
     */
    uint64_t bytes_total = 0;
    bool_t transfer_finished = FALSE;
    for(;;)
    {
        /**
         * 1. Create Data packet
         * 2. Send Data packet
         * 3. Receive Packet
         * 4. Check for acknowedged data block
         */

        /* 1. Create Data packet */
        ret = es_client.transport_layer->recv(&conn, buff, &buff_size);
        if (0 != ret)
        {
            ES_LOG_MSG(ES_ERROR, "Read failed!");
            break;
        }

        i_pkt = es_tftp_ipkt_get(buff, buff_size);
        if (i_pkt == NULL)
        {
            /* Mallformed packet */
            const uint8_t *err_msg = (const uint8_t *)"Mallformed TFTP packet!";
            ES_LOG_MSG(ES_ERROR, "%s", err_msg);
            buff_size = es_tftp_opkt_err(buff, ES_TFTP_ERR_ILLEGAL_TFTP_OPERATION,
            err_msg);
            break;
        }

        /* 4. Check for acknowedged data block */
        /* The client is telling us that an error has occured on his side */
        if (i_pkt->opcode == ES_TFTP_ERROR)
        {
            /* At this point the client has disconnected, so we close the transfer here
             * as well.
             */
            ES_LOG_MSG(ES_ERROR, "Error %s (%d)", i_pkt->err.msg, i_pkt->err.code);
            break;
        }

        /**
         * Waiting for ACK of the DATA block that we have sent
         * Any other opcode is threated as an error.
         */
        if (i_pkt->opcode != ES_TFTP_ACK)
        {
            const uint8_t *err_msg = (const uint8_t *)"Illegal TFTP operation!";
            ES_LOG_MSG(ES_ERROR, "%s", err_msg);
            buff_size = es_tftp_opkt_err(buff, ES_TFTP_ERR_ILLEGAL_TFTP_OPERATION,
            err_msg);
            break;
        }

        /* ACK received. Check block n */
        if (i_pkt->ack.block_number != block_number)
        {
            /* TODO: Add err  */
            ES_LOG_MSG(ES_ERROR, "Block number missmatch! i:%d Expected: %d",
                i_pkt->ack.block_number, block_number);
            break;
        }

        if (transfer_finished) {
            ES_LOG_MSG(ES_INFO, "Sent file %s", src_file);
            ret = 0;
            break;
        }

        block_number = (block_number + 1) % ES_TFTP_BLOCK_N_MAX_VALUE;
        buff_size = es_tftp_opkt_data (buff, block_number, es_port_file_read, &fp,
            &bytes_read);
        es_client.transport_layer->send(&conn, buff, buff_size);
        if (bytes_read < ES_TFTP_BLOCK_SIZE)
            transfer_finished = TRUE;
    }

    es_port_file_close(&fp);
    es_client.transport_layer->disconnect(&conn);

    return ret;
}
