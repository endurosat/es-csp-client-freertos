/******************************** INCLUDE FILES *******************************/
#include "cmsis_os2.h"

#include "es_system.h"
#include "es_tasks.h"
#include "es_spi_drv.h"
#include "es_uart_dbg_drv.h"
#include "es_uart_csp_drv.h"
#include "es_csp.h"
#include "es_sd_card.h"

#include "esclient.h"
/******************************** LOCAL DEFINES *******************************/
#define ES_MODULE_NAME "main"

#define ES_DBG_UART_BAUDRATE 115200
#define ES_APPLICATION_UART_BAUDRATE 115200
#define ES_HOST_CSP_ADDRESS 11U
#define ES_CSP_ADDRESS 10U
/********************************** TYPEDEFS **********************************/


/****************************** MAIN ENTRY POINT ******************************/
int main(void) {
	es_log_iface_t log_iface = es_log_iface_init_default;
    es_csp_uart_drv_t uart_drv = es_csp_uart_drv_init_default;

	es_system_hall_init();
	es_system_gpio_init();
	es_spi_drv_init();
	/* Configure the system clock */
	es_system_clock_config();
	es_uart_dbg_drv_init(ES_DBG_UART_BAUDRATE);
    es_uart_csp_drv_init(ES_APPLICATION_UART_BAUDRATE);
    es_sd_card_fatfs_init();

    /* Init scheduler */
    osKernelInitialize();

	/**
	 * To disable the logging comment out this part.
	 */
	log_iface.timestamp = es_system_get_tick;
	log_iface.tx = es_uart_dbg_drv_tx;
    es_log_init(&log_iface);
	es_log_set_lvl(ES_TRACE);
	ES_LOG_MSG(ES_INFO, "ES Client v%d", ESCLIENT_VERSION);

    es_sd_card_mount();

    /**
     * Initialize UART driver & CSP lib. This functionality should not
     * be ported. It is only used to provide a working example.
     */
    uart_drv.tx = es_uart_csp_drv_tx;
    es_csp_conf_t es_csp_conf;
    es_csp_conf.s_addr = ES_HOST_CSP_ADDRESS;
    es_csp_conf.phy_type = ES_CSP_UART;
    es_csp_conf.drv_iface = &uart_drv;

    /****
     *
     * __CAN_TODO__
     *
     *  This is a refenrence implmentation and has not been tested
     *
     *   es_csp_can_drv_t can_drv = es_csp_can_drv_init_default;
     *   can_drv.tx = es_can_csp_drv_tx; // NOTE:
     *
     *   es_can_csp_drv_tx is a function of type:
     *      int (*es_csp_can_tx_t)(void *, uint32_t, const uint8_t *, uint8_t)
     *
     *   es_csp_conf_t es_csp_conf;
     *   es_csp_conf.s_addr = ES_HOST_CSP_ADDRESS;
     *   es_csp_conf.phy_type = ES_CSP_CAN;
     *   es_csp_conf.drv_iface = &can_drv;
     *   es_csp_init(&es_csp_conf);
     */
    es_csp_init(&es_csp_conf);

    /* Initialize client lib */
    es_client_init();
    es_client_deinit();

    /* Initialize CSP as tranport layer */
    es_client_csp_tl_init(ES_CSP_ADDRESS);

	es_tasks_init();

    /* Start scheduler */
    osKernelStart();
	/* We should never get here as control is now taken by the scheduler */

	return 0;
}
