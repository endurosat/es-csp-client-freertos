/*****************************************************************************
 * **File:** es_csp.h
 *
 * **Description:**
 */

/******************************** INCLUDE FILES *******************************/
#include <csp/csp.h>

#include <csp/interfaces/csp_if_kiss.h>
#include <csp/interfaces/csp_if_can.h>


#include "stm32h7xx_hal.h"
#include "es_hooks.h"

#include "es_csp.h"
/********************************** DEFINES ***********************************/
#define ES_CSP_UART_IFACE_NAME      "ES_UART"
#define ES_CSP_CAN_IFACE_NAME       "ES_CAN"

/******* CSP routing task *******/
#define ES_CSP_TASK_STACK_SIZE                   500
#define ES_CSP_TASK_PRIORITY                     0

#define ES_CSP_ROUTER_STACK         400
#define ES_CSP_SFP_CHUNK_SIZE       100
#define ES_CSP_MTU                  600
/********************************* TYPPEDEFES *********************************/
typedef int8_t (*es_int8_voidPtr_t) (void *);

typedef struct _es_csp_ctx_t {
    es_csp_phy_t physical;
    /* CSP Interface */
    csp_iface_t iface;
    /* Driver I/O handler  */
    es_csp_uart_drv_t *uart_drv;
    es_csp_can_drv_t *can_drv;

    /* Interfaces */
    csp_kiss_interface_data_t kiss_iface;
    csp_can_interface_data_t can_iface;
} es_csp_ctx_t;

/***************************** FUNCTION PROTOTYPES ****************************/
static int8_t es_csp_kiss_init(void *);
static int8_t es_csp_can_init(void *);

/******************************** LOCAL DATA **********************************/
static es_int8_voidPtr_t es_csp_phy_init[ES_CSP_PHY_LAYERS] = {
    es_csp_kiss_init,
    es_csp_can_init,
};
static es_csp_ctx_t es_csp_ctx;

static const char *physical_to_str[ES_CSP_PHY_LAYERS] = {
    "UART",
    "CAN"
};

/***************************** LOCAL FUNCTIONS ********************************/
static int8_t es_csp_kiss_init(void *args)
{
    int8_t ret = CSP_ERR_NONE;
    es_csp_conf_t *es_csp_conf = (es_csp_conf_t *)args;

    es_csp_ctx.uart_drv = (es_csp_uart_drv_t *) es_csp_conf->drv_iface;
    es_csp_ctx.kiss_iface.tx_func = es_csp_ctx.uart_drv->tx;
    es_csp_ctx.iface.name = ES_CSP_UART_IFACE_NAME;
    es_csp_ctx.iface.interface_data = &es_csp_ctx.kiss_iface;
    csp_kiss_add_interface(&es_csp_ctx.iface);
    if (ret != CSP_ERR_NONE)
        return -1;

    ret = csp_rtable_set(CSP_DEFAULT_ROUTE, 0, &es_csp_ctx.iface,
        CSP_NO_VIA_ADDRESS);
    if (ret != CSP_ERR_NONE)
        return -1;

    return 0;
}

static int8_t es_csp_can_init(void *args)
{
    int8_t ret = CSP_ERR_NONE;
    es_csp_conf_t *es_csp_conf = (es_csp_conf_t *)args;

    es_csp_ctx.can_drv = (es_csp_can_drv_t *) es_csp_conf->drv_iface;
    es_csp_ctx.can_iface.tx_func = es_csp_ctx.can_drv->tx;
    es_csp_ctx.iface.name = ES_CSP_CAN_IFACE_NAME;
    es_csp_ctx.iface.interface_data = &es_csp_ctx.can_iface;
    csp_can_add_interface(&es_csp_ctx.iface);
    if (ret != CSP_ERR_NONE)
    {
        return -1;
    }
    ret = csp_rtable_set(CSP_DEFAULT_ROUTE, 0, &es_csp_ctx.iface, CSP_NO_VIA_ADDRESS);
    if (ret != CSP_ERR_NONE)
    {
        return -1;
    }

    return 0;
}

/******************************* INTERFACE FUNCTIONS **************************/
int8_t es_csp_init(es_csp_conf_t *es_csp_conf)
{
    int ret = 0;

    csp_conf_t csp_conf;

    /* Init CSP with address and default settings */
    csp_conf_get_defaults(&csp_conf);
    csp_conf.address = es_csp_conf->s_addr;
    csp_conf.buffer_data_size = ES_CSP_MTU;
    ret = csp_init(&csp_conf);
    if (ret != CSP_ERR_NONE)
        return -1;

    es_csp_ctx.physical = es_csp_conf->phy_type;
    ret = es_csp_phy_init[es_csp_ctx.physical](es_csp_conf);
    if (ret != 0)
        return -1;
    csp_route_start_task(ES_CSP_TASK_STACK_SIZE, ES_CSP_TASK_PRIORITY);

    return 0;
}

int8_t es_csp_uart_frame_rx(const uint8_t *frame, uint32_t frame_len,
    void *px_task_woken)
{
    csp_kiss_rx(&es_csp_ctx.iface, frame, frame_len, px_task_woken);
    return 0;
}

int8_t es_csp_can_frame_rx(uint32_t id, const uint8_t *data, uint32_t dlc,
    void *px_task_woken)
{
    int ret = 0;

    ret = csp_can_rx(&es_csp_ctx.iface, id, data, dlc, px_task_woken);
    if (CSP_ERR_NONE != ret)
    {
        return -1;
    }

    return 0;
}
