
#include "main.h"

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

	//  PA0 - Fixture Mode Digital Input
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	ClearGPIO_Struct(&GPIO_InitStruct);
	
	//  PA1 - Analog Input - VIN
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	ClearGPIO_Struct(&GPIO_InitStruct);
	
	//  PA2 - Digital Output - CO2 Alarm
	
	//  PA3 - Digital Output - Lamp Driver Power
	
	//  PA4 - Digital Output - Lamp Driver PWM
	GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;			//LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	ClearGPIO_Struct(&GPIO_InitStruct);

	//  PA5 - Analog Input - V_Lamp-  ///// Demo Board LED
	
	//  PA6 - (((Not Used)))
	
	//  PA7 - Analog Input - V_Lamp+
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	ClearGPIO_Struct(&GPIO_InitStruct);
	
	//  PA8 - Analog Input - CO2 Signal
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	ClearGPIO_Struct(&GPIO_InitStruct);

	//  PA9 - Analog Input - Detector Temperature
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	ClearGPIO_Struct(&GPIO_InitStruct);
	
	//  PA10 - (((Not Used)))

	//  PB6 - USART TX (Need Swap Enabled) or SCL
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	if(LL_GPIO_ReadReg(GPIOA, IDR) & 0x01)
	{
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
		GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
#ifdef FORCE_UART_MODE
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
#endif
	}
	else
	{
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	}
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	ClearGPIO_Struct(&GPIO_InitStruct);

	//  PB7 - USART RX (Need Swap Enabled) or SDA
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	if(LL_GPIO_ReadReg(GPIOA, IDR) & 0x01)
	{
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;	
		GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
#ifdef FORCE_UART_MODE
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
#endif
	}
	else
	{
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	}
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	ClearGPIO_Struct(&GPIO_InitStruct);
}

void ClearGPIO_Struct(LL_GPIO_InitTypeDef *sGPIO)
{
	sGPIO->Pin = 0;
	sGPIO->Mode = 0;
	sGPIO->Speed = 0;
	sGPIO->OutputType = 0;
	sGPIO->Pull = 0;
	sGPIO->Alternate = 0;
}

/**
  * @brief  Turn-on LED4.
  * @param  None
  * @retval None
  */
#ifdef DEMO_BOARD
void LED_On(void)
{
  /* Turn LED4 on */
  LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
}
#endif

/**
  * @brief  Turn-off LED4.
  * @param  None
  * @retval None
  */
#ifdef DEMO_BOARD
void LED_Off(void)
{
  /* Turn LED4 off */
  LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);
}
#endif
