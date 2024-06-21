/* -----------------------------------------------------------------------------
 *            file: holdingRegs.cpp
 *        synopsis: MODBUS functions responsible for reading and writing holding
 *                  registers.
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
 *  10/21/2013    DSC             Created
 */
#include  <stdint.h>
#include  "T68xx.h"     /* varuious offsets required for register definitions */
//#include  "modbus.h"    /* various Modbus definitions */
#include  "holdingRegs.h"
//#include  "queue.h"     /* must be included before globals.h */
//#include  "status.h"    /* must be included before globals.h */
#include  "eeprom.h"    /* must be included before globals.h */
//#include  "globals.h"   /* queues */
//#include  "common.h"    /* process request ADT's, functions, etc. */
//#include  "hardware.h"  /* r/w hardware functions */
#include  "registerFunctions.h" /* eeprom r/w functions */

/* ----- data types --------------------------------------------------------- */
/* ----- definitions -------------------------------------------------------- */
/* ----- global declarations ------------------------------------------------ */
/* ----- local declarations ------------------------------------------------- */

/* IMPORTANT - This table must be sorted in ascending order, based on the key
 * field whose definitions are in the Holding_Registers enumeration (see
 * holdingRegs.h). This ensures that the Binary Search routine works. If the
 * table below is not sorted then the functions below will likely return a
 * SERVER_DEVICE_FAILURE error code. */
