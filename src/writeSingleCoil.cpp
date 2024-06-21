/* -----------------------------------------------------------------------------
 *            file: writeSingleCoil.cpp
 *        synopsis: Implements MODBUS function 5, Write Single Coil (a.k.a, a
 *                  discrete output).
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
 *  5/23/2013     DSC             Created
 */
#include  <stdint.h>
#include  "modbus.h"    /* various Modbus definitions */
#include  "T67xx.h"     /* COILS_OFFSET */
#include  "coils.h"     /* various definitions for discrete outputs */
#include  "writeSingleCoil.h" /* required for the extern "C" declaration */
#include  "queue.h"     /* must be included before globals.h */
#include  "status.h"    /* must be included before globals.h */
#include  "eeprom.h"    /* must be included before globals.h */
#include  "globals.h"   /* queues */
#include  "common.h"    /* process request ADT's, functions, etc. */
#include  "hardware.h"  /* r/w hardware functions */

/* ----- local declarations ------------------------------------------------- */
static process_request_type const process_request_table[] =
{
  { (uint16_t)RESET_DEVICE, bad_read, reset_device },
  { (uint16_t)START_ZERO_CAL, bad_read, start_zero_cal },
  { (uint16_t)START_SPAN1_CAL, bad_read, start_span1_cal },
  { (uint16_t)START_SPAN2_CAL, bad_read, start_span2_cal },
  { (uint16_t)START_SINGLE_POINT_CAL, bad_read, start_single_point_cal },
  { (uint16_t)UPDATE_EEPROM_BLOCK, bad_read, update_eeprom_block },
  { (uint16_t)ABC_LOGIC_CTRL, bad_read, abc_logic_ctrl },
  { (uint16_t)TEST_MODE_0, bad_read, test_mode_0 },
  { (uint16_t)TEST_MODE_1, bad_read, test_mode_1 },
  { (uint16_t)TEST_MODE_2, bad_read, test_mode_2 },
  { (uint16_t)TEST_MODE_3, bad_read, test_mode_3 },
  { (uint16_t)TEST_MODE_4, bad_read, test_mode_4 },
};

static const uint16_t REGS = sizeof(process_request_table) /
  sizeof(process_request_type);

/* -----------------------------------------------------------------------------
 *       synopsis : Write Single Coil, MODBUS Function 5 (0x05).
 *
 *                  Request:
 *                    |05| - function code (removed by parse_modbus_request())
 *                    |aa| - output address (MSB)
 *                    |aa| - output address (LSB)
 *                    |dd| - output value (MSB)
 *                    |dd| - output value (LSB)
 *
 *                    output address : 0x0000 - 0xFFFF
 *                    output value : 0x0000 (OFF) or 0xFF00 (ON)
 *
 *                      Note that for the output value there are only 2 possible
 *                      values, all other values are ignored.
 *
 *                  Response:
 *                    |05| - function code
 *                    |aa| - output address (MSB)
 *                    |aa| - output address (LSB)
 *                    |dd| - output value (MSB)
 *                    |dd| - output value (LSB)
 *
 *
 *                  Error:
 *                    |85| - error code
 *                    |ee| - exception code (1,2,3 or 4)
 *
 *                  Exception Codes :
 *                    0 - No error
 *                    1 - Illegal Function
 *                    2 - Illegal data address
 *                    3 - Illegal data value
 *                    4 - Slave device failure
 *
 *            see : MODBUS Application Protocol Specification V1.1b.
 */
uint8_t write_single_coil(void)
{
  uint16_t output_address;
  uint16_t output_value;
  uint8_t  retval = NO_ERROR;

  /* prime the pump */
  if (!(output_queue.enqueue(WRITE_SINGLE_COIL) &&
        get_address_and_registers(&output_address, &output_value)))
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  /* test the output value == OK */
  else if (!((WRITE_SINGLE_COIL_OFF == output_value) ||
             (WRITE_SINGLE_COIL_ON == output_value)))
  {
    retval = ILLEGAL_DATA_VALUE;
  }

  /* test output address == OK */
  else if (!(((uint16_t)COILS_OFFSET<=output_address) &&
             ((uint16_t)END_COILS > output_address)))
  {
    retval = ILLEGAL_DATA_ADDRESS;
  }

  /* build the response and set the register data */
  else
  {
    uint8_t  x = 0;
    int16_t  index = 0;
    bool     result = true;

    while ((index<4) && (result==true))
    {
      if (0==index)
      {
        x = (uint8_t)((output_address>>8) & 0x00ff);
      }
      else if (1==index)
      {
        x = (uint8_t)((output_address) & 0x00ff);
      }
      else if (2==index)
      {
        x = (uint8_t)((output_value>>8) & 0x00ff);
      }
      else
      {
        x = (uint8_t)((output_value) & 0x00ff);
      }

      result = output_queue.enqueue(x);
      index++;
    } /* end-while */

    if (result)
    {
      index = binary_search(process_request_table, REGS, output_address);

      if ((index != -1) && (REGS > index))
      {
        retval = process_request_table[index].write_request(
            process_request_table[index].key,
            &output_value);
      }
    }
    else
    {
      retval = SERVER_DEVICE_FAILURE;
    }
  }

  return retval;
} /* write_single_coil */

