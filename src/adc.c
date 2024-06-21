/**
  ******************************************************************************
  * @file    Examples_LL/USART/USART_Communication_Rx_IT_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to send bytes over USART IP using
  *          the STM32C0xx USART LL API.
  *          Peripheral initialization done using LL initialization function.
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

#include "main.h"

uint32_t ulADC_Data;
uint16_t uiCO2_Measure_Tmr;

//uint16_t uiADC_CO2_Reading;
//uint16_t uiADC_Temp_Reading;
uint16_t uiADC_VIN_Reading;
uint16_t uiADC_VLamp_Plus_Reading;
uint16_t uiADC_VLamp_Minus_Reading;
uint16_t uiADC_Config_Reading;


/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
void ADC1_Init(void)
{
  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_SYSCLK);

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);

  // ADC1 interrupt Init
  NVIC_SetPriority(ADC1_IRQn, 2);
  NVIC_EnableIRQ(ADC1_IRQn);
  LL_ADC_EnableIT_EOC(ADC1);

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  ADC_InitStruct.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  LL_ADC_REG_SetSequencerConfigurable(ADC1, LL_ADC_REG_SEQ_FIXED);
	
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
	ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

	// Enable ADC Internal Voltage Regulator & ADC
	LL_ADC_EnableInternalRegulator(ADC1);				//  For Robust Routine
	LL_ADC_Enable(ADC1);												//  For Robust Routine
}

/**
  * @brief  Perform ADC activation procedure to make it ready to convert
  *         (ADC instance: ADC1).
  * @param  None
  * @retval None
  */
void ADC1_Activate(void)
{
  __IO uint32_t wait_loop_index = 0U;
  __IO uint32_t backup_setting_adc_dma_transfer = 0U;

	LL_ADC_Disable(ADC1);
	wait_loop_index = (10);
	while(wait_loop_index != 0)
	{
		wait_loop_index--;
	}
	
	if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    LL_ADC_EnableInternalRegulator(ADC1);    // Enable ADC Internal Regulator

		wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }

    backup_setting_adc_dma_transfer = LL_ADC_REG_GetDMATransfer(ADC1);
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);

    LL_ADC_StartCalibration(ADC1);   	 // Run ADC self calibration

    while(LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
    {
			;
		}

    /* Restore ADC DMA transfer request after calibration */
    LL_ADC_REG_SetDMATransfer(ADC1, backup_setting_adc_dma_transfer);

    /* Delay between ADC end of calibration and ADC enable.                   */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    wait_loop_index = (10);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }

    LL_ADC_Enable(ADC1);  			  // Enable ADC
	}
}

/**
  * @brief  ADC group regular overrun interruption callback
  * @note   This function is executed when ADC group regular
  *         overrun error occurs.
  * @retval None
  */
void AdcGrpRegularOverrunError_Callback(void)
{
  /* Note: Disable ADC interruption that caused this error before entering in
           infinite loop below. */

  /* In case of error due to overrun: Disable ADC group regular overrun interruption */
  LL_ADC_DisableIT_OVR(ADC1);

  /* Error reporting */
  Error_Handler();
}

uint16_t ADC_Measure(uint8_t ucChannel)
{
	uint16_t result = 0;
	
	ADC1->CHSELR = (1 << ucChannel);
	SET_BIT(ADC1->CR, ADC_CR_ADSTART);
	while((ADC1->ISR & ADC_ISR_EOS) == 0)
	{
		;
	}
	ADC1->ISR &= ~0x01;
	result = ADC1->DR;
	
	return result;
}
