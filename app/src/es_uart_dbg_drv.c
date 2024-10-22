/******************************** INCLUDE FILES *******************************/
#include "stm32h7xx_hal.h"

#include "es_uart_dbg_drv.h"
#include "es_hooks.h"
/********************************** DEFINES ***********************************/
#define STLINK_RX_Pin GPIO_PIN_8
#define STLINK_RX_GPIO_Port GPIOD
#define STLINK_TX_Pin GPIO_PIN_9
#define STLINK_TX_GPIO_Port GPIOD

/******************************* TYPPEDEFES ***********************************/

/******************************** LOCAL DATA **********************************/
static UART_HandleTypeDef huart3; /* Debug UART */

/***************************** LOCAL FUNCTIONS ********************************/
static void es_uart_dbg_drv_hal_init(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if(huart->Instance == USART3)
    {
        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
            es_hooks_err_hndlr();

        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**USART3 GPIO Configuration
         PD8     ------> USART3_TX
         PD9     ------> USART3_RX
        */
        GPIO_InitStruct.Pin = STLINK_RX_Pin|STLINK_TX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    }
}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
static void es_uart_dbg_drv_hal_deinit(UART_HandleTypeDef* huart)
{
    if(huart->Instance == USART3)
    {
        __HAL_RCC_USART3_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOD, STLINK_RX_Pin|STLINK_TX_Pin);
    }
}

/******************************* INTERFACE FUNCTIONS **************************/
/**
  * @brief USART3 Initialization Function
  */
int8_t es_uart_dbg_drv_init(uint32_t baudrate)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = baudrate;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    es_uart_dbg_drv_hal_init(&huart3);

    if (HAL_UART_Init(&huart3) != HAL_OK)
        return -1;

    if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
        return -1;

    if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
        return -1;

    if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
        return -1;

    return 0;
}

void es_uart_dbg_drv_deinit(void) {
    es_uart_dbg_drv_hal_deinit(&huart3);
}

int es_uart_dbg_drv_tx( const uint8_t *buffer, size_t buffer_len, uint32_t timeout)
{
    if (HAL_UART_Transmit(&huart3, buffer, buffer_len, timeout) != HAL_OK)
        return -1;

    return 0;
}
