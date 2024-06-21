/* -----------------------------------------------------------------------------
 *            file: gas_daq.cpp
 *        synopsis: The task and support functions to measure the CO2
 *          author: David S. Cooper (DSC)
 *       copyright: Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *                  All rights reserved.
 *           $Date: 2019-01-11 16:16:40 -0500 (Fri, 11 Jan 2019) $
 *       $Revision: 200 $
 *
 *  Revision history
 *
 *  Date          Name            Description
 *  ============= =============== ==============================================
 *  7/1/2013      DSC             Created
 */
	 
#include "main.h"


//#include  <stdint.h>
	//#include  "SYSTK.h"           /* get_system_tick */
	//#include  "kernel.h"          /* task_manager */
	//include  "tasks.h"           /* task definitions and declarations */
//#include  "gas_daq.h"         /* prototypes and definitions */
	//#include  "hardware.h"        /* enable_*_power */
	//#include  "queue.h"           /* must be included before globals.h */
	//#include  "status.h"          /* must be included before globals.h */
//#include  "eeprom.h"          /* must be included before globals.h */
	//#include  "globals.h"         /* status */
	//#include  "modbus.h"          /* WRITE_SINGLE_COIL_ON/OFF */
//#include  "dsp.h"             /* dsp_calculate_gas_ppm */
	//#include  "wdog.h"            /* wdog_clear() */

/* ----- data types --------------------------------------------------------- */
/* ----- definitions -------------------------------------------------------- */
static const uint16_t NUMBER_OF_SAMPLES = 16U;
#define MEASURE_NADIR   (RUN+100)
#define LAMP_STEP       (RUN+150)
#define MEASURE_ZENITH  (RUN+200)

/* ----- local variables ---------------------------------------------------- */
static uint16_t temp_nadir;
static uint16_t temp_zenith;
static uint16_t temp_detector_temperature;
static uint16_t temp_input_volts;
//static uint16_t temp_lamp_plus_volts;
//static uint16_t temp_lamp_minus_volts;
static uint16_t lamp_ctrl_index;

static uint16_t counter;
static uint32_t avg_detector_temperature;
static uint32_t avg_nadir;
static uint32_t avg_zenith;
//static uint8_t block_flag;

/* ----- global declarations ------------------------------------------------ */
uint16_t  nadir;
uint16_t  zenith;
uint16_t  detector_temperature;
uint16_t  input_volts;
uint16_t  lamp_plus_volts;
uint16_t  lamp_minus_volts;
uint16_t  dsp_adapt_counter;

uint8_t		ucGas_DAQ_Step = 0;

/* ----- forward declarations ----------------------------------------------- */


/* -----------------------------------------------------------------------------
 *       synopsis : Periodic task that does the CO2 measurement. The entire
 *                  measurement cycle (lamp control, ADC measurements, etc.) are
 *                  handled in this function.
 *     param [in] : tcb - pointer to the task control block (Tcb) for this task.
 */