/*
static process_request_type const process_request_table[] =
{
  { (uint16_t)MODEL_NUMBER, eeprom_read, set_model_number },
  { (uint16_t)MFG_DATA, eeprom_read, set_model_number },
  { (uint16_t)SERIAL_NUMBER_HI, eeprom_read, set_serial_number },
  { (uint16_t)SERIAL_NUMBER_LO, eeprom_read, set_serial_number },
  { (uint16_t)OPS_FLAG, eeprom_read, eeprom_write },
  { (uint16_t)SLAVE_ADDRESS, get_slave_address, set_slave_address },
  { (uint16_t)BAUD_RATE, eeprom_read, set_baud_rate },
  { (uint16_t)PARITY, eeprom_read, set_parity },
  { (uint16_t)SAMPLE_TIME, eeprom_read, eeprom_write },
  { (uint16_t)NADIR_TIME, eeprom_read, eeprom_write },
  { (uint16_t)ZENITH_TIME, eeprom_read, eeprom_write },
  { (uint16_t)CAL_SAMPLES, eeprom_read, eeprom_write },
  { (uint16_t)TEMP_DIVISOR_HI, eeprom_read, eeprom_write },
  { (uint16_t)TEMP_DIVISOR_LO, eeprom_read, eeprom_write },
  { (uint16_t)CO2_BOUND_LIMIT_HI, eeprom_read, eeprom_write },
  { (uint16_t)CO2_BOUND_LIMIT_LO, eeprom_read, eeprom_write },
  { (uint16_t)ZERO_CAL_TARGET_PPM, eeprom_read, eeprom_write },
  { (uint16_t)ZERO_CAL_RESULT_HI, eeprom_read, eeprom_write },
  { (uint16_t)ZERO_CAL_RESULT_LO, eeprom_read, eeprom_write },
  { (uint16_t)SPAN1_CAL_TARGET_PPM, eeprom_read, eeprom_write },
  { (uint16_t)SPAN1_CAL_RESULT_HI, eeprom_read, eeprom_write },
  { (uint16_t)SPAN1_CAL_RESULT_LO, eeprom_read, eeprom_write },
  { (uint16_t)SPAN1_ZERO_RATIO_HI, eeprom_read, eeprom_write },
  { (uint16_t)SPAN1_ZERO_RATIO_LO, eeprom_read, eeprom_write },
  { (uint16_t)SPAN2_CAL_TARGET_PPM, eeprom_read, eeprom_write },
  { (uint16_t)SPAN2_CAL_RESULT_HI, eeprom_read, eeprom_write },
  { (uint16_t)SPAN2_CAL_RESULT_LO, eeprom_read, eeprom_write },
  { (uint16_t)SPAN2_ZERO_RATIO_HI, eeprom_read, eeprom_write },
  { (uint16_t)SPAN2_ZERO_RATIO_LO, eeprom_read, eeprom_write },
  { (uint16_t)SNGPT_CAL_TARGET_PPM, eeprom_read, eeprom_write },
  { (uint16_t)TCOR_FACTOR_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR_FACTOR_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR1_COEFF_X0_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR1_COEFF_X0_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR1_COEFF_X1_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR1_COEFF_X1_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR1_COEFF_X2_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR1_COEFF_X2_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR1_COEFF_X3_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR1_COEFF_X3_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR2_COEFF_X0_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR2_COEFF_X0_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR2_COEFF_X1_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR2_COEFF_X1_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR2_COEFF_X2_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR2_COEFF_X2_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR2_COEFF_X3_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR2_COEFF_X3_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR3_COEFF_X0_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR3_COEFF_X0_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR3_COEFF_X1_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR3_COEFF_X1_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR3_COEFF_X2_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR3_COEFF_X2_LO, eeprom_read, eeprom_write },
  { (uint16_t)TCOR3_COEFF_X3_HI, eeprom_read, eeprom_write },
  { (uint16_t)TCOR3_COEFF_X3_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS1_PPM_COEFF_X0_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS1_PPM_COEFF_X0_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS1_PPM_COEFF_X1_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS1_PPM_COEFF_X1_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS1_PPM_COEFF_X2_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS1_PPM_COEFF_X2_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS1_PPM_COEFF_X3_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS1_PPM_COEFF_X3_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X0_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X0_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X1_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X1_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X2_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X2_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X3_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X3_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X4_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X4_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X5_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X5_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X6_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X6_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X7_HI, eeprom_read, eeprom_write },
  { (uint16_t)GAS2_PPM_COEFF_X7_LO, eeprom_read, eeprom_write },
  { (uint16_t)LAMP_DATA_TIME_0, eeprom_read, eeprom_write },
  { (uint16_t)LAMP_DATA_VOLT_0, eeprom_read, set_dac_value_to_eeprom },
  { (uint16_t)LAMP_DATA_TIME_1, eeprom_read, eeprom_write },
  { (uint16_t)LAMP_DATA_VOLT_1, eeprom_read, set_dac_value_to_eeprom },
  { (uint16_t)LAMP_DATA_TIME_2, eeprom_read, eeprom_write },
  { (uint16_t)LAMP_DATA_VOLT_2, eeprom_read, set_dac_value_to_eeprom },
  { (uint16_t)ABC_CORRECTION_FACTOR_HI, eeprom_read, eeprom_write },
  { (uint16_t)ABC_CORRECTION_FACTOR_LO, eeprom_read, eeprom_write },
  { (uint16_t)ABC_SAMPLE_RATE, eeprom_read, eeprom_write },
  { (uint16_t)ABC_EVAL_COUNT, eeprom_read, eeprom_write },
  { (uint16_t)ABC_SAMPLE_COUNT, eeprom_read, set_abc_sample_count },
  { (uint16_t)DSP_ALPHA_COEFF_HI, eeprom_read, eeprom_write },
  { (uint16_t)DSP_ALPHA_COEFF_LO, eeprom_read, eeprom_write },
  { (uint16_t)DSP_ADAPT_COEFF_HI, eeprom_read, eeprom_write },
  { (uint16_t)DSP_ADAPT_COEFF_LO, eeprom_read, eeprom_write },
  { (uint16_t)DSP_ADAPT_BOUND_HI, eeprom_read, eeprom_write },
  { (uint16_t)DSP_ADAPT_BOUND_LO, eeprom_read, eeprom_write },
  { (uint16_t)GAS_PPM_UPPER_BOUND_1, eeprom_read, eeprom_write },
  { (uint16_t)GAS_PPM_UPPER_BOUND_2, eeprom_read, eeprom_write },
  { (uint16_t)ALTITUDE_HI, eeprom_read, eeprom_write },
  { (uint16_t)ALTITUDE_LO, eeprom_read, eeprom_write },
  { (uint16_t)CAL_ALTITUDE_HI, eeprom_read, eeprom_write },
  { (uint16_t)CAL_ALTITUDE_LO, eeprom_read, eeprom_write },
  { (uint16_t)PRESSURE_HI, eeprom_read, eeprom_write },
  { (uint16_t)PRESSURE_LO, eeprom_read, eeprom_write },
  { (uint16_t)WARM_UP_TIME, eeprom_read, eeprom_write },
  { (uint16_t)DAC_CTRL, get_dac_lamp_voltage_control, set_dac_lamp_voltage_control },
  { (uint16_t)FIRST_SAMPLE_CF_HI, eeprom_read, eeprom_write },
  { (uint16_t)FIRST_SAMPLE_CF_LO, eeprom_read, eeprom_write },
  { (uint16_t)AVG_CTRL, eeprom_read, eeprom_write },
};

static const uint16_t REGS = sizeof(process_request_table) /
  sizeof(process_request_type);
*/

/* ----- forward declarations ----------------------------------------------- */

