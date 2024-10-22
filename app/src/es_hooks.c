/******************************** INCLUDE FILES *******************************/
#include "stm32h7xx_hal.h"

/****************************** INTERFACE FUNCTIONS ***************************/
void es_hooks_err_hndlr(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
    __disable_irq();
    for (;;)
    {
    }
}
