

#include "main.h"

#define SLAVE_BOARD

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

//  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

//  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1);

//  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB8   ------> I2C1_SCL
  PB9   ------> I2C1_SDA
  */
/*  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
*/
  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */
  /* Configure Event and Error IT:
   *  - Set priority for I2C1_IRQn
   *  - Enable I2C1_IRQn
   */
  NVIC_SetPriority(I2C1_IRQn, 1);
  NVIC_EnableIRQ(I2C1_IRQn);
  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
//  I2C_InitStruct.Timing = 0x0090273D;
  I2C_InitStruct.Timing = 0x4090273D;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 2;
  I2C_InitStruct.OwnAddress1 = sHRegs.slave_address << 1;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
	
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  /* USER CODE BEGIN I2C1_Init 2 */
#ifdef SLAVE_BOARD
  uint32_t timing = 0;
  timing = __LL_I2C_CONVERT_TIMINGS(0x0, 0xC, 0x0, 0x21, 0x6C);
  LL_I2C_SetTiming(I2C1, timing);

  LL_I2C_SetOwnAddress1(I2C1, sHRegs.slave_address, LL_I2C_OWNADDRESS1_7BIT);
  LL_I2C_EnableOwnAddress1(I2C1);

  LL_I2C_EnableIT_ADDR(I2C1);
#else
//  LL_I2C_SetTiming(I2C1, I2C_TIMING);

  LL_I2C_EnableIT_RX(I2C1);
#endif
  LL_I2C_EnableIT_RX(I2C1);
  LL_I2C_EnableIT_NACK(I2C1);
  LL_I2C_EnableIT_ERR(I2C1);
  LL_I2C_EnableIT_STOP(I2C1);
  /* USER CODE END I2C1_Init 2 */
	uiFlags.bI2C_Mode = true;
}

void I2C_SendMessage(void)
{
	uint16_t uiSend;
	
	if(uiFlags.bI2C_XmtInProcess == false)
	{
		if(TxBuff.end != 0)
		{
			uiFlags.bI2C_XmtInProcess = true;
			TxBuff.ptr = 0;
//			LL_I2C_GENERATE_START_WRITE();
		}
	}

	if(TxBuff.ptr < TxBuff.end)
	{
		uiSend = TxBuff.Buff[TxBuff.ptr ++];
	  LL_I2C_TransmitData8(I2C1, uiSend);		
//		I2C1->TXDR = uiSend;
//		SET_BIT(I2C1->CR1, I2C_CR1_TXIE);
	}
	else
	{
		////// send stop bit
		CLEAR_BIT(I2C1->CR1, I2C_CR1_TXIE);
		uiFlags.bI2C_XmtInProcess = false;
	}
//	ucCommTimeout = 0;					//  Reset Timeout Timer
}

/*
void Slave_Ready_To_Transmit_Callback(void)
{
	;
}

void Slave_Complete_Callback(void)
{
	;
}
*/
/*
void I2C1_IRQHandler(void)
{
	;
}
*/