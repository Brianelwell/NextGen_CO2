/* -----------------------------------------------------------------------------
 *            file: inputRegFuncs.cpp
 *        synopsis: These are functions that handle a Modbus Read Input
 *                  Registers request when recognized by the program. All of
 *                  these functions populate the process_request_table[] in the
 *                  module readInputRegisters.cpp.
 *          author: David S. Cooper (DSC)
 *       copyright: Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *                  All rights reserved.
 *          Date: 2017-12-11 11:59:00 -0800 (Mon, 11 Dec 2017) $
 *      Revision: 190 $
 *
 *  Revision history
 *
 *  Date          Name            Description
 *  ============= =============== ==============================================
 *  10/17/2013    DSC             Created
 */
#include  <cstdint>
#include  "inputRegsFuncs.h"
#include  "T67xx.h"         /* offsets required for input register definitions */
#include  "readInputRegisters.h"  /* required for the extern "C" declaration */
#include  "kernel.h"        /* task_manager */
#include  "tasks.h"         /* task definitions and declarations */
#include  "modbus.h"        /* various Modbus definitions */
#include  "queue.h"         /* must be included before globals.h */
#include  "status.h"        /* must be included before globals.h */
#include  "eeprom.h"        /* must be included before globals.h */
#include  "globals.h"       /* queues */
#include  "gas_daq.h"       /* raw_sig, nadir, zenith, etc. */
#include  "dsp.h"           /* gas_ppm, tcor_sig, norm_sig, etc. */
#include  "rtops.h"         /* rt_ops_flag */

/* ----- data types --------------------------------------------------------- */
/* ----- definitions -------------------------------------------------------- */
/* ----- global declarations ------------------------------------------------ */
/* ----- local declarations ------------------------------------------------- */
/* ----- forward declarations ----------------------------------------------- */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the current device status word.
 *     param [in] : address - the MODBUS Input Register address for STATUS.
 *    param [out] : data - destination address for the current device status.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 */
