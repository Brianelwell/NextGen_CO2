/** @file       inputRegsFuncs.h
 *  @brief      These are functions that handle a Modbus Read Input Registers
 *              request when recognized by the program. All of these functions
 *              populate the process_request_table[] in the module
 *              readInputRegisters.cpp.
 *
 *  @author     David S. Cooper (DSC)
 *  @copyright  Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *  @copyright  All rights reserved.
 *
 *  $Date: 2018-01-18 18:07:51 -0500 (Thu, 18 Jan 2018) $
 *  $Revision: 191 $
 */
#if !defined(INPUT_REGS_FUNCS_H)
#define INPUT_REGS_FUNCS_H

#ifdef __cplusplus
extern "C" {
#endif

/** @brief  Retrieve the current device status word.
 *  @param [in] address
 *          parameter is not used.
 *  @param [out] data
 *          The destination address for the current device status.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_device_status(const uint16_t address, uint16_t* data);

/** @brief  Retrieve the most current processed gas value.
 *  @param [in] address
 *          parameter is not used.
 *  @param [out] data
 *          The destination address for the gas_ppm value.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_gas_ppm(const uint16_t address, uint16_t* data);

/** @brief  Retrieve the most current raw gas measurement signal value.
 *  @param [in] address
 *          parameter is not used.
 *  @param [out] data
 *          The destination address for the current raw signal value.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_raw_sig(const uint16_t address, uint16_t* data);

/**
 *  @brief  Retrieve the most recent tcor calculation.
 *  @param [in] address
 *          The MODBUS Input Register address for tcor_sig (hi or low).
 *  @param [out] data
 *          The destination address for the tcor value.
 *  @return NO_ERROR if successful, else SLAVE_DEVICE_FAILURE is input parameter
 *          data is void or ILLEGAL_DATA_ADDRESS if input parameter address is not
 *          the proper address. */
uint8_t get_tcor_sig(const uint16_t address, uint16_t* data);

/** @brief Retrieve the most recent normalized signal calculation.
 *  @param [in] address
 *          The MODBUS Input Register address for norm_sig (hi or low).
 *  @param [out] data
 *          The destination address for the norm_sig value.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_norm_sig(const uint16_t address, uint16_t* data);

/** @brief  Retrieve the most recent averaged normalized signal calculation.
 *  @param [in] address
 *          The MODBUS Input Register address for norm_sig_avg (hi or low).
 *  @param [out] data
 *          The destination address for the norm_sig value.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_norm_sig_avg(const uint16_t address, uint16_t* data);

/** @brief  This parameter is used in the power filter and the tap weight.
 *  @param [in] address
 *          The MODBUS Input Register address for norm_sig_avg_alpha (hi or
 *          low).
 *  @param [out] data
 *          The destination address for the norm_sig_avg_alpha value.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_norm_sig_avg_alpha(const uint16_t address, uint16_t* data);

/** @brief  Retrieve the most current nadir measurement.
 *  @param [in] address
 *          parameter is not used.
 *  @param [out] data
 *          The destination address for the current nadir measurement.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_nadir(const uint16_t address, uint16_t* data);

/** @brief  Retrieve the most current zenith measurement.
 *  @param [in] address
 *          parameter is not used.
 *  @param [out] data
 *          The destination address for the current zenith measurement.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_zenith(const uint16_t address, uint16_t* data);

/** @brief  Retrieve the most current TEMP_SIGNAL measurement from the ASIC as
 *          measured in the gas_daq_task() function.
 *  @param [in] address
 *          parameter is not used.
 *  @param [out] data
 *          The destination address for ASIC TEMP_SIGNAL measurement.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_detector_temperature(const uint16_t address, uint16_t* data);

/** @brief  Retrieve the most current ADC_CHANNEL_V_LAMP_PLUS measurement from
 *          the ASIC as measured in the gas_daq_task() function.
 *  @param [in] address
 *          The parameter is not used.
 *  @param [out] data
 *          The destination address for VIN_ADC measurement.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_lamp_plus_volts(const uint16_t address, uint16_t* data);

/** @brief  Retrieve the most current ADC_CHANNEL_V_LAMP_MINUS measurement from
 *          the ASIC as measured in the gas_daq_task() function.
 *  @param [in] address
 *          The parameter is not used.
 *  @param [out] data
 *          The destination address for VIN_ADC measurement.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect. */
uint8_t get_lamp_minus_volts(const uint16_t address, uint16_t* data);

/** @brief  Retrieve the most current VIN_ADC measurement from the ASIC as
 *          measured in the gas_daq_task() function.
 *  @param [in] address
 *          parameter is not used.
 *  @param [out] data
 *          The destination address for VIN_ADC measurement.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect.
 */
uint8_t get_input_volts(const uint16_t address, uint16_t* data);

/** @brief Return the current state of the rt_ops_flag. This indicates the
 *         communications and PWM settings in use.
 *  @param [in]  address
 *          parameter is not used.
 *  @param [out] data
 *          The destination address for the rt_ops_flag.
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect.
 */
uint8_t get_rt_ops_flag(const uint16_t address, uint16_t* data);

/**
 *  @brief Return the number of seconds the device has run continuously.
 *  @param [in] address
 *          parameter is not used.
 *  @param [out] data
 *          The destination address for RUN_TIME accumulation.
 *  @param [in,out]
 *  @return NO_ERROR or SLAVE_DEVICE_FAILURE is input parameter incorrect.
 */
uint8_t get_up_time(const uint16_t address, uint16_t* data);

#ifdef __cplusplus
}
#endif

#endif /* INPUT_REGS_FUNCS_H */

