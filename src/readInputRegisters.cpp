/* -----------------------------------------------------------------------------
 *            file: readInputRegisters.cpp
 *        synopsis: MODBUS function responsible for reading 1 or more Input
 *                  Registers.
 *          author: David S. Cooper (DSC)
 *       copyright: Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *                  All rights reserved.
 *           $Date: 2018-03-01 14:39:44 -0500 (Thu, 01 Mar 2018) $
 *       $Revision: 197 $
 *
 *  Revision history
 *
 *  Date          Name            Description
 *  ============= =============== ==============================================
 *  5/23/2013     DSC             Created
 */
#include  <stdint.h>
#include  "modbus.h"          /* various Modbus definitions */
#include  "T67xx.h"       /* offsets required for input register definitions */
#include  "readInputRegisters.h"  /* required for the extern "C" declaration */
#include  "queue.h"           /* must be included before globals.h */
#include  "status.h"          /* must be included before globals.h */
#include  "eeprom.h"          /* must be included before globals.h */
#include  "globals.h"         /* queues */
#include  "common.h"          /* process request ADT's, functions, etc. */
#include  "hardware.h"        /* r/w hardware functions */
#include  "inputRegsFuncs.h"  /* function definitions for the process table */

/* ----- forward declarations ----------------------------------------------- */
/* ----- local declarations ------------------------------------------------- */
static process_request_type const process_request_table[] =
{
  { (uint16_t)UPDATE_COUNT, eeprom_read, bad_write },
  { (uint16_t)FIRMWARE_REVISION, get_firmware_revision, bad_write },
  { (uint16_t)STATUS, get_device_status, bad_write },
  { (uint16_t)GAS_PPM, get_gas_ppm, bad_write },
  { (uint16_t)RAW_SIG, get_raw_sig, bad_write },
  { (uint16_t)TCOR_SIG_HI, get_tcor_sig, bad_write },
  { (uint16_t)TCOR_SIG_LO, get_tcor_sig, bad_write },
  { (uint16_t)NORM_SIG_HI, get_norm_sig, bad_write },
  { (uint16_t)NORM_SIG_LO, get_norm_sig, bad_write },
  { (uint16_t)NORM_SIG_AVG_HI, get_norm_sig_avg, bad_write },
  { (uint16_t)NORM_SIG_AVG_LO, get_norm_sig_avg, bad_write },
  { (uint16_t)NORM_SIG_AVG_ALHPA_HI, get_norm_sig_avg_alpha, bad_write },
  { (uint16_t)NORM_SIG_AVG_ALPHA_LO, get_norm_sig_avg_alpha, bad_write },
  { (uint16_t)NADIR, get_nadir, bad_write },
  { (uint16_t)ZENITH, get_zenith, bad_write },
  { (uint16_t)TEMP_SIGNAL, get_detector_temperature, bad_write },
  { (uint16_t)V_LAMP_PLUS, get_lamp_plus_volts, bad_write },
  { (uint16_t)V_LAMP_MINUS, get_lamp_minus_volts, bad_write },
  { (uint16_t)VIN_ADC, get_input_volts, bad_write },
  { (uint16_t)CO2_BOUND_VALUE, get_co2_bound_value, bad_write },
  { (uint16_t)FOOBAR1, bad_read, bad_write },
  { (uint16_t)FOOBAR2, bad_read, bad_write },
  { (uint16_t)RT_OPS_FLAG, get_rt_ops_flag, bad_write },
  { (uint16_t)UP_TIME_HI, get_up_time, bad_write },
  { (uint16_t)UP_TIME_LO, get_up_time, bad_write },
};

static const uint16_t REGS = sizeof(process_request_table) /
  sizeof(process_request_type);

/*lint -e{1784} Symbol 'read_input_registers' previously declared as "C" */
/* -----------------------------------------------------------------------------
 *       synopsis : Implements the Read Input Registers function 4 (0x04).
 *         return : NO_ERROR (0) or one of the MODBUS Exception Codes.
 *            see : MODBUS Application Protocol Specification V1.1b
 */
uint8_t read_input_registers(void)
{
  uint16_t starting_address;
  uint16_t quantity_of_registers;
  uint8_t  retval = NO_ERROR;
  uint16_t data;

  /* prime the pump */
  if (!(output_queue.enqueue(READ_INPUT_REGISTERS) &&
        get_address_and_registers(&starting_address, &quantity_of_registers)))
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  /* test Quantity of Registers */
  else if (!((MIN_QUANTITY_OF_REGISTERS <= quantity_of_registers) &&
             (MAX_QUANTITY_OF_REGISTERS >= quantity_of_registers)))
  {
    retval = ILLEGAL_DATA_VALUE;
  }

  /* Starting Address + Quantity of Registers == OK AND ... */
  else if ((uint16_t)END_INPUT_REGISTERS <
           (starting_address + quantity_of_registers))
  {
    retval = ILLEGAL_DATA_ADDRESS;
  }

  /* ... Start Address == OK */
  else if (!(((uint16_t)INPUT_REGISTERS_OFFSET <= starting_address) &&
             ((uint16_t)END_INPUT_REGISTERS > starting_address)))
  {
    retval = ILLEGAL_DATA_ADDRESS;
  }

  /* get the register data */
  else
  {
    uint16_t ending_address = starting_address + quantity_of_registers - 1;

    /* assume we're going to get all the data and put the byte count in the
     * response */
    uint8_t reg = (uint8_t)(quantity_of_registers * 2);

    if (output_queue.enqueue((uint8_t)((reg) & 0x00FF)))
    {
      for (uint16_t address=starting_address;
           (retval==NO_ERROR && address<=ending_address); address++)
      {
        int16_t index = binary_search(process_request_table, REGS, address);

        if ((index != -1) && (REGS > index))
        {
          retval = process_request_table[index].read_request(
              process_request_table[index].key,
              &data);

          if (NO_ERROR == retval)
          {
            bool success = output_queue.enqueue((uint8_t)((data>>8) & 0x00FF));

            if (!(success && output_queue.enqueue((uint8_t)((data) & 0x00FF))))
            {
              retval = SERVER_DEVICE_FAILURE;
            }
          }
        }
        else
        {
          retval = SERVER_DEVICE_FAILURE;
        }
      }
    }
  }

  return retval;
} /* read_input_registers */

