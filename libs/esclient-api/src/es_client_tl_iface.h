#ifndef __ES_CLIENT_TRANSPORT_LAYER__
#define __ES_CLIENT_TRANSPORT_LAYER__

/******************************** INCLUDE FILES *******************************/
#include <stdint.h>
#include "esclient_classes.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/********************************  DEFINES ************************************/
/* Transport layer MTU ( Set to maximum supported value of the tranport layers )*/
#define CLIENT_IO_BUFFER_SIZE                           600

/******************************* LOCAL TYPEDEFS *******************************/

/***************************** INTERFACE DATA TYPES ***************************/
/* Transport layer types */
typedef enum _es_client_transport_t {
    ES_CLIENT_TL_CSP = 0,
    ES_CLIENT_TL_N,
} es_client_transport_t;

/* Different application are hosted on different ports */
typedef enum _es_client_app_t {
    ES_CLIENT_RPC = 0,
    ES_CLIENT_TFPT,
    ES_CLIENT_APP_N,
    ES_CLIENT_INVALID,
} es_client_app_t;

typedef int8_t (*es_client_tl_init_t) (void *);
typedef void (*es_client_tl_deinit_t) (void);
typedef uint16_t (*es_client_tl_get_port_t) (es_client_app_t);
typedef int8_t (*es_client_tl_send_t) (void *, uint8_t*, int32_t);
typedef int8_t (*es_client_tl_recv_t) (void *, uint8_t*, int32_t*);
typedef int8_t (*es_client_connect_t) (void *);
typedef int8_t (*es_client_disconnect_t) (void *);

typedef struct _es_client_tl_t {
    /* Init / Deinit*/
    es_client_tl_init_t init;
    es_client_tl_deinit_t deinit;
    /* Get the application port */
    es_client_tl_get_port_t get_port;
    /* Connect to server endpoint */
    es_client_connect_t connect;
    /* Close connection */
    es_client_disconnect_t disconnect;
    /* I/O handlers */
    es_client_tl_send_t send;
    es_client_tl_recv_t recv;
} es_client_tl_t;

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* __ES_CLIENT_TRANSPORT_LAYER__ */
