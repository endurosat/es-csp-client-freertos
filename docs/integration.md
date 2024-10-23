# FreeRTOS integration

```{contents}
:depth: 3
```

The platform on which the `esclient-api` is used will expect that the CSP stack
has been initialized.

The following code block from the demo app which resides in the `app/src/main.c`
file initializes UART driver with baudrate of `115200` and uses this UART
as a KISS interface (see `es_csp.c`).

```C
    /**
     * Initialize UART driver & CSP lib. This functionality should not
     * be ported. It is only used to provide a working example.
     */
    es_csp_uart_drv_t uart_drv = es_csp_uart_drv_init_default;
    es_uart_csp_drv_init(ES_APPLICATION_UART_BAUDRATE);
    uart_drv.tx = es_uart_csp_drv_tx;
    es_csp_conf_t es_csp_conf;
    es_csp_conf.s_addr = 11;
    es_csp_conf.phy_type = ES_CSP_UART;
    es_csp_conf.drv_iface = &uart_drv;
    es_csp_init(&es_csp_conf);
```

**Note: To use CAN as physical layer instead of UART it should be initialized.**

The following intiliazation has been provided in the demo application:

```C
es_csp_can_drv_t can_drv = es_csp_can_drv_init_default;
can_drv.tx = es_can_csp_drv_tx; // NOTE:
/**
 * es_can_csp_drv_tx is a function of type:
 * int (*es_csp_can_tx_t)(void *, uint32_t, const uint8_t *, uint8_t)
 */
es_csp_conf_t es_csp_conf;
es_csp_conf.s_addr = ES_HOST_CSP_ADDRESS;
es_csp_conf.phy_type = ES_CSP_CAN;
es_csp_conf.drv_iface = &can_drv;
es_csp_init(&es_csp_conf);
```

After initializing the CSP stack these two functions should be called:

```C
    #define CSP_ADDRESS 10

    es_client_init();
    es_client_csp_tl_init(CSP_ADDRESS);
```

The first one basically initializes the command handlers and the second one the CSP
as transport layer for the `esclient-api` library. After performing these steps the
library should be functional and able to transfer files with `es_client_file_upload`
and `es_client_file_download`.

## FreeRTOS port

The following steps should be performed in order for the `esclient-api` to be able
to use CSP as tranport layer.

In this example code we are using UART as physical and the KISS protocol.

1. Initialize UART & attach rx handler (uart_csp_drv_rx_cb).

```C
/***
 * UART HAL Init
 *
 * reference file: app/src/es_uart_csp_drv.c
 **/
static UART_HandleTypeDef huart4; /* CSP UART Handle */
static volatile uint8_t i_byte = 0;
static BaseType_t pxTaskWoken = pdFALSE;

int uart_init()
{
    /**
     * Initialize UART, NVIC here
     * .
     * .
     * .
     */

    /* Attach csp rx handler ~ uart_csp_drv_rx_cb */
    HAL_UART_RegisterCallback(&huart4, HAL_UART_RX_COMPLETE_CB_ID,
        uart_csp_drv_rx_cb);

    HAL_UART_Receive_IT(&huart4, (uint8_t*)&i_byte, 1);
}

static
void es_uart_csp_drv_rx_cb(UART_HandleTypeDef* huart)
{
    es_csp_uart_frame_rx((const uint8_t *)&i_byte, 1, &pxTaskWoken);
    if (HAL_OK != HAL_UART_Receive_IT(&huart4, (uint8_t*)&i_byte, 1))
        return;
    es_system_toggle_yellow_led();
}

int uart_csp_drv_tx(void *drv_data, const uint8_t *o_frame, size_t o_frame_len)
{
    uint32_t timeout = 1000;
    HAL_UART_Transmit(&huart4, o_frame, o_frame_len, timeout);

    return 0;
}
```

* Initialize and configure UART peripheral attaching the `es_csp_uart_frame_rx (csp_kiss_rx)` handler to process the inbound byte stream.
* Initialize CSP lib & add kiss interface to it with a valid tx handler `es_uart_csp_drv_tx`.

[CSP Reference doc](https://libcsp.github.io/libcsp/api/interfaces/csp_if_kiss_h.html#interface-functions)

To use CAN:

* Initialize and configure CAN peripheral attaching the `es_csp_can_frame_rx (csp_can_rx)` handler to process the inbound byte stream.
* Initialize CSP lib & add kiss interface to it with a valid tx handler `es_can_csp_drv_tx`.

[CSP Reference doc](https://libcsp.github.io/libcsp/api/interfaces/csp_if_can_h.html#interface-functions)

2. Initialize CSP.

```C
#define ES_CSP_TASK_STACK_SIZE                   500
#define ES_CSP_TASK_PRIORITY                     0

/***
 * CSP Init
 *
 * reference file: app/src/es_csp.c
 ***/
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
    /** Note this function calls
    * the physical layer intialization (UART / CAN). The test setup has been only
    * used with UART.
    *
    */
    ret = es_csp_phy_init[es_csp_ctx.physical](es_csp_conf);

    if (ret != 0)
        return -1;
    csp_route_start_task(ES_CSP_TASK_STACK_SIZE, ES_CSP_TASK_PRIORITY);

    return 0;
}
```

The only difference is in the physical layer all the application code above is indiferent to the layer used.

3. Initialize the `esclient-api` lib and use it for communication with the module.

```C
    #include "esclient.h"

    uint16_t csp_address = 10;

    es_client_init();
    es_client_csp_tl_init(csp_address);

    ret = es_client_file_upload("/path/to/host/filesystem/filename",
     "/path/to/remote/filesystem/filename");
    if (0 != ret) {
        ES_LOG_MSG(ES_ERROR, "File upload failed");
    }
```
