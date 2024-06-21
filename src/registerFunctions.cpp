/* -----------------------------------------------------------------------------
 *            file: registerFunctions.cpp
 *        synopsis: A collection of register functions used by various Modbus
 *                  request demands.
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
 *  10/23/2013    DSC             Created. Took functions scattered around the
 *                                Modbus function directory (mbfuncs) and
 *                                aggregated them here.
 */
#include  <stdint.h>
#include  "modbus.h"              /* MODBUS definitions */
#include  "T67xx.h"               /* xxx_REGISTERS_OFFSET, etc. */
#include  "readInputRegisters.h"  /* UPDATE_COUNT, etc. */
#include  "holdingRegs.h"         /* SERIAL_NUMBER_HI, etc. */
#include  "eeprom.h"              /* eeprom*, etc. */
#include  "registerFunctions.h"   /* extern "C" for functions below */

/* ----- data types --------------------------------------------------------- */
/* ----- definitions -------------------------------------------------------- */
/* ----- global declarations ------------------------------------------------ */
/* ----- local declarations ------------------------------------------------- */
/* ----- forward declarations ----------------------------------------------- */

/* -----------------------------------------------------------------------------
 *       synopsis : Update the model_number fields in eeprom.
 *     param [in] : address - MODBUS Holding Register for MODEL_NUMBER.
 *     param [in] : data - the manufacture (actually testing) date.
 *         return : NO_ERROR if the model_number field was updated
 *                  successfully, otherwise returns SLAVE_DEVICE_FAILURE (0x04)
 *                  if the eeprom failed to update.
 *            see : eeprom.h, holdingRegs.h, writeSingleReg.cpp
 */
uint8_t set_model_number(const uint16_t address, uint16_t* data)
{
  return eeprom_write(address, data);
} /* set_model_number */

/* -----------------------------------------------------------------------------
 *       synopsis : Update one of the serial_number fields in eeprom.
 *     param [in] : address - MODBUS Holding Register for;
 *                    SERIAL_NUMBER_HI
 *                    SERIAL_NUMBER_LO
 *     param [in] : data - new serial number.
 *         return : NO_ERROR if the serial_number field was updated
 *                  successfully, otherwise returns SLAVE_DEVICE_FAILURE (0x04)
 *                  if the eeprom failed to update.
 *            see : eeprom.h, holdingRegs.h, writeSingleReg.cpp
 *           note : The serial_number field in the eeprom is 4 bytes which
 *                  requires 2 register writes to write all of it. Data is
 *                  written most significant to least.
 */
uint8_t set_serial_number(const uint16_t address, uint16_t* data)
{
  return eeprom_write(address, data);
} /* set_serial_number */

/* -----------------------------------------------------------------------------
 *       synopsis : Retrieve the current SLAVE_ADDRESS value from eeprom memory.
 *     param [in] : address - MODBUS Holding Register address for SLAVE_ADDRESS.
 *    param [out] : data - destination address for the SLAVE_ADDRESS value.
 *         return : NO_ERROR if the eeprom can be read, otherwise returns the
 *                  MODBUS SLAVE_DEVICE_FAILURE (0x04).
 *            see : holdingRegs.h, readHoldingRegs.cpp
 */
