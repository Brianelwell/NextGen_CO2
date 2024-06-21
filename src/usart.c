
#include "main.h"

Buffer RxBuff;
Buffer TxBuff;
uint8_t ucCommTimeout = 0;
uint16_t uiCommit_Timer = 0;

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void USART1_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};		//  Initialize Structures

  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);	//  Enable Peripheral Clock

  NVIC_SetPriority(USART1_IRQn, 1);		//  Setup USART Interrupt
  NVIC_EnableIRQ(USART1_IRQn);
  LL_USART_EnableIT_RXNE(USART1);

  USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV4;
  USART_InitStruct.BaudRate = 19200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_9B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_EVEN;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_SetTXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_SetRXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_DisableFIFO(USART1);
  LL_USART_ConfigAsyncMode(USART1);

  /* USER CODE BEGIN WKUPType USART1 */
	SET_BIT(USART1->CR2, USART_CR2_SWAP);
  /* USER CODE END WKUPType USART1 */

  LL_USART_Enable(USART1);
	SET_BIT(USART1->ICR, USART_ICR_TCCF);		//  Clear Bit Before Enabling Interrupt
	SET_BIT(USART1->CR1, USART_CR1_TCIE);

  /* Polling USART1 initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
  {
  }

	USART1_Error_Callback();
	uiFlags.bI2C_Mode = false;
}

/**
  * @brief  Function called from USART IRQ Handler when RXNE flag is set
  *         Function is in charge of reading character received on USART RX line.
  * @param  None
  * @retval None
  */
void USART1_CharReception_Callback(void)
{
  __IO uint32_t received_char;

  //  Read Received character. RXNE flag is cleared by reading of RDR register
  received_char = LL_USART_ReceiveData8(USART1);
	if(!uiFlags.bUART_RcvInProcess)
	{
		RxBuff.ptr = 0;
		uiFlags.bUART_RcvInProcess = true;
	}
	queue_enqueue(RxBuff, received_char);
	ucCommTimeout = 0;					//  Reset Timeout Timer
}

/**
  * @brief  Function called from USART IRQ Handler when TC flag is set
  *         Function is in charge of sending characters on USART TX line.
  * @param  None
  * @retval None
  */
void USART1_SendMessage(void)
{
	uint16_t uiSend;
	
	if(uiFlags.bUART_XmtInProcess == false)
	{
		if(TxBuff.end != 0)
		{
			uiFlags.bUART_XmtInProcess = true;
			TxBuff.ptr = 0;
		}
	}

	SET_BIT(USART1->ICR, USART_ICR_TCCF);
	if(TxBuff.ptr < TxBuff.end)
	{
		uiSend = TxBuff.Buff[TxBuff.ptr ++];
		USART1->TDR = uiSend;
	}
	else
	{
		uiFlags.bUART_XmtInProcess = false;
	}
	ucCommTimeout = 0;					//  Reset Timeout Timer
}

/**
  * @brief  Function called in case of error detected in USART IT Handler
  * @param  None
  * @retval None
  */
void USART1_Error_Callback(void)
{
  volatile uint32_t isr_reg;

  isr_reg = LL_USART_ReadReg(USART1, ISR);
	if(isr_reg & LL_USART_ISR_PE)
	{
		SET_BIT(USART1->ICR, USART_ICR_PECF);
	}
	if(isr_reg & LL_USART_ISR_FE)
	{
		SET_BIT(USART1->ICR, USART_ICR_FECF);
	}
	if(isr_reg & LL_USART_ISR_NE)
	{
		SET_BIT(USART1->ICR, USART_ICR_NECF);
	}
	if(isr_reg & LL_USART_ISR_ORE)
	{
		SET_BIT(USART1->ICR, USART_ICR_ORECF);
	}
	if(isr_reg & LL_USART_ISR_IDLE)
	{
		SET_BIT(USART1->ICR, USART_ICR_IDLECF);
	}
	if(isr_reg & LL_USART_ISR_TXFE)
	{
	  WRITE_REG(USART1->TDR, isr_reg);
	}
	if(isr_reg & LL_USART_ISR_TC)
	{
		SET_BIT(USART1->ICR, USART_ICR_TCCF);
	}
}

/* -----------------------------------------------------------------------------
 * Put (insert) an element at the back of the queue. Will not overwrite
 * contents.
 */
bool queue_enqueue(Buffer &buff, uint8_t data)
{
  bool result = false;
  uint16_t new_end = buff.end + 1;
	
	buff.ptr = 0;
	if (new_end >= BUFFER_SIZE)
  {
    new_end = 0;
  }

  if (new_end != buff.ptr)
  {
    buff.Buff[buff.end] = data;
    buff.end = new_end;
    result = true;
  }

  return result;
}

/* -----------------------------------------------------------------------------
 * Gets (removes) an element from the front of the queue.
 * Note that if the head and tail indexes are equal then there is nothing in the
 * queue, regardless if it wrapped around or not.
 *//*
bool queue_dequeue(Buffer buff, uint8_t* data)
{
  bool result = false;

		buff.ptr = 0;
//  if (buff.start != buff.end)
  if (buff.end != 0)
  {
    *data = buff.Buff[buff.ptr++];

    if (buff.ptr >= BUFFER_SIZE)
    {
      buff.ptr = 0;
    }

    result = true;
  }

  return result;
} *//* dequeue */
