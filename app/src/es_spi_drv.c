/******************************** INCLUDE FILES *******************************/
#include "stm32h7xx_hal.h"

#include "es_hooks.h"
/********************************** DEFINES ***********************************/

/******************************** TYPPEDEFES **********************************/

/******************************** LOCAL DATA **********************************/
SPI_HandleTypeDef hspi2;

/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void es_spi_drv_hal_init(SPI_HandleTypeDef* hspi)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	if(hspi->Instance==SPI2)
  	{
  		/** Initializes the peripherals clock
  		*/
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI2;
		PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
			es_hooks_err_hndlr();


		/* Peripheral clock enable */
		__HAL_RCC_SPI2_CLK_ENABLE();

		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		/**
		 * Tuki shi trebva da sa vkliuchi i chip selecta
		 */
		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);

		/** Configure GPIO pin : PB11
 			PB11     ------> SPI2_CS
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		/** SPI2 GPIO Configuration
			PC2_C     ------> SPI2_MISO
			PC3_C     ------> SPI2_MOSI
			PB10     ------> SPI2_SCK
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}

/**
* @brief SPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void es_spi_drv_hal_deinit(SPI_HandleTypeDef* hspi)
{
 	if(hspi->Instance==SPI2)
  	{
		/* Peripheral clock disable */
		__HAL_RCC_SPI2_CLK_DISABLE();
		/**SPI2 GPIO Configuration
		PC2_C     ------> SPI2_MISO
		PC3_C     ------> SPI2_MOSI
		PB10     ------> SPI2_SCK
		*/
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2|GPIO_PIN_3);

		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);
	}
}

/******************************* INTERFACE FUNCTIONS **************************/
int8_t es_spi_drv_init(void)
{
	/* SPI2 parameter configuration*/
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 0;
	hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;

  	es_spi_drv_hal_init(&hspi2);

	if (HAL_SPI_Init(&hspi2) != HAL_OK)
		return -1;

	return 0;
}

void es_spi_drv_deinit(void)
{
	es_spi_drv_hal_deinit(&hspi2);
}
