#ifndef __ES_UART_CSP_DRV_H__
#define __ES_UART_CSP_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include <stdint.h>
#include <stddef.h>
/*********************************** DEFINES **********************************/

/*********************************** TYPEDEFS *********************************/

/******************************* INTERFACE FUNCTIONS **************************/
int8_t es_uart_csp_drv_init(uint32_t baudrate);
void es_uart_csp_drv_deinit(void);
int es_uart_csp_drv_tx(void *, const uint8_t *o_frame, size_t o_frame_len);

#ifdef __cplusplus
}
#endif

#endif