uint8_t get_slave_address(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;
  uint16_t value;

  if (0 != data)
  {
    retval = eeprom_read(SLAVE_ADDRESS, &value);
    if (NO_ERROR == retval)
    {
      /* make sure that the value is bounded */
      *data = (uint16_t)(0x00FF & value);
    }
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* get_slave_address */

/* -----------------------------------------------------------------------------
 *       synopsis : Update the slave_address field in eeprom.
 *     param [in] : address - MODBUS Holding Register for SLAVE_ADDRESS.
 *     param [in] : data - new slave address. The change takes effect after
 *                  reset.
 *         return : NO_ERROR if the data value is valid and the selected field
 *                  was updated successfully, otherwise returns
 *                  ILLEGAL_DATA_VALUE (0x03) error if the SLAVE_ADDRESS value
 *                  was out of range or SLAVE_DEVICE_FAILURE (0x04) if the eeprom
 *                  failed to update.
 *            see : holdingRegs.h, writeSingleReg.cpp
 */
uint8_t set_slave_address(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    if ((*data>=1) && (*data<=247))
    {
      retval = eeprom_write(SLAVE_ADDRESS, data);
    }
    else
    {
      retval = ILLEGAL_DATA_VALUE;
    }
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* set_slave_address */

/* -----------------------------------------------------------------------------
 *       synopsis : Generalized function to write the eeprom from a MODBUS
 *                  function call. This call is specific for updates that
 *                  control the DAC as the value needs to be range bound (i.e.,
 *                  0 to 4095).
 *     param [in] : address - The MODBUS Holding Register;
 *                    DAC_LAMP_ON_VOLTAGE
 *                    DAC_LAMP_ON_VOLTAGE_1
 *                    DAC_LAMP_ON_VOLTAGE_2
 *     param [in] : data - The new register value. The change usually takes
 *                  effect on reset.
 *         return : NO_ERROR if the selected field was updated successfully,
 *                  otherwise returns SLAVE_DEVICE_FAILURE (0x04) if the eeprom
 *                  failed to update.
 *            see : writeSingleReg.cpp
 */
uint8_t set_dac_value_to_eeprom(const uint16_t address, uint16_t* data)
{
  uint8_t retval = NO_ERROR;

  if (0 != data)
  {
    if (*data < 4096 )
    {
      retval = eeprom_write(address, data);
    }
    else
    {
      retval = ILLEGAL_DATA_VALUE;
    }
  }

  return retval;
} /* set_dac_value_to_eeprom */

/* -----------------------------------------------------------------------------
 *       synopsis : Update the baud_rate field in eeprom.
 *     param [in] : address - MODBUS Holding Register address for BAUD_RATE.
 *     param [in] : data - new baud rate value. The change takes effect after
 *                  reset.
 *         return : NO_ERROR if the data value is valid and the selected
 *                  field was updated successfully, otherwise returns
 *                  ILLEGAL_DATA_VALUE (0x03) error if the BAUD_RATE was out of
 *                  range or SLAVE_DEVICE_FAILURE (0x04) if the eeprom failed to
 *                  update.
 *            see : holdingRegs.h, writeSingleReg.cpp
 */
uint8_t set_baud_rate(const uint16_t address, uint16_t* data)
{
  uint8_t      retval = NO_ERROR;

  if (0 != data)
  {
    /* bound the possible values */
    if ((*data==9600) || (*data==19200))
    {
      retval = eeprom_write(BAUD_RATE, data);
    }
    else
    {
      retval = ILLEGAL_DATA_VALUE;
    }
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* set_baud_rate */

/* -----------------------------------------------------------------------------
 *       synopsis : Update the parity field in eeprom.
 *     param [in] : address - MODBUS Holding Register address for PARITY.
 *     param [in] : data - The new parity value. The change takes effect after
 *                  reset.
 *         return : NO_ERROR if the data value is valid and the eeprom was
 *                  updated successfully, otherwise returns ILLEGAL_DATA_VALUE
 *                  (0x03) error if the PARITY value was out of range or
 *                  SLAVE_DEVICE_FAILURE (0x04) if the eeprom failed to update.
 *            see : holdingRegs.h, writeSingleReg.cpp
 */
uint8_t set_parity(const uint16_t address, uint16_t* data)
{
  uint8_t      retval = NO_ERROR;

  if (0 != data)
  {
    /* bound the possible values */
    if ((*data==EVEN_PARITY) || (*data==ODD_PARITY) || (*data==NO_PARITY))
    {
      retval = eeprom_write(PARITY, data);
    }
    else
    {
      retval = ILLEGAL_DATA_VALUE;
    }
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* set_parity */

/* -----------------------------------------------------------------------------
 *       synopsis : Bounds the maximum number of samples that will be considered
 *                  for the ABC correct factor, and for the maximum number of
 *                  samples that will be copied when one of the ping-pong flash
 *                  page does a write.
 *     param [in] : address - MODBUS Holding Register address for the
 *                  ABC_MAX_SAMPLE_COUNT.
 *     param [in] : data - The new abc_sample_count value. The change takes
 *                  effect after reset.
 *         return : NO_ERROR if the data value is valid and the eeprom was
 *                  updated successfully, otherwise returns ILLEGAL_DATA_VALUE
 *                  (0x03) error if the ABC_MAX_SAMPLE_COUNT value was greater
 *                  than ABC_MAX_SAMPLE_COUNT or SLAVE_DEVICE_FAILURE (0x04) if
 *                  the eeprom failed to update.
 *            see : holdingRegs.h, writeSingleReg.cpp
 */
uint8_t set_abc_sample_count(const uint16_t address, uint16_t* data)
{
  uint8_t   retval = NO_ERROR;

  if (0 != data)
  {
    /* bound the sample size to the temp_block[] */
    if (*data <= ABC_MAX_SAMPLE_COUNT)
    {
      retval = eeprom_write(ABC_SAMPLE_COUNT, data);
    }
    else
    {
      retval = ILLEGAL_DATA_VALUE;
    }
  }
  else
  {
    retval = SERVER_DEVICE_FAILURE;
  }

  return retval;
} /* set_abc_sample_count */

