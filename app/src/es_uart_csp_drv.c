/******************************** INCLUDE FILES *******************************/
#include <string.h>

#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"

#include "es_system.h"
#include "es_hooks.h"
#include "es_csp.h"
/********************************** DEFINES ***********************************/

/******************************** TYPPEDEFES **********************************/

/******************************** LOCAL DATA **********************************/
static UART_HandleTypeDef huart4; /* CSP UART Handle */
static volatile uint8_t i_byte;
static BaseType_t pxTaskWoken = pdFALSE; // pdTRUE;

/***************************** LOCAL FUNCTIONS ********************************/
static void es_uart_csp_drv_hal_init(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	if(huart->Instance==UART4)
	{
		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART4;
		PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
			es_hooks_err_hndlr();

		/* Peripheral clock enable */
		__HAL_RCC_UART4_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		/**
		 * UART4 GPIO Configuration
		 * PA0     ------> UART4_TX
		 * PC11     ------> UART4_RX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
	}
}

static void es_uart_csp_drv_hal_deinit(UART_HandleTypeDef* huart)
{
	if(huart->Instance == UART4)
	{
		/* Peripheral clock disable */
		__HAL_RCC_UART4_CLK_DISABLE();

		/**
		 * UART4 GPIO Configuration
		 * PA0     ------> UART4_TX
		 * PC11     ------> UART4_RX
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_11);

		/* UART4 interrupt DeInit */
		HAL_NVIC_DisableIRQ(UART4_IRQn);
	}
}

static
void es_uart_csp_drv_rx_cb(UART_HandleTypeDef* huart)
{
	es_csp_uart_frame_rx((const uint8_t *)&i_byte, 1, &pxTaskWoken);
	if (HAL_OK != HAL_UART_Receive_IT(&huart4, (uint8_t*)&i_byte, 1))
		return;
	es_system_toggle_yellow_led();
}

/******************************* INTERFACE FUNCTIONS **************************/
/**
  * @brief UART4 Initialization Function
  */
int8_t es_uart_csp_drv_init(uint32_t baudrate)
{
	huart4.Instance = UART4;
	huart4.Init.BaudRate = baudrate;
	huart4.Init.WordLength = UART_WORDLENGTH_8B;
	huart4.Init.StopBits = UART_STOPBITS_1;
	huart4.Init.Parity = UART_PARITY_NONE;
	huart4.Init.Mode = UART_MODE_TX_RX;
	huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	es_uart_csp_drv_hal_init(&huart4);

	if (HAL_UART_Init(&huart4) != HAL_OK)
		return -1;

	if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
		return -1;

	if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
		return -1;

	if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
		return -1;

  if (HAL_OK != HAL_UART_RegisterCallback(&huart4, HAL_UART_RX_COMPLETE_CB_ID,
  	es_uart_csp_drv_rx_cb))
      return -1;

  if (HAL_OK != HAL_UART_Receive_IT(&huart4, (uint8_t*)&i_byte, 1))
      return -1;

	return 0;
}

void es_uart_csp_drv_deinit(void)
{
	es_uart_csp_drv_hal_deinit(&huart4);
}

int es_uart_csp_drv_tx(void *drv_data, const uint8_t *o_frame, size_t o_frame_len)
{
	UNUSED(drv_data);

	uint32_t timeout = 1000;
	HAL_UART_Transmit(&huart4, o_frame, o_frame_len, timeout);
	es_system_toggle_yellow_led();

	return 0;
}

void UART4_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart4);
}
