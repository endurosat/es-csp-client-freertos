/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

void HAL_MspInit(void)
{
  	__HAL_RCC_SYSCFG_CLK_ENABLE();
  	/* System interrupt init*/
  	/* PendSV_IRQn interrupt configuration */
  	HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
}
