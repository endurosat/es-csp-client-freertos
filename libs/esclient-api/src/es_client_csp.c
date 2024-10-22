/******************************** INCLUDE FILES *******************************/

#include <csp/csp.h>
#include <csp/arch/csp_malloc.h>
#include <csp/interfaces/csp_if_kiss.h>
#include <csp/interfaces/csp_if_can.h>

#include "esclient_classes.h"

#include "es_client_csp.h"
#include "es_client_tl_iface.h"

/* Internal gp utils & classes */
#include "es_plf.h"
#include "es_port.h"

#include "es_log.h"
/*********************************** DEFINES **********************************/
#define ES_MODULE_NAME     "es_csp"

/******* CSP Client connection properties *******/
#define ES_CSP_RECV_TIMEOUT             1000
#define ES_CSP_SEND_TIMEOUT             1000

#define ES_CSP_CONN_RETRIES       10
#define ES_CSP_CONN_RETRIE_WAIT   10
#define ES_CSP_CONN_OPTIONS       (CSP_O_NONE)

#define ES_CSP_MTU (CLIENT_IO_BUFFER_SIZE)

#define ES_CSP_RPC_PORT     10u
#define ES_CSP_TFTP_PORT    11u
/********************************* TYPEDEF ************************************/
typedef struct _es_csp_state_t {
    uint32_t dest_addr;
} es_csp_state_t;

/********************************* LOCAL DATA *********************************/

/***************************** FUNCTION PROTOTYPES ****************************/

/********************************* LOCAL DATA *********************************/
static uint16_t es_client_app_port [ES_CLIENT_APP_N] = {
    ES_CSP_RPC_PORT,
    ES_CSP_TFTP_PORT
};

/********************************* LOCAL STATE ********************************/
static es_csp_state_t es_csp_state;
static es_client_tl_t es_client_csp_tl;

/******************************* LOCAL FUNCTIONS *******************************/
/**
 * Initialize CSP Context
 *
 */
static int8_t es_client_csp_init(void  *args)
{
    es_csp_conf_t *csp_conf = (es_csp_conf_t *)args;
    es_csp_state.dest_addr = csp_conf->addr;

    return 0;
}

static void es_client_csp_deinit()
{
}

static int8_t es_client_csp_connect(void *conn_ctx)
{
    int8_t connection_retries = ES_CSP_CONN_RETRIES;
    int8_t connected = 0;
    es_client_conn_t *conn = (es_client_conn_t *)conn_ctx;
    do
    {
        conn->descriptor = csp_connect(0, es_csp_state.dest_addr,
            conn->dest_port, 15000, (ES_CSP_CONN_OPTIONS));
        if (conn->descriptor == NULL)
        {
            /* Connect failed */
            connected = 0;
            connection_retries--;
            ES_LOG_MSG(ES_WARNING, "Connection failed (%u)", connection_retries);
            es_port_delay(ES_CSP_CONN_RETRIE_WAIT);
        }
        else
        {
            connected = 1;
            ES_LOG_MSG(ES_TRACE, "Connection initialized");
        }
    } while (connection_retries && !connected);

    return connected ? 0 : -1;
}

static int8_t es_client_csp_disconnect(void *conn_ctx)
{
    int8_t ret = CSP_ERR_NONE;

    es_client_conn_t *conn = (es_client_conn_t *)conn_ctx;

    ret = csp_close(conn->descriptor);
    if (CSP_ERR_NONE != ret)
    {
        ES_LOG_MSG(ES_ERROR, "Failed to close connection");
        return -1;
    }
    conn->descriptor = NULL;

    return 0;
}

static uint16_t es_client_csp_get_port(es_client_app_t app_type)
{
    if (app_type < ES_CLIENT_RPC || app_type >= ES_CLIENT_APP_N) {
        ES_LOG_MSG(ES_ERROR, "Invalid application type: %d", app_type);
        return ES_CLIENT_INVALID;
    }

    return es_client_app_port[app_type];
}

static int8_t es_client_csp_send(void *conn_ctx, uint8_t *o_buffer,
    int32_t o_buffer_size)
{
    int8_t ret = CSP_ERR_NONE;

    es_client_conn_t *conn = (es_client_conn_t *)conn_ctx;

    csp_packet_t *packet = csp_buffer_get(ES_CSP_MTU);
    if (packet == NULL) {
        /* Could not get buffer element */
        ES_LOG_MSG(ES_ERROR, "Failed to get CSP buffer");
        return -1;
    }
    memcpy(packet->data, o_buffer, o_buffer_size);
    packet->length = o_buffer_size;

    if (1 != csp_send(conn->descriptor, packet, ES_CSP_SEND_TIMEOUT))
    {
        /* Send failed */
        ES_LOG_MSG(ES_ERROR, "Send failed");
        csp_buffer_free(packet);
    }

    return ret;
}

static int8_t es_client_csp_recv(void *conn_ctx, uint8_t *i_buffer,
    int32_t *i_buffer_size)
{
    int8_t ret = CSP_ERR_NONE;
    es_client_conn_t *conn = (es_client_conn_t *)conn_ctx;

    csp_packet_t *packet = NULL;
    packet = csp_read(conn->descriptor, ES_CSP_RECV_TIMEOUT);
    // check packet
    if (NULL != packet)
    {
        memcpy(i_buffer, packet->data, packet->length);
        *i_buffer_size = packet->length;
        csp_buffer_free(packet);
    }
    else
    {
    	ret = CSP_ERR_TIMEDOUT;
    }

    return ret;
}

/***************************** INTERFACE FUNCTIONS ****************************/
es_client_tl_t *es_client_csp_get(void)
{
    es_client_csp_tl.init = es_client_csp_init;
    es_client_csp_tl.deinit = es_client_csp_deinit;

    es_client_csp_tl.get_port = es_client_csp_get_port;

    es_client_csp_tl.connect = es_client_csp_connect;
    es_client_csp_tl.disconnect = es_client_csp_disconnect;

    es_client_csp_tl.send = es_client_csp_send;
    es_client_csp_tl.recv = es_client_csp_recv;

    return &es_client_csp_tl;
}
