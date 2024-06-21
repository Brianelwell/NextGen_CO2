/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/USART/USART_Communication_Rx_IT_Init/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEMO_BOARD
//#define FORCE_UART_MODE

/* Includes ------------------------------------------------------------------*/
#include  <stdint.h>
#include  <stddef.h>
#include "stm32c0xx.h"
#include "stm32c0xx_ll_iwdg.h"
#include "stm32c0xx_ll_rcc.h"
#include "stm32c0xx_ll_system.h"
#include "stm32c0xx_ll_adc.h"
#include "stm32c0xx_ll_bus.h"
#include "stm32c0xx_ll_exti.h"
#include "stm32c0xx_ll_utils.h"
#include "stm32c0xx_ll_pwr.h"
#include "stm32c0xx_ll_gpio.h"
#include "stm32c0xx_ll_usart.h"
#include "stm32c0xx_ll_tim.h"
#include "stm32c0xx_ll_i2c.h"
#include "T68xx.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include "common.h"
#include "gas_daq.h"
#include "flash.h"
#include "pwm.h"
#include "i2c.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

typedef struct
{
	volatile bool bTick_1mS;			//	: 1
	volatile bool bTick_10mS;			//	: 2
	volatile bool bTick_1S;				//	: 4
	volatile bool bRobust;				//	: 8
} VolFlags;

typedef struct
{
	bool bUART_RcvInProcess;			//	: 1
	bool bUART_XmtInProcess;			//	: 2
	bool bCO2_MeasureInProcess;		//  : 4
	bool bFlashCommitInProcess;		//  : 8
	bool bI2C_Mode;								//  : 16
	bool bI2C_RcvInProcess;				//  : 32
	bool bI2C_XmtInProcess;				//	: 64
} Flags;

typedef struct
{
	uint16_t model_number = DEFAULT_MODEL_NUMBER;									// 4000
	uint16_t mfg_data = DEFAULT_MFG_DATA;
	uint32_t serial_number = DEFAULT_SERIAL_NUMBER;
	uint16_t ops_flag = DEFAULT_OPS_FLAG;
	uint16_t slave_address = DEFAULT_SLAVE_ADDRESS;
	uint16_t baud_rate = DEFAULT_BAUD_RATE;
	uint16_t parity = DEFAULT_PARITY;
	uint16_t sample_time = DEFAULT_SAMPLE_TIME;
	uint16_t nadir_time = DEFAULT_NADIR_TIME;
	uint16_t zenith_time = DEFAULT_ZENITH_TIME;										// 4010
	uint16_t cal_samples = DEFAULT_CAL_SAMPLES;
	float temp_divisor = DEFAULT_TEMP_DIVISOR;
	uint16_t co2_bound_limit_hi = DEFAULT_CO2_BOUND_LIMIT_HI;
	uint16_t co2_bound_limit_lo = DEFAULT_CO2_BOUND_LIMIT_LO;
	uint16_t zero_cal_target_ppm = DEFAULT_ZERO_CAL_TARGET_PPM;
	float zero_cal_result = DEFAULT_ZERO_CAL_RESULT;
	uint16_t span1_cal_target_ppm = DEFAULT_SPAN1_CAL_TARGET_PPM;
	float span1_cal_result = DEFAULT_SPAN1_CAL_RESULT;						// 4020
	float span1_zero_ratio = DEFAULT_SPAN1_ZERO_RATIO;
	uint16_t span2_cal_target_ppm = DEFAULT_SPAN2_CAL_TARGET_PPM;
	float span2_cal_result = DEFAULT_SPAN2_CAL_RESULT;
	float span2_zero_ratio = DEFAULT_SPAN2_ZERO_RATIO;
	uint16_t sngpt_cal_target_ppm = DEFAULT_SNGPT_CAL_TARGET_PPM;
	float tcor_factor = DEFAULT_TCOR_FACTOR;											// 4030
	float tcor1_coeff_x0 = DEFAULT_TCOR1_COEFF_X0;
	float tcor1_coeff_x1 = DEFAULT_TCOR1_COEFF_X1;
	float tcor1_coeff_x2 = DEFAULT_TCOR1_COEFF_X2;
	float tcor1_coeff_x3 = DEFAULT_TCOR1_COEFF_X3;
	float tcor2_coeff_x0 = DEFAULT_TCOR2_COEFF_X0;								// 4040
	float tcor2_coeff_x1 = DEFAULT_TCOR2_COEFF_X1;
	float tcor2_coeff_x2 = DEFAULT_TCOR2_COEFF_X2;
	float tcor2_coeff_x3 = DEFAULT_TCOR2_COEFF_X3;
	float tcor3_coeff_x0 = DEFAULT_TCOR3_COEFF_X0;
	float tcor3_coeff_x1 = DEFAULT_TCOR3_COEFF_X1;								// 4050
	float tcor3_coeff_x2 = DEFAULT_TCOR3_COEFF_X2;
	float tcor3_coeff_x3 = DEFAULT_TCOR3_COEFF_X3;
	float gas1_ppm_coeff_x0 = DEFAULT_GAS1_PPM_COEFF_X0;
	float gas1_ppm_coeff_x1 = DEFAULT_GAS1_PPM_COEFF_X1;
	float gas1_ppm_coeff_x2 = DEFAULT_GAS1_PPM_COEFF_X2;					// 4060
	float gas1_ppm_coeff_x3 = DEFAULT_GAS1_PPM_COEFF_X3;
	float gas2_ppm_coeff_x0 = DEFAULT_GAS2_PPM_COEFF_X0;
	float gas2_ppm_coeff_x1 = DEFAULT_GAS2_PPM_COEFF_X1;
	float gas2_ppm_coeff_x2 = DEFAULT_GAS2_PPM_COEFF_X2;
	float gas2_ppm_coeff_x3 = DEFAULT_GAS2_PPM_COEFF_X3;					// 4070
	float gas2_ppm_coeff_x4 = DEFAULT_GAS2_PPM_COEFF_X4;
	float gas2_ppm_coeff_x5 = DEFAULT_GAS2_PPM_COEFF_X5;
	float gas2_ppm_coeff_x6 = DEFAULT_GAS2_PPM_COEFF_X6;
	float gas2_ppm_coeff_x7 = DEFAULT_GAS2_PPM_COEFF_X7;
	uint16_t lamp_data_time_0 = DEFAULT_LAMP_DATA_TIME_0;					// 4080
	uint16_t lamp_data_volt_0 = DEFAULT_LAMP_DATA_VOLT_0;
	uint16_t lamp_data_time_1 = DEFAULT_LAMP_DATA_TIME_1;
	uint16_t lamp_data_volt_1 = DEFAULT_LAMP_DATA_VOLT_1;
	uint16_t lamp_data_time_2 = DEFAULT_LAMP_DATA_TIME_2;
	uint16_t lamp_data_volt_2 = DEFAULT_LAMP_DATA_VOLT_2;
	float abc_correlation_factor = DEFAULT_ABC_CORRELATION_FACTOR;
	uint16_t abc_sample_rate = DEFAULT_ABC_SAMPLE_RATE;
	uint16_t abc_eval_count = DEFAULT_ABC_EVAL_COUNT;
	uint16_t abc_sample_count = DEFAULT_ABC_SAMPLE_COUNT;					// 4090
	float dsp_alpha_coeff = DEFAULT_DSP_ALPHA_COEFF;
	float dsp_adapt_coeff = DEFAULT_DSP_ADAPT_COEFF;
	float dsp_adapt_bound = DEFAULT_DSP_ADAPT_BOUND;
	uint16_t gas_ppm_upper_bound_1 = DEFAULT_GAS_UPPER_BOUND_1;
	uint16_t gas_ppm_upper_bound_2 = DEFAULT_GAS_UPPER_BOUND_2;
	float altitude = DEFAULT_ALTITUDE;														// 4100
	float cal_altitude = DEFAULT_CAL_ALTITUDE;
	float pressure = DEFAULT_PRESSURE;
	uint16_t warm_up_time = DEFAULT_WARM_UP_TIME;
	uint16_t dac_ctrl = DEFAULT_DAC_CTRL;
	float first_sample_cf = DEFAULT_FIRST_SAMPLE_CF;
	uint16_t avg_ctrl = DEFAULT_AVG_CTRL;													// 4109
} HoldRegs;					//  Size = 220 Bytes (20MAY2024), 230 With Boundary Skips

#define UNSIGNED_INTEGER		1
#define UNSIGNED_LONG				2
#define FLOAT_VALUE					3
typedef struct
{
	uint8_t model_number = UNSIGNED_INTEGER;						// 4000
	uint8_t mfg_data = UNSIGNED_INTEGER;
	uint8_t serial_number = UNSIGNED_LONG;
	uint8_t ops_flag = UNSIGNED_INTEGER;
	uint8_t slave_address = UNSIGNED_INTEGER;
	uint8_t baud_rate = UNSIGNED_INTEGER;
	uint8_t parity = UNSIGNED_INTEGER;
	uint8_t sample_time = UNSIGNED_INTEGER;
	uint8_t nadir_time = UNSIGNED_INTEGER;
	uint8_t zenith_time = UNSIGNED_INTEGER;							// 4010 - Qty 10
	uint8_t cal_samples = UNSIGNED_INTEGER;
	uint8_t temp_divisor = FLOAT_VALUE;
	uint8_t co2_bound_limit_hi = UNSIGNED_INTEGER;
	uint8_t co2_bound_limit_lo = UNSIGNED_INTEGER;
	uint8_t zero_cal_target_ppm = UNSIGNED_INTEGER;
	uint8_t zero_cal_result = FLOAT_VALUE;
	uint8_t span1_cal_target_ppm = UNSIGNED_INTEGER;
	uint8_t span1_cal_result = FLOAT_VALUE;							// 4020
	uint8_t span1_zero_ratio = FLOAT_VALUE;
	uint8_t span2_cal_target_ppm = UNSIGNED_INTEGER;		// Qty - 20
	uint8_t span2_cal_result = FLOAT_VALUE;
	uint8_t span2_zero_ratio = FLOAT_VALUE;
	uint8_t sngpt_cal_target_ppm = UNSIGNED_INTEGER;
	uint8_t tcor_factor = FLOAT_VALUE;									// 4030
	uint8_t tcor1_coeff_x0 = FLOAT_VALUE;
	uint8_t tcor1_coeff_x1 = FLOAT_VALUE;
	uint8_t tcor1_coeff_x2 = FLOAT_VALUE;
	uint8_t tcor1_coeff_x3 = FLOAT_VALUE;
	uint8_t tcor2_coeff_x0 = FLOAT_VALUE;								// 4040
	uint8_t tcor2_coeff_x1 = FLOAT_VALUE;								// Qty - 30
	uint8_t tcor2_coeff_x2 = FLOAT_VALUE;
	uint8_t tcor2_coeff_x3 = FLOAT_VALUE;
	uint8_t tcor3_coeff_x0 = FLOAT_VALUE;
	uint8_t tcor3_coeff_x1 = FLOAT_VALUE;								// 4050
	uint8_t tcor3_coeff_x2 = FLOAT_VALUE;
	uint8_t tcor3_coeff_x3 = FLOAT_VALUE;
	uint8_t gas1_ppm_coeff_x0 = FLOAT_VALUE;
	uint8_t gas1_ppm_coeff_x1 = FLOAT_VALUE;
	uint8_t gas1_ppm_coeff_x2 = FLOAT_VALUE;						// 4060
	uint8_t gas1_ppm_coeff_x3 = FLOAT_VALUE;						// Qty - 40
	uint8_t gas2_ppm_coeff_x0 = FLOAT_VALUE;
	uint8_t gas2_ppm_coeff_x1 = FLOAT_VALUE;
	uint8_t gas2_ppm_coeff_x2 = FLOAT_VALUE;
	uint8_t gas2_ppm_coeff_x3 = FLOAT_VALUE;						// 4070
	uint8_t gas2_ppm_coeff_x4 = FLOAT_VALUE;
	uint8_t gas2_ppm_coeff_x5 = FLOAT_VALUE;
	uint8_t gas2_ppm_coeff_x6 = FLOAT_VALUE;
	uint8_t gas2_ppm_coeff_x7 = FLOAT_VALUE;
	uint8_t lamp_data_time_0 = UNSIGNED_INTEGER;				// 4080
	uint8_t lamp_data_volt_0 = UNSIGNED_INTEGER;				// Qty - 50
	uint8_t lamp_data_time_1 = UNSIGNED_INTEGER;
	uint8_t lamp_data_volt_1 = UNSIGNED_INTEGER;
	uint8_t lamp_data_time_2 = UNSIGNED_INTEGER;
	uint8_t lamp_data_volt_2 = UNSIGNED_INTEGER;
	uint8_t abc_correlation_factor = FLOAT_VALUE;
	uint8_t abc_sample_rate = UNSIGNED_INTEGER;
	uint8_t abc_eval_count = UNSIGNED_INTEGER;
	uint8_t abc_sample_count = UNSIGNED_INTEGER;				// 4090
	uint8_t dsp_alpha_coeff = FLOAT_VALUE;
	uint8_t dsp_adapt_coeff = FLOAT_VALUE;							// Qty - 60
	uint8_t dsp_adapt_bound = FLOAT_VALUE;
	uint8_t gas_ppm_upper_bound_1 = UNSIGNED_INTEGER;
	uint8_t gas_ppm_upper_bound_2 = UNSIGNED_INTEGER;
	uint8_t altitude = FLOAT_VALUE;											// 4100
	uint8_t cal_altitude = FLOAT_VALUE;
	uint8_t pressure = FLOAT_VALUE;
	uint8_t warm_up_time = UNSIGNED_INTEGER;
	uint8_t dac_ctrl = UNSIGNED_INTEGER;
	uint8_t first_sample_cf = FLOAT_VALUE;
	uint8_t avg_ctrl = UNSIGNED_INTEGER;								// Qty - 70
} HRegTypes;
#define QTY_HREGTYPES		70