/* -----------------------------------------------------------------------------
 *       synopsis : MODBUS request processing - MODBUS Function 0x03 - Read
 *                  Holding Registers.
 *
 *                  Reguest:
 *                    |03| - function code (removed by parse_modbus_request())
 *                    |aa| - starting address (MSB)
 *                    |aa| - starting address (LSB)
 *                    |nn| - quantity of registers,N (MSB)
 *                    |nn| - quantity of registers,N (MSB)
 *
 *                    starting address : 0x0000 - 0xFFFF
 *                    quantity of registers : 1 - 0x7D (125)
 *
 *                  Response:
 *                    |03| - function code (enqueued here)
 *                    |nn| - byte count (2*N registers, bytes that follow)
 *                    |dd| - data
 *                    ....
 *                    |dd| - data
 *
 *
 *         return : NO_ERROR (0) if there was no error in processing the
 *                  request, else the relevant exception code (2,3 or 4) is
 *                  returned.
 *            see : MODBUS Application Protocol Specification V1.1b
 */
uint8_t read_holding_registers(void)
{
  uint16_t starting_address = 0;
  uint16_t quantity_of_registers = 0;
  uint8_t  retval = NO_ERROR;
  uint16_t data;

  /* prime the pump */
  if (!(output_queue.enqueue(READ_HOLDING_REGISTERS) &&
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

  /* Starting Address + Quantity of Registers == OK AND ...*/
  else if ((uint16_t)END_HOLDING_REGISTERS <
           (starting_address + quantity_of_registers))
  {
    retval = ILLEGAL_DATA_ADDRESS;
  }

  /* ... Start Address == OK */
  else if (!(((uint16_t)HOLDING_REGISTERS_OFFSET <= starting_address) &&
             ((uint16_t)END_HOLDING_REGISTERS > starting_address)))
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
          retval = SERVER_DEVICE_FAILURE;  /* breaks for loop */
        }
      }
    }
  }

  return retval;
} /* read_holding_registers */

/* -----------------------------------------------------------------------------
 *       synopsis : Write Single (Holding) Register, MODBUS Function Code 06
 *                  (0x06).
 *
 *                  Request:
 *                    |06| - function code (removed by parse_modbus_request())
 *                    |aa| - register address (MSB)
 *                    |aa| - register address (LSB)
 *                    |dd| - register value (MSB)
 *                    |dd| - register value (LSB)
 *
 *                    register address : 0x0000 - 0xFFFF
 *                    register value : 0x0000 - 0xFFFF
 *
 *                  Response: (request packet is returned if no error)
 *                    |06| - function code
 *                    |aa| - register address (MSB)
 *                    |aa| - register address (LSB)
 *                    |dd| - register value (MSB)
 *                    |dd| - register value (LSB)
 *
 *                  Error:
 *                    |86| - error code
 *                    |ee| - exception code (2,3 or 4)
 *
 *                  Exception Codes :
 *                    0 - No error
 *                    1 - Illegal Function
 *                    2 - Illegal data address
 *                    3 - Illegal data value
 *                    4 - Slave device failure
 *
 *            see : MODBUS Application Protocol Specification V1.1b.
 *           note : We build the response here because we have easy access to
 *                  the address and value. If thre is an error the caller
 *                  (i.e., parse_modbus_request) clears the output queue.
 */
uint8_t write_single_register(void)
{
  uint16_t register_address = 0;
  uint16_t register_value = 0;
  uint8_t  retval = NO_ERROR;

  /* prime the pump */
  if (!(output_queue.enqueue(WRITE_SINGLE_REGISTER) &&
        get_address_and_registers(&register_address, &register_value)))
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  /* Register Address == OK */
  else if (!(((uint16_t)HOLDING_REGISTERS_OFFSET <= register_address) &&
             ((uint16_t)END_HOLDING_REGISTERS > register_address)))
  {
    retval = ILLEGAL_DATA_ADDRESS;
  }

  /* build the response and set the register data */
  else
  {
    int16_t  index = 0;
    bool     result = true;

    /* response requires function code (already put there above), Register
     * Address and Register Value */
    result = put_address_and_registers(register_address, register_value);

    if (result)
    {
      index = binary_search(process_request_table, REGS, register_address);

      if ((index != -1) && (REGS > index))
      {
        /* create a copy of the current active eeprom block */
        eeprom_create_active_block_copy(true);

        /* process the request */
        retval = process_request_table[index].write_request(
            process_request_table[index].key,
            &register_value);

        /* commit to flash */
        result = eeprom_commit_changes();
      }
      else
      {
        result = false;
      }
    }

    if(false==result)
    {
      retval = SERVER_DEVICE_FAILURE;
    }
  }

  return retval;
} /* write_single_register */

