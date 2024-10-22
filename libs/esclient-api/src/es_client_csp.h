#ifndef __ES_CLIENT_CSP__
#define __ES_CLIENT_CSP__

/******************************** INCLUDE FILES *******************************/
#include "es_client_tl_iface.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*********************************** DEFINES **********************************/
#define ES_CSP_DLFT_ADDRESS  (10)

/*********************************** TYPEDEFS *********************************/
typedef struct _es_csp_conf_t {
    int addr;
} es_csp_conf_t;
#define es_csp_conf_init_default {ES_CSP_DLFT_ADDRESS}

/***************************** INTERFACE DATA TYPES ***************************/

/***************************** INTERFACE FUNCTIONS ****************************/
es_client_tl_t *es_client_csp_get(void);

#ifdef __cplusplus
}
#endif /* _cplusplus */

#endif /* __ES_CLIENT_CSP__ */
