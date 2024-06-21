/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/USART/USART_Communication_Rx_IT_Init/Src/stm32c0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32c0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
	ulTicks ++;
	uiIntFlags.bTick_1mS = 1;
  if(++ucTick_10mS_acc >= 10)
  {
		uiIntFlags.bTick_10mS = 1;
	  ucTick_10mS_acc = 0;
  }
  if(++uiTick_1S_acc >= 1000)
  {
		uiIntFlags.bTick_1S = 1;
    uiTick_1S_acc = 0;

    uiTick_1S_Ctr ++;
    if(!(uiTick_1S_Ctr & 0x0f))
    {
			uiIntFlags.bRobust = 1;
    }
  }
}

/******************************************************************************/
/* STM32C0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32c0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line 4 to 15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_13) != RESET)
  {
    LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_13);

#ifdef DEMO_BOARD
		LED_Off();
    /* Handle user button press in dedicated function */
    UserButton_Callback();
#endif
  }
}

/**
  * @brief This function handles USART1 interrupt.
  */
void USART1_IRQHandler(void)
{
  volatile uint32_t isr_reg;
	
  /* USER CODE BEGIN USART1_IRQn 0 */
  isr_reg = LL_USART_ReadReg(USART1, ISR);
	if(isr_reg & 0x0f)
	{
#ifdef DEMO_BOARD		
		LED_On();
#endif
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
	}

  /* Check RXNE flag value in ISR register */
  if (LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
  {
    /* RXNE flag will be cleared by reading of RDR register (done in call) */
    /* Call function in charge of handling Character reception */
    USART1_CharReception_Callback();
  }
	else if(LL_USART_IsActiveFlag_TC(USART1))
	{
		USART1_SendMessage();
	}
  else
	{
		USART1_Error_Callback();		//  Cleanup
	}
}

/**
  * Brief   This function handles I2C1 (Slave) event interrupt request.
  * Param   None
  * Retval  None
  */
void I2C1_IRQHandler(void)
{
  /* Check ADDR flag value in ISR register */
  if (LL_I2C_IsActiveFlag_ADDR(I2C1))
  {
    /* Verify the Address Match with the OWN Slave address */
    if (LL_I2C_GetAddressMatchCode(I2C1) == 0x2a)	//0x15)		///SLAVE_OWN_ADDRESS)
    {
      /* Verify the transfer direction, a read direction, Slave enters transmitter mode */
      if (LL_I2C_GetTransferDirection(I2C1) == LL_I2C_DIRECTION_READ)
      {
        /* Clear ADDR flag value in ISR register */
        LL_I2C_ClearFlag_ADDR(I2C1);

        /* Enable Transmit Interrupt */
        LL_I2C_EnableIT_TX(I2C1);
      }
      else
      {
        /* Clear ADDR flag value in ISR register */
        LL_I2C_ClearFlag_ADDR(I2C1);
//				if(!uiFlags.bI2C_RcvInProcess)
//				{
				uiFlags.bI2C_RcvInProcess = true;
				RxBuff.Buff[0] = sHRegs.slave_address;
				RxBuff.ptr = 1;
//				}
//				else
//				{
//					RxBuff.Buff[RxBuff.ptr++] = LL_I2C_ReceiveData8(I2C1);
//				}
  
        /* Call Error function */
//        Error_Callback();
      }
    }
    else
    {
      /* Clear ADDR flag value in ISR register */
      LL_I2C_ClearFlag_ADDR(I2C1);

      /* Call Error function */
//      Error_Callback();
    }
  }
	else if(LL_I2C_IsActiveFlag_RXNE(I2C1))
	{
		RxBuff.Buff[RxBuff.ptr++] = LL_I2C_ReceiveData8(I2C1);
	}
  /* Check NACK flag value in ISR register */
  else if (LL_I2C_IsActiveFlag_NACK(I2C1))
  {
    /* End of Transfer */
    LL_I2C_ClearFlag_NACK(I2C1);
  }
  /* Check TXIS flag value in ISR register */
  else if (LL_I2C_IsActiveFlag_TXIS(I2C1))
  {
    /* Call function Slave Ready to Transmit Callback */
//    Slave_Ready_To_Transmit_Callback();
		I2C_SendMessage();
  }
  /* Check STOP flag value in ISR register */
  else if (LL_I2C_IsActiveFlag_STOP(I2C1))
  {
    /* Clear STOP flag value in ISR register */
    LL_I2C_ClearFlag_STOP(I2C1);
		uiFlags.bI2C_RcvInProcess = false;
//		Handle_Rcvd_Msg();
//  	LL_I2C_HandleTransfer(I2C1, sHRegs.slave_address, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
		
    /* Check TXE flag value in ISR register */
    if (!LL_I2C_IsActiveFlag_TXE(I2C1))
    {
      /* Flush the TXDR register */
      LL_I2C_ClearFlag_TXE(I2C1);
			uiFlags.bI2C_Mode = false;
    }

    /* Call function Slave Complete Callback */
//    Slave_Complete_Callback();
  }
  /* Check TXE flag value in ISR register */
  else if (!LL_I2C_IsActiveFlag_TXE(I2C1))
  {
    /* Do nothing */
    /* This Flag will be set by hardware when the TXDR register is empty */
    /* If needed, use LL_I2C_ClearFlag_TXE() interface to flush the TXDR register  */
  }
  else
  {
    /* Call Error function */
//    Error_Callback();
  }
}

/*
	// Check NACK flag value in ISR register 
	else if (LL_I2C_IsActiveFlag_NACK(I2C1))
	{
		// End of Transfer
		LL_I2C_ClearFlag_NACK(I2C1);
	}
	// Check TXIS flag value in ISR register
	else if (LL_I2C_IsActiveFlag_TXIS(I2C1))
	{
  	// Call function Slave Ready to Transmit Callback 
	  Slave_Ready_To_Transmit_Callback();
	}
	// Check STOP flag value in ISR register 
	else if (LL_I2C_IsActiveFlag_STOP(I2C1))
	{
		// Clear STOP flag value in ISR register 
		LL_I2C_ClearFlag_STOP(I2C1);

		// Check TXE flag value in ISR register
		if (!LL_I2C_IsActiveFlag_TXE(I2C1))
		{
			// Flush the TXDR register
			LL_I2C_ClearFlag_TXE(I2C1);
		}

	  // Call function Slave Complete Callback 
	  Slave_Complete_Callback();
	}
	// Check TXE flag value in ISR register 
	else if (!LL_I2C_IsActiveFlag_TXE(I2C1))
	{
		// Do nothing 
		// This Flag will be set by hardware when the TXDR register is empty
		// If needed, use LL_I2C_ClearFlag_TXE() interface to flush the TXDR register 
	}
	else
	{
		// Call Error function
		Error_Callback();
	}
}
*/

/**
  * @brief  This function handles ADC1 interrupt request.
  * @param  None
  * @retval None
  */
void ADC1_IRQHandler(void)
{
  /* Check whether ADC group regular end of unitary conversion caused         */
  /* the ADC interruption.                                                    */
  if(LL_ADC_IsActiveFlag_EOC(ADC1) != 0)
  {
    /* Clear flag ADC group regular end of unitary conversion */
    LL_ADC_ClearFlag_EOC(ADC1);

    /* Call interruption treatment function */
//    AdcGrpRegularUnitaryConvComplete_Callback();
		ulADC_Data = LL_ADC_REG_ReadConversionData32(ADC1);
  }

  /* Check whether ADC group regular overrun caused the ADC interruption */
  if(LL_ADC_IsActiveFlag_OVR(ADC1) != 0)
  {
    /* Clear flag ADC group regular overrun */
    LL_ADC_ClearFlag_OVR(ADC1);
  }
}