/* -----------------------------------------------------------------------------
 *       synopsis : Write Multiple (Holding) Registers, MODBUS Function Code 16
 *                  (0x10). This function is used to write a block of contiguous
 *                  registers (1 to 123 registers) to the device.
 *                  The requested written values are specified in the request
 *                  data field. Data is packed as two bytes per register.
 *
 *                  The normal response returns the function code, starting
 *                  address, and quantity of registers written.
 *
 *                  Request:
 *                    |10| - function code (removed by parse_modbus_request())
 *                    |aa| - Starting Address (MSB)
 *                    |aa| - Starting Address (LSB)
 *                    |nn| - Quantity of Registers (MSB)
 *                    |nn| - Quantity of Registers (LSB)
 *                    |bc| - Byte Count (2*N)
 *                    |dd|
 *                    |dd|
 *                    .... - values (2*N Bytes)
 *                    |dd|
 *                    |dd|
 *
 *                    Starting Address = 0x0000 - 0xFFFF
 *                    Quantity of Registers = 0x0001 - 0x007B
 *                    N = Quantity of Registers
 *
 *                  Response
 *                    |10| - Function code
 *                    |aa| - Starting Address (MSB)
 *                    |aa| - Starting Address (LSB)
 *                    |nn| - Quantity of Registers (MSB)
 *                    |nn| - Quantity of Registers (LSB)
 *
 *                  Error
 *                    |90| - Error code
 *                    |dd| - Exception code (1,2,3 or 4)
 *
 *            see : MODBUS Application Protocol Specification V1.1b.
 *           note : We build the response here because we have easy access to
 *                  the address and value. If thre is an error the caller
 *                  (i.e., parse_modbus_request) clears the output queue.
 */
uint8_t write_multiple_registers(void)
{
  uint16_t starting_address = 0;
  uint16_t quantity_of_registers = 0;
  uint8_t  byte_count = 0;
  uint8_t  retval = NO_ERROR;

    /* prime the pump */
  if (!(output_queue.enqueue(WRITE_MULTIPLE_REGISTERS) &&
        get_address_and_registers(&starting_address, &quantity_of_registers)))
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  /* test Quantity of Registers 1 to 123 */
  else if (!((0x0001 <= quantity_of_registers) &&
             (0x007B >= quantity_of_registers)))
  {
    retval = ILLEGAL_DATA_VALUE;
  }

  /* get the byte count */
  else if (!input_queue.dequeue(&byte_count))
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  /* byte count == Quantity of Registers * 2 */
  else if (!((uint16_t)byte_count==(2*quantity_of_registers)))
  {
    retval = ILLEGAL_DATA_VALUE;
  }

  /* Starting Address + Quantity of Registers == OK AND ...*/
  else if ((uint16_t)END_HOLDING_REGISTERS <
           (starting_address + quantity_of_registers))
  {
    retval = ILLEGAL_DATA_ADDRESS;
  }

  /* ... Start Address == OK */
  else if (!(((uint16_t)HOLDING_REGISTERS_OFFSET <= starting_address) &&
             ((uint16_t)END_HOLDING_REGISTERS > starting_address)))
  {
    retval = ILLEGAL_DATA_ADDRESS;
  }

  /* process the request */
  else
  {
    bool     result = true;
    uint16_t ending_address = starting_address + quantity_of_registers - 1;

    /* response requires function code (already put there above), Starting
     * Address and Quantity of Registers */
    result = put_address_and_registers(starting_address, quantity_of_registers);

    /* create a copy of the current active eeprom block */
    eeprom_create_active_block_copy(true);

    if (result)
    {
      for (uint16_t address=starting_address; (retval==NO_ERROR && address<=ending_address); address++)
      {
        int16_t index = binary_search(process_request_table, REGS, address);

        if ((index != -1) && (REGS > index))
        {
          uint8_t msb, lsb;
          uint16_t data = 0;

          if(input_queue.dequeue(&msb) & input_queue.dequeue(&lsb))
          {
            data = (uint16_t)((msb << 8) + lsb);
          }

          /* process the request */
          retval = process_request_table[index].write_request(
            process_request_table[index].key,
            &data);
        }
        else
        {
          result = false;
          break;
        }
      }
    }

    if((true==result) && (NO_ERROR==retval))
    {
      /* commit to flash */
      result = eeprom_commit_changes();
    }
    else
    {
      retval = SERVER_DEVICE_FAILURE;
    }

  }
  return retval;
} /* write_multiple_registers */

