#ifndef __ES_UART_DBG_DRV_H__
#define __ES_UART_DBG_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include <stdint.h>
#include <stddef.h>
/*********************************** DEFINES **********************************/

/*********************************** TYPEDEFS *********************************/

/******************************* INTERFACE FUNCTIONS **************************/
int8_t es_uart_dbg_drv_init(uint32_t baudrate);
void es_uart_dbg_drv_deinit(void);
int es_uart_dbg_drv_tx(const uint8_t *buffer, size_t buffer_len, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif
