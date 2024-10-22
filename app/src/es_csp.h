#ifndef __ES_CSP_H__
#define __ES_CSP_H__

/******************************** INCLUDE FILES *******************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************** DEFINES **********************************/

/*********************************** TYPEDEFS *********************************/
typedef enum _es_csp_phy_t {
    ES_CSP_UART = 0,
    ES_CSP_CAN,
    ES_CSP_PHY_LAYERS
} es_csp_phy_t;

typedef struct _es_csp_conf_t {
    es_csp_phy_t phy_type;
    uint32_t s_addr;
    void *drv_iface;
} es_csp_conf_t;

typedef int (*es_csp_uart_tx_hndlr_t)(void *, const uint8_t *, size_t);
typedef struct _es_csp_uart_drv_t {
    es_csp_uart_tx_hndlr_t tx;
} es_csp_uart_drv_t;
#define es_csp_uart_drv_init_default {NULL}

typedef int (*es_csp_can_tx_t)(void *, uint32_t, const uint8_t *, uint8_t);
typedef struct _es_csp_can_drv_t {
    es_csp_can_tx_t tx;
} es_csp_can_drv_t;
#define es_csp_can_drv_init_default {NULL}

/******************************* INTERFACE FUNCTIONS **************************/
int8_t es_csp_init(es_csp_conf_t *es_csp_conf);
int8_t es_csp_uart_frame_rx(const uint8_t *frame, uint32_t frame_len,
    void *px_task_woken);
int8_t es_csp_can_frame_rx(uint32_t id, const uint8_t *data, uint32_t dlc,
    void *px_task_woken);

#ifdef __cplusplus
}
#endif

#endif