typedef struct
{
	uint16_t update_count = 0;
	uint16_t firmware_revision = 132;
	uint16_t status = 0;
	uint16_t gas_ppm;
	uint16_t raw_sig;
	float tcor_sig;
	float norm_sig;
	float norm_sig_avg;
	float norm_sig_avg_alpha;
	uint16_t nadir;
	uint16_t zenith;
	uint16_t temp_signal;
	uint16_t v_lamp_plus;
	uint16_t v_lamp_minus;
	uint16_t vin_adc;
	uint16_t co2_bound_value;
	uint16_t foobar1;
	uint16_t foobar2;
	uint16_t rt_ops_flag;
	uint32_t up_time;
} InputRegs;

typedef struct
{
	uint8_t update_count = UNSIGNED_INTEGER;						// 5000
	uint8_t firmware_revision = UNSIGNED_INTEGER;
	uint8_t status = UNSIGNED_INTEGER;
	uint8_t gas_ppm = UNSIGNED_INTEGER;
	uint8_t raw_sig = UNSIGNED_INTEGER;
	uint8_t tcor_sig = FLOAT_VALUE;
	uint8_t norm_sig = FLOAT_VALUE;
	uint8_t norm_sig_avg = FLOAT_VALUE;
	uint8_t norm_sig_avg_alpha = FLOAT_VALUE;
	uint8_t nadir = UNSIGNED_INTEGER;
	uint8_t zenith = UNSIGNED_INTEGER;
	uint8_t temp_signal = UNSIGNED_INTEGER;
	uint8_t v_lamp_plus = UNSIGNED_INTEGER;
	uint8_t v_lamp_minus = UNSIGNED_INTEGER;
	uint8_t vin_adc = UNSIGNED_INTEGER;
	uint8_t co2_bound_value = UNSIGNED_INTEGER;
	uint8_t foobar1 = UNSIGNED_INTEGER;
	uint8_t foobar2 = UNSIGNED_INTEGER;
	uint8_t rt_ops_flag = UNSIGNED_INTEGER;
	uint8_t up_time = UNSIGNED_LONG;
} IRegTypes;

extern VolFlags uiIntFlags;
extern Flags uiFlags;
extern uint32_t ulTicks;
extern uint8_t ucTick_10mS_acc;
extern uint16_t uiTick_1S_acc;
extern uint16_t uiTick_1S_Ctr;
extern HoldRegs sHRegs;
extern HoldRegs* pHRegs;
extern InputRegs sIRegs;
extern InputRegs *pIRegs;

#ifdef DEMO_BOARD
extern uint16_t uiLED_acc;
extern uint16_t uiLED_Interval;
#endif

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
uint16_t Calc_crc(Buffer);

#ifdef DEMO_BOARD
/* USER CODE BEGIN EFP */
void UserButton_Callback(void);
#endif

//void USART_CharReception_Callback(void);
//void Error_Callback(void);
/* USER CODE END EFP */

#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
