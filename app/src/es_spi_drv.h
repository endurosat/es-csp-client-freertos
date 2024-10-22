#ifndef __ES_SPI_DRV_H__
#define __ES_SPI_DRV_H__

/******************************** INCLUDE FILES *******************************/
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************** DEFINES **********************************/

/*********************************** TYPEDEFS *********************************/

/******************************* INTERFACE FUNCTIONS **************************/
int8_t es_spi_drv_init(void);
void es_spi_drv_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
