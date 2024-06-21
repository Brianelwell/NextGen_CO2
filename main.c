/**
  ******************************************************************************
  * 											Next Generation CO2 Sensor
  * 											Amphenol Advanced Sensors
  *
  ******************************************************************************
  */

//  Includes
#include  "main.h"

//  Global Variables
VolFlags uiIntFlags;								//  Flags Inside Interrupts
Flags uiFlags;											//  Flags Outside Interrupts
uint32_t ulTicks = 0;
uint8_t  ucTick_10mS_acc = 0;
uint16_t uiTick_1S_acc = 0;
uint16_t uiTick_1S_Ctr = 0;
HoldRegs sHRegs;
HoldRegs *pHRegs = &sHRegs;
InputRegs sIRegs;
InputRegs *pIRegs = &sIRegs;

#ifdef DEMO_BOARD
uint16_t uiLED_Ctr = 0;
uint16_t uiLED_Interval = 100;
#endif

//  Function Prototypes
void SystemClock_Config(void);
void init(void);
void Handle_Tick(void);
void Handle_Tick_10mS(void);
void Handle_Tick_1S(void);
void Handle_Robust(void);
void IWDG_Init(void);

#ifdef DEMO_BOARD
/* USER CODE BEGIN PFP */
void     LED_On(void);
void     LED_Off(void);
/* USER CODE END PFP */
#endif

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	init();

  while (1)
  {
		if(uiIntFlags.bTick_1mS)
		{
			Handle_Tick();
		}
		if(uiIntFlags.bTick_10mS)
		{
			Handle_Tick_10mS();
		}
		if(uiIntFlags.bTick_1S)
		{
			Handle_Tick_1S();
		}
		if(uiIntFlags.bRobust)
		{
			Handle_Robust();
		}
  }
}

void init(void)
{
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  SystemClock_Config();			//  Configure System Clock

	//  Restore Holding Regs from Flash
	if(FlashRestore() == false)
	{
		FlashInitialize();
	}
	
	Handle_Robust();
	ADC1_Activate();

	IWDG_Init();							//  Configure Watchdog Timer
	uiFlags.bWarmUpMode = true;
}

void Handle_Tick(void)
{
	uiIntFlags.bTick_1mS = 0;
	
	gas_daq_task();

	if(++ucCommTimeout > 4)
	{
		if(uiFlags.bUART_RcvInProcess)
		{
			//  Validate Message
			if(RxBuff.Buff[0] == sHRegs.slave_address)		//  Confirm Address Match
			{
				if(Calc_crc(RxBuff) == 0)										//  Validate CRC
				{
					Handle_Rcvd_Msg();
				}
			}
			uiFlags.bUART_RcvInProcess = false;						//  Cleanup
			RxBuff.end = 0;
		}
		else if(uiFlags.bUART_XmtInProcess)
		{
			SET_BIT(USART1->ICR, USART_ICR_TCCF);					//  Ensure Bit Cleared
			uiFlags.bUART_XmtInProcess = false;						//  Cleanup
		}
		else
		{
			ucCommTimeout = 0;
		}
	}
}

void Handle_Tick_10mS(void)
{
	uiIntFlags.bTick_10mS = 0;

	uiCO2_Measure_Tmr ++;
}

void Handle_Tick_1S(void)
{
	uiIntFlags.bTick_1S = 0;
	sIRegs.up_time ++;
	
	if(uiFlags.bWarmUpMode)
	{
		if(sIRegs.up_time > sHRegs.warm_up_time)
		{
			uiFlags.bWarmUpMode = false;
		}
	}
	
	if(uiFlags.bFlashCommitInProcess == true)
	{
		if(++uiCommit_Timer >= FLASH_COMMIT_TIMEOUT)
		{
			FlashCommit();
			uiFlags.bFlashCommitInProcess = false;
			uiCommit_Timer = 0;	
		}
	}

	LL_IWDG_ReloadCounter(IWDG);
}

void Handle_Robust(void)
{
	uiIntFlags.bRobust = 0;
	
  GPIO_Init();
	ADC1_Init();
	TIM14_Init();
#ifndef FORCE_UART_MODE
	if(LL_GPIO_ReadReg(GPIOA, IDR) & 0x01)
	{
		I2C1_Init();
	}
	else
	{
	  USART1_Init();
	}
#else
	USART1_Init();
#endif

#ifdef DEMO_BOARD	
	LED_Off();
#endif
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  LL_RCC_HSI_Enable();  							// Active & Configure HSI
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(64);
  LL_RCC_SetHSIDiv(LL_RCC_HSI_DIV_1);
  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_HCLK_DIV_1);

  /* Sysclk activation on the HSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(48000000);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(48000000);
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
void IWDG_Init(void)
{
  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_32);
  LL_IWDG_SetReloadCounter(IWDG, 4078);
  while (LL_IWDG_IsReady(IWDG) != 1)
  {
  }
  LL_IWDG_ReloadCounter(IWDG);
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT Functions                                     */
/******************************************************************************/
/**
  * @brief  Function to manage User push-button
  * @param  None
  * @retval None
  */
#ifdef DEMO_BOARD
void UserButton_Callback(void)
{
  // Turn LED4 Off on User button press (allow to restart sequence)
  LED_Off();
}
#endif

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
