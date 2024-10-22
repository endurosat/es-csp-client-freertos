/*****************************************************************************
 * **File:** es_client.h
 *
 * **Description:** ES Client API
 *
 * **Example Usage**
 *
 * .. code-block:: C
 *		:caption: Initialize & use the ES Client module over CSP
 *		:linenos:
 *		:lineno-start: 1
 *		:name: <es_client>
 *
 *		es_client_init();
 *		es_client_csp_tl_init(10);
 *
 ****************************************************************************/
#ifndef __ES_CLIENT_H_INCLUDED__
#define __ES_CLIENT_H_INCLUDED__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************** DEFINES **********************************/

typedef struct _es_client_conn_t {
       void *descriptor;  /**< Connection descriptor */
       uint16_t dest_port;  /**< Destination port */
} es_client_conn_t;
#define es_client_conn_init_default {NULL, 0}

/*********************************** TYPEDEFS *********************************/

/******************************* INTERFACE FUNCTIONS **************************/

/**
 * Initializes es client lib.
 *
 * @return On success 0, -1 in case of failiure
 */
int8_t
es_client_init (void);

/**
 * Deinit es_client object.
 */
void
es_client_deinit (void);

/**
 * Initialize CSP as a transport layer for es_client
 *
 * @param[in] dest_addr: Destination address of the module.
 * @return 0 if successful, -1 if initialization failed.
 */
int8_t
es_client_csp_tl_init(uint16_t dest_addr);


/**
 * Upload a file.
 *
 * @param[in] src_file: Name of the file to be uploaded.
 * @param[in] dest_file: Remote location (path) on the file system where to store the file.
 * @return 0 if successful, -1 if failed.
 */
int8_t
es_client_file_upload(const char *src_file, const char *dest_file);


/**
 * Download a file.
 *
 * @param[in] src_file: Path of the file to be downloaded.
 * @param[in] dest_file Location on the host file system where
 *  to store the file.
 * @return 0 if successful, -1 if failed.
 */
int8_t
es_client_file_download(const char *src_file, const char *dest_file);

#ifdef __cplusplus
}
#endif

#endif
