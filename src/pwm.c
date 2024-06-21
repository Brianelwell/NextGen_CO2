

#include "main.h"

bool SetDutyCycle(uint8_t uiDutyCycle)
{
	if(uiDutyCycle > 100)
	{
		return false;
	}
	
	LL_TIM_WriteReg(TIM14, CCR1, uiDutyCycle);
	return true;
}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
void TIM14_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
//	uint32_t TimOutClock = SystemCoreClock/1;
  uint32_t timxPrescaler = __LL_TIM_CALC_PSC(SystemCoreClock, 50000);
//  uint32_t timxPeriod = __LL_TIM_CALC_ARR(TimOutClock, timxPrescaler, 100);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM14); 		// Peripheral clock enable

  TIM_InitStruct.Prescaler = timxPrescaler;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 100u;		//__LL_TIM_CALC_ARR(TimOutClock, timxPrescaler, 1000u);
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM14, &TIM_InitStruct);
	
  LL_TIM_EnableARRPreload(TIM14);
  LL_TIM_OC_EnablePreload(TIM14, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;       //LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;		//LL_TIM_OCSTATE_DISABLE;
//  TIM_OC_InitStruct.CompareValue = ((timxPeriod + 1 ) / 2);
  TIM_OC_InitStruct.CompareValue = LL_TIM_ReadReg(TIM14, CCR1);
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM14, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
	
  LL_TIM_OC_DisableFast(TIM14, LL_TIM_CHANNEL_CH1);
  LL_TIM_SetTriggerOutput(TIM14, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM14);

  LL_TIM_CC_EnableChannel(TIM14, LL_TIM_CHANNEL_CH1);
  LL_TIM_EnableCounter(TIM14);

//	LL_TIM_GenerateEvent_UPDATE(TIM14);		/// ???
	
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
/*
void TIM1_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
	uint32_t TimOutClock = SystemCoreClock/1;
  uint32_t timxPrescaler = __LL_TIM_CALC_PSC(SystemCoreClock, 50000);
  uint32_t timxPeriod = __LL_TIM_CALC_ARR(TimOutClock, timxPrescaler, 100);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1); 		// Peripheral clock enable

  TIM_InitStruct.Prescaler = timxPrescaler;		//959u;			//timxPrescaler;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = __LL_TIM_CALC_ARR(TimOutClock, timxPrescaler, 1000u);
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM1, &TIM_InitStruct);
	
  LL_TIM_EnableARRPreload(TIM1);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH2N);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;		//LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = ((timxPeriod + 1 ) / 2);
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStruct);
	
  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH2N);
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM1);

  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2N);
  LL_TIM_EnableCounter(TIM1);

	LL_TIM_GenerateEvent_UPDATE(TIM1);		/// ???
	
}
*/