uint8_t get_device_status(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    *data = status.get_device_status();
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_device_status */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most current processed gas value.
 *     param [in] : address - parameter is not used.
 *    param [out] : data - destination address for the gas_ppm value.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 */
uint8_t get_gas_ppm(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    *data = gas_ppm;
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_gas_ppm */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most current raw gas measurement signal value.
 *     param [in] : address - parameter is not used.
 *    param [out] : data - destination address for the current raw signal value.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 */
uint8_t get_raw_sig(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    *data = (uint16_t)raw_sig;
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_raw_sig */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most recent tcor calculation.
 *     param [in] : address - the MODBUS Input Register address for tcor_sig (hi
 *                  or low).
 *    param [out] : data - destination address for the tcor value.
 *         return : NO_ERROR if successful, else SLAVE_DEVICE_FAILURE is input
 *                  parameter data is void or ILLEGAL_DATA_ADDRESS if input
 *                  parameter address is not the proper address.
 *           note : tcor_sig is stored as a float type in memory. This function
 *                  returns the upper or lower parts converted into their
 *                  respective uint16_t parts.
 */
uint8_t get_tcor_sig(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;
  uint32_t tmpvar;

  if (0 != data)
  {
    tmpvar = *((uint32_t*)((void*)&tcor_sig));
    switch(address)
    {
      case TCOR_SIG_HI :
      {
        uint32_t t1 = (tmpvar >> 16);
        t1 &= 0x0000FFFF;
        *data = (uint16_t)(t1);
      }

      break;
      case TCOR_SIG_LO :
      {
        uint32_t t1 = tmpvar;
        t1 &= 0x0000FFFF;
        *data = (uint16_t)(t1);
      }
      break;

      default : {retval = ILLEGAL_DATA_ADDRESS;} break;
    }
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_tcor_sig */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most recent normalized signal calculation.
 *     param [in] : address - the MODBUS Input Register address for norm_sig (hi
 *                  or low).
 *    param [out] : data - destination address for the norm_sig value.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 *           note : norm_sig is stored as a float type in memory. This function
 *                  returns the upper or lower parts converted into their
 *                  respective uint16_t parts.
 */
uint8_t get_norm_sig(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;
  uint32_t tmpvar;

  if (0 != data)
  {
    tmpvar = *((uint32_t*)((void*)&norm_sig));
    switch(address)
    {
      case NORM_SIG_HI :
      {
        uint32_t t1 = (tmpvar >> 16);
        t1 &= 0x0000FFFF;
        *data = (uint16_t)(t1);
      }

      break;
      case NORM_SIG_LO :
      {
        uint32_t t1 = tmpvar;
        t1 &= 0x0000FFFF;
        *data = (uint16_t)(t1);
      }
      break;

      default : {retval = ILLEGAL_DATA_ADDRESS;} break;
    }
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_norm_sig */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most recent averaged normalized signal
 *                  calculation.
 *     param [in] : address - the MODBUS Input Register address for norm_sig_avg
 *                  (hi or low).
 *    param [out] : data - destination address for the norm_sig value.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 *           note : norm_sig_avg is stored as a float type in memory. This
 *                  function returns the upper or lower parts converted into
 *                  their respective uint16_t parts.
 */
uint8_t get_norm_sig_avg(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;
  uint32_t tmpvar;

  if (0 != data)
  {
    tmpvar = *((uint32_t*)((void*)&norm_sig_avg));
    switch(address)
    {
      case NORM_SIG_AVG_HI :
      {
        uint32_t t1 = (tmpvar >> 16);
        t1 &= 0x0000FFFF;
        *data = (uint16_t)(t1);
      }

      break;
      case NORM_SIG_AVG_LO :
      {
        uint32_t t1 = tmpvar;
        t1 &= 0x0000FFFF;
        *data = (uint16_t)(t1);
      }
      break;

      default : {retval = ILLEGAL_DATA_ADDRESS;} break;
    }
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_norm_sig_avg */

/* -----------------------------------------------------------------------------
 *       synopsis : This parameter is used in the power filter and the tap
 *                  weight.
 *     param [in] : address - the MODBUS Input Register address for
 *                  norm_sig_avg_alpha (hi or low).
 *    param [out] : data - destination address for the norm_sig_avg_alpha value.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 *           note : norm_sig_avg_alpha is stored as a float type in memory. This
 *                  function returns the upper or lower parts converted into
 *                  their respective uint16_t parts.
 */
uint8_t get_norm_sig_avg_alpha(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;
  uint32_t tmpvar;

  if (0 != data)
  {
    tmpvar = *((uint32_t*)((void*)&norm_sig_avg_alpha));
    switch(address)
    {
      case NORM_SIG_AVG_ALHPA_HI :
      {
        uint32_t t1 = (tmpvar >> 16);
        t1 &= 0x0000FFFF;
        *data = (uint16_t)(t1);
      }

      break;
      case NORM_SIG_AVG_ALPHA_LO :
      {
        uint32_t t1 = tmpvar;
        t1 &= 0x0000FFFF;
        *data = (uint16_t)(t1);
      }
      break;

      default : {retval = ILLEGAL_DATA_ADDRESS;} break;
    }
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_norm_sig_avg_alpha */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most current nadir measurement.
 *     param [in] : address - parameter is not used.
 *    param [out] : data - destination address for the current nadir measurement.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 */
uint8_t get_nadir(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    *data = nadir;
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_nadir */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most current zenith measurement.
 *     param [in] : address - parameter is not used.
 *    param [out] : data - destination address for the current zenith
 *                  measurement.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 */
uint8_t get_zenith(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    *data = zenith;
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_zenith */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most current TEMP_SIGNAL measurement from the
 *                  ASIC as measured in the gas_daq_task() function.
 *     param [in] : address - the MODBUS Input Register address for TEMP_SIGNAL.
 *    param [out] : data - destination address for ASIC TEMP_SIGNAL measurement.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 */
uint8_t get_detector_temperature(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    *data = detector_temperature;
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_detector_temperature */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most current ADC_CHANNEL_V_LAMP_PLUS
 *                  measurement from the ASIC as measured in the gas_daq_task()
 *                  function.
 *     param [in] : address - parameter is not used.
 *    param [out] : data - destination address for measurement.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 *            see : gas_daq_task() in gas_daq.cpp
 *           note : This measurement is one sample old.
 */
uint8_t get_lamp_plus_volts(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    *data = lamp_plus_volts;
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_lamp_plus_volts */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most current ADC_CHANNEL_V_LAMP_MINUS
 *                  measurement from the ASIC as measured in the gas_daq_task()
 *                  function.
 *     param [in] : address - parameter is not used.
 *    param [out] : data - destination address for measurement.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 *            see : gas_daq_task() in gas_daq.cpp
 *           note : This measurement is one sample old.
 */
uint8_t get_lamp_minus_volts(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    *data = lamp_minus_volts;
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_lamp_minus_volts */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the most current VIN_ADC measurement from the
 *                  ASIC as measured in the gas_daq_task() function.
 *     param [in] : address - parameter is not used.
 *    param [out] : data - destination address for VIN_ADC measurement.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 */
uint8_t get_input_volts(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    *data = input_volts;
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_input_volts */

/* -----------------------------------------------------------------------------
 *       synopsis : Return the current state of the rt_ops_flag.
 *                  This indicates the communications and PWM settings in use.
 *     param [in] : address - parameter is not used.
 *    param [out] : data - destination address for RUN_TIME accumulation.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 */
uint8_t get_rt_ops_flag(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    *data = rt_ops_flag;
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_rt_ops_flag */

/* -----------------------------------------------------------------------------
 *       synopsis : Return the number of seconds the device has run
 *                  continuously.
 *     param [in] : address - parameter is not used.
 *    param [out] : data - destination address for RUN_TIME accumulation.
 *         return : NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter
 *                  incorrect.
 */
uint8_t get_up_time(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;
  uint32_t tmpvar;

  if (0 != data)
  {
    tmpvar = *((uint32_t*)((void*)&run_time_in_seconds));
    switch(address)
    {
      case UP_TIME_HI :
      {
        uint32_t t1 = (tmpvar >> 16);
        t1 &= 0x0000FFFF;
        *data = (uint16_t)(t1);
      }

      break;
      case UP_TIME_LO :
      {
        uint32_t t1 = tmpvar;
        t1 &= 0x0000FFFF;
        *data = (uint16_t)(t1);
      }
      break;

      default :
      {
        retval = ILLEGAL_DATA_ADDRESS;
      } break;
    }
  }

  return retval;
} /* get_run_time */