//void gas_daq_task(pTcb tcb)
void gas_daq_task(void)
{
//  uint16_t data;
	uint16_t uiResult, uiAccum_CO2, uiAccum_Temp;
	uint16_t i;

	switch(ucGas_DAQ_Step)
	{
	case 0:
//		uiCO2_Measure_Tmr ++;
		if(uiCO2_Measure_Tmr< sHRegs.sample_time)
		{
			break;
		}
		ucGas_DAQ_Step ++;
		uiCO2_Measure_Tmr = 0;
		uiFlags.bCO2_MeasureInProcess = true;
		// break;													//  Intentional Flow Through
		
	case 1:
		uiAccum_CO2 = 0;
		uiAccum_Temp = 0;
		for(i = 0; i < NUMBER_OF_SAMPLES; i++)
		{
			if(ADC1->ISR & ADC_ISR_ADRDY)			//  Make Sure A/D Ready to Sample
			{
				if((uiResult = ADC_Measure(ADC_CHANNEL_GAS_SIGNAL))	> 0)			//  Start CO2 Measurement
				{
//					sIRegs.raw_sig += uiResult_CO2;
					uiAccum_CO2 += uiResult;
				}
			}
			if(ADC1->ISR & ADC_ISR_ADRDY)			//  Make Sure A/D Ready to Sample
			{
				if((uiResult = ADC_Measure(ADC_CHANNEL_TEMP_SIGNAL)) > 0)			//  Start CO2 Measurement
				{
//					sIRegs.temp_signal = uiResult;
					uiAccum_Temp += uiResult;
				}
			}
		}
		sIRegs.raw_sig = uiAccum_CO2 / NUMBER_OF_SAMPLES;
		sIRegs.temp_signal = uiAccum_Temp / NUMBER_OF_SAMPLES;
		ucGas_DAQ_Step ++;
		break;

	default:
		ucGas_DAQ_Step = 0;
	}

//  switch (tcb->state)
//  {
    /* not taking measurements */
//    case BLOCKED :
    {
      /* do nothing */
      
    }
//    break;

    /* Power the amplifier and detector on */
//    case RUN :
    {
      /* let the delay schedule the task */
//      clear_task_event_flag(GAS_DAQ_EVENT);
    
      /* For Sample on Demand */
/*
      if (block->first_sample_cf.re == 0)
      {
        if (counter1 < block->sample_time/100)
        {
          break;
        }
        else
        {
          counter1 = 0;
        }
*/
        /*      This condition only used to put sensor in sleep for sample on  
         *      damand while we power up sensor for the first time. 
         */ 
/*			
        if(block_flag == 1)
        {
          tcb = get_task(MEAS_PERIOD_TASK);
          tcb->state = BLOCKED;
          tcb->delay = TICKS_PER_SECOND;
          block_flag = 0;
          flag = 1;
          counter1 = block->sample_time;
          norm_sig_avg_alpha = block->dsp_alpha_coeff.re;
          dsp_adapt_counter = 0;
          data = WRITE_SINGLE_COIL_ON;
          break;
        }
      }
*/
      /* turn on the power to the amp and detector */
//      data = WRITE_SINGLE_COIL_ON;
//      (void)lamp_driver_power(0U, &data);
//      (void)detector_power(0U, &data);
      
//      tcb->delay = block->nadir_time;
//      tcb->state = MEASURE_NADIR;

      /* make sure lamp is off */
//      data = 0U;
//      (void)set_dac_lamp_voltage_control(0U, &data);

      /* initialize our measurement variables */
      temp_nadir = 0U;
      temp_zenith = 0U;
      temp_detector_temperature = 0U;
      temp_input_volts = 0U;

      /* initialize the index into the lamp control array */
      lamp_ctrl_index = 0U;
      counter = 0U;
      avg_detector_temperature = 0U;
      avg_nadir = 0U;
      avg_zenith = 0U;
    }
//    break;

    /* measure the temperature, measure the nadir and turn on the lamp */
//    case MEASURE_NADIR :
    {
      /* measure the detector temperature */
 //     (void)get_temp_signal(0U, &temp_detector_temperature);
      avg_detector_temperature += temp_detector_temperature;

      /* measure nadir */
//      (void)get_gas_signal(0U, &temp_nadir);
      avg_nadir += temp_nadir;

      if (++counter >= NUMBER_OF_SAMPLES)
      {
//        tcb->state = MEASURE_NADIR + 1;
      }
      else
      {
//        tcb->delay = 1U;
      }
    }
//    break;

//    case MEASURE_NADIR + 1U :
    {
      temp_detector_temperature =
        (uint16_t)(avg_detector_temperature / NUMBER_OF_SAMPLES);

      temp_nadir = (uint16_t)(avg_nadir / NUMBER_OF_SAMPLES);

      /* turn on the lamp */
//      data = block->lamp_data[lamp_ctrl_index].volt;
//      (void)set_dac_lamp_voltage_control(0U, &data);

      /* delay before we measure the zenith */
//      tcb->delay = block->lamp_data[lamp_ctrl_index].time;
//      tcb->state = LAMP_STEP;
    }
//    break;

    /* stairstep the lamp on (i.e., lamp control) */
//    case LAMP_STEP :
    {
      lamp_ctrl_index++;
      if (lamp_ctrl_index < LAMP_ON_STEPS)
      {
//        data = block->lamp_data[lamp_ctrl_index].volt;
//        (void)set_dac_lamp_voltage_control(0U, &data);
//        tcb->delay = block->lamp_data[lamp_ctrl_index].time;
//        tcb->state = LAMP_STEP;
      }
      else
      {
        counter = 0U;
        avg_zenith = 0U;
//        tcb->delay = block->zenith_time;
//        tcb->state = MEASURE_ZENITH;
      }
    }
//    break;

    /* measure the zenith, measure the input voltage */
//    case MEASURE_ZENITH :
    {
      /* measure zenith */
//      (void)get_gas_signal(0U, &temp_zenith);
      avg_zenith += temp_zenith;

      if (++counter >= NUMBER_OF_SAMPLES)
      {
//        tcb->state = MEASURE_ZENITH + 1;
      }
      else
      {
//        tcb->delay = 1U;
      }
    }
//    break;

//    case MEASURE_ZENITH + 1 :
    {
      temp_zenith = (uint16_t)(avg_zenith / NUMBER_OF_SAMPLES);

      /* measure the input voltage */
		
//      (void)get_vin_adc(0U, &temp_input_volts);

      /* get the lamp voltage */
/*      (void)get_v_lamp_plus(0U, &temp_lamp_plus_volts);
      (void)get_v_lamp_minus(0U, &temp_lamp_minus_volts);

      tcb->delay = 1U; // can't be 0 or it won't run!
      tcb->state = FINISH;
    }
    break;
*/
//    case FINISH :
//    {
      /* turn off the power to the amp and detector */
/*      data = WRITE_SINGLE_COIL_OFF;
      (void)lamp_driver_power(0U, &data);
      (void)detector_power(0U, &data);

      data = 0U;
      (void)set_dac_lamp_voltage_control(0U, &data);

      tcb->delay = 0U; // has to be 0 or it won't stop!
      tcb->state = RUN;
*/
      /* cache the current measurements so readings are somewhat current */
/*
      nadir = temp_nadir;
      zenith = temp_zenith;
      detector_temperature = temp_detector_temperature;
      input_volts = temp_input_volts;
      lamp_plus_volts = temp_lamp_plus_volts;
      lamp_minus_volts = temp_lamp_minus_volts;
*/
//      if ((STATUS_WARMUP_MODE & status.get_device_status()) != STATUS_WARMUP_MODE)
//      {
        /* do the gas ppm calculation - the result is left in the global
         * variable 'gas_ppm'. see dsp.cpp */
//        dsp_calculate_gas_ppm();
//      }
//      else
//      {  
        /* initialize variable for calculating the gas ppm the first time
         * through */       
//        dsp_initialize_gas_ppm();
//      }
//    }
//    break;
/*
    case INIT :
    default :
    {
      tcb->id = GAS_DAQ_TASK;
      tcb->state = RUN;
      tcb->delay = 0U;
      tcb->last = 0U;
      tcb->events = GAS_DAQ_EVENT;
      if (block->avg_ctrl == SPECIAL_MEDIA_FEATURE)
      {
        gas_ppm = 550U;
      }
      else
      {
        gas_ppm = 0U;
      }
      if ( block->first_sample_cf.re == 0)
      {
        block_flag = 1;
      }
    }
    break;
*/		
  }
} /* gas_daq_task */

