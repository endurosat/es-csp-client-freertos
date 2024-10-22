#ifndef __ES_SYSTEM_H__
#define __ES_SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include <stdint.h>
/*********************************** DEFINES **********************************/

/*********************************** TYPEDEFS *********************************/

/******************************* INTERFACE FUNCTIONS **************************/
void es_system_hall_init(void);
uint32_t es_system_get_tick(void);
void es_system_clock_config(void);
void es_system_gpio_init(void);
void es_system_heartbeat(void);
void es_system_toggle_yellow_led(void);

#ifdef __cplusplus
}
#endif

#endif
