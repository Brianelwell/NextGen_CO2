/** @file       registerFunctions.h
 *  @brief      A collection of register functions used by various Modbus request
 *              demands.
 *  @author     David S. Cooper (DSC)
 *  @copyright  Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *  @copyright  All rights reserved.
 *
 *  $Date: 2017-12-11 14:59:00 -0500 (Mon, 11 Dec 2017) $
 *  $Revision: 190 $
 */
#if !defined(REGISTER_FUNCTIONS_H)
#define REGISTER_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief Update the model_number fields in eeprom.
 *  @param [in] address The MODBUS holding register for MODEL_NUMBER.
 *  @param [in] data The manufacture (actually testing) date.
 *  @return NO_ERROR if the model_number field was updated successfully, otherwise
 *  returns SLAVE_DEVICE_FAILURE (0x04) if the eeprom failed to update.
 *  @note This should only update the requested field if that field is 0xFFFF
 *  (i.e., has never been updated). The manufacturing date should only be
 *  written to a newly programmed device.
 */
uint8_t set_model_number(const uint16_t address, uint16_t* data);

/**
 *  @brief Update one of the serial_number fields in eeprom.
 *  @param [in] address The MODBUS Holding Register for either SERIAL_NUMBER_HI
 *  or SERIAL_NUMBER_LO.
 *  @param [in] data The new serial number.
 *  @return NO_ERROR if the serial_number field was updated successfully,
 *  otherwise returns SLAVE_DEVICE_FAILURE (0x04) if the eeprom failed to
 *  update.
 *  @note This should only update the requested field if that field is 0xFFFF.
 *  The serial number should only be written to a newly programmed device.
 *  @note The serial_number field in the eeprom is 4 bytes which requires 2
 *  register writes to write all of it. Data is written most significant to
 *  least.
 */
uint8_t set_serial_number(const uint16_t address, uint16_t* data);

/**
 *  @brief Generalized function to write the eeprom from a MODBUS function call.
 *  This call is specific for updates that control the DAC as the value needs to
 *  be range bound (i.e., 0 to 4095).
 *  @param [in] address - The MODBUS Holding Register;
 *  <ul>
 *    <li>LAMP_DATA_VOLT_0</li>
 *    <li>LAMP_DATA_VOLT_1</li>
 *    <li>LAMP_DATA_VOLT_2</li>
 *  </ul>
 *  @param [in] data - The new register value. The change usually takes effect
 *  on reset.
 *  @return NO_ERROR if the selected field was updated successfully, otherwise
 *  returns SLAVE_DEVICE_FAILURE (0x04) if the eeprom failed to update.
 */
uint8_t set_dac_value_to_eeprom(const uint16_t address, uint16_t* data);


/**
 *  @brief Retrieve the <i>slave_address</i> value from the eeprom.
 *  @param [in] address The MODBUS Holding Register address for SLAVE_ADDRESS.
 *  @param [out] data The destination address for the retrieved value.
 *  @return NO_ERROR if the selected field could be read, otherwise returns the
 *  MODBUS SLAVE_DEVICE_FAILURE (0x04).
 */
uint8_t get_slave_address(const uint16_t address, uint16_t* data);

/**
 *  @brief Update the <i>slave_address</i> field in eeprom.
 *  @param [in] address The MODBUS Holding Register for SLAVE_ADDRESS.
 *  @param [in] data The new slave address. The change takes effect on reset.
 *  @return NO_ERROR if the selected field was updated successfully, otherwise
 *  returns ILLEGAL_DATA_VALUE (0x03) error if the SLAVE_ADDRESS value was out
 *  of range or SLAVE_DEVICE_FAILURE (0x04) if the flash failed to update.
 */
uint8_t set_slave_address(const uint16_t address, uint16_t* data);

/**
 *  @brief Update the <i>baud_rate</i> field in eeprom.
 *  @param [in] address The MODBUS Holding Register address for BAUD_RATE.
 *  @param [in] data The new baud rate value. The change takes effect after
 *  reset.
 *  @return NO_ERROR if the selected field was updated successfully, otherwise
 *  returns ILLEGAL_DATA_VALUE (0x03) error if the BAUD_RATE was out of range or
 *  SLAVE_DEVICE_FAILURE (0x04) if the flash failed to update.
 */
uint8_t set_baud_rate(const uint16_t address, uint16_t* data);

/**
 *  @brief Update the <i>parity</i> field in eeprom.
 *  @param [in] address The MODBUS Holding Register address for PARITY.
 *  @param [in] data The new parity value. The change takes effect after reset.
 *  @return NO_ERROR if the selected field was updated successfully, otherwise
 *  returns ILLEGAL_DATA_VALUE (0x03) error if the PARITY value was out of range
 *  or SLAVE_DEVICE_FAILURE (0x04) if the flash failed to update.
 */
uint8_t set_parity(const uint16_t address, uint16_t* data);

/**
 *  @brief Bounds the maximum number of samples that will be considered for the
 *  ABC correct factor, and for the maximum number of samples that will be
 *  copied when one of the ping-pong flash page does a write.
 *  @param [in] address MODBUS Holding Register address for the
 *  ABC_MAX_SAMPLE_COUNT.
 *  @param [in] data  The new abc_sample_count value. The change takes effect
 *  after reset.
 *  @return NO_ERROR if the data value is valid and the eeprom was updated
 *  successfully, otherwise returns ILLEGAL_DATA_VALUE (0x03) error if the
 *  ABC_MAX_SAMPLE_COUNT value was greater than ABC_MAX_SAMPLE_COUNT or
 *  SLAVE_DEVICE_FAILURE (0x04) if the eeprom failed to update.
 */
uint8_t set_abc_sample_count(const uint16_t address, uint16_t* data);

#ifdef __cplusplus
}
#endif

#endif /* REGISTER_FUNCTIONS_H */

