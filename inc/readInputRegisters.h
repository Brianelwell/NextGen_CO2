/** @file       readInputRegisters.h
 *  @brief      Implements MODBUS function 4, Read Input (read only) Registers.
 *
 *              This function code is used to read from 1 to 125 contiguous
 *              input registers in the sensor. The Request PDU specifies the
 *              starting register address and the number of registers. In the
 *              PDU Registers are addressed starting at zero.
 *
 *              The register data in the response message are packed as two
 *              bytes per register, with the binary contents right justified
 *              within each byte. For each register, the first byte contains the
 *              high order bits and the second contains the low order bits.
 *
 *  @author     David S. Cooper (DSC)
 *  @copyright  Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *  @copyright  All rights reserved.
 *
 *  $Date: 2018-01-18 18:07:51 -0500 (Thu, 18 Jan 2018) $
 *  $Revision: 191 $
 */
#if !defined(READINPUTREGISTERS_H)
#define READINPUTREGISTERS_H

/** @brief Definition for the Read Input Registers MODBUS function. */
const uint8_t READ_INPUT_REGISTERS = 4;

/** @defgroup read_input_registers_memory_map Read Input Registers Memory Map
 *
 *  See @ref T67xx_base_address_definitions for the INPUT_REGISTERS_OFFSET
 *  definition.
 *
 * @{ */

/** @brief Definitions for the MODBUS registers accessible with the Read Input
 *  Registers Function (4, 0x04). These are read only (R0) registers. */
enum Input_Registers
{
  UPDATE_COUNT = INPUT_REGISTERS_OFFSET, /**< UPDATE_COUNT  @ 5000 */
  FIRMWARE_REVISION,      /**< FIRMWARE_REVISION            @ 5001 */
  STATUS,                 /**< STATUS                       @ 5002 */
  GAS_PPM,                /**< GAS_PPM                      @ 5003 */
  RAW_SIG,                /**< RAW_SIG                      @ 5004 */
  TCOR_SIG_HI,            /**< TCOR_SIG_HI                  @ 5005 */
  TCOR_SIG_LO,            /**< TCOR_SIG_LO                  @ 5006 */
  NORM_SIG_HI,            /**< NORM_SIG_HI                  @ 5007 */
  NORM_SIG_LO,            /**< NORM_SIG_LO                  @ 5008 */
  NORM_SIG_AVG_HI,        /**< NORM_SIG_AVG_HI              @ 5009 */
  NORM_SIG_AVG_LO,        /**< NORM_SIG_AVG_LO              @ 5010 */
  NORM_SIG_AVG_ALHPA_HI,  /**< NORM_SIG_AVG_ALHPA_HI        @ 5011 */
  NORM_SIG_AVG_ALPHA_LO,  /**< NORM_SIG_AVG_ALPHA_LO        @ 5012 */
  NADIR,                  /**< NADIR                        @ 5013 */
  ZENITH,                 /**< ZENITH                       @ 5014 */
  TEMP_SIGNAL,            /**< TEMP_SIGNAL                  @ 5015 */
  V_LAMP_PLUS,            /**< V_LAMP_PLUS                  @ 5016 */
  V_LAMP_MINUS,           /**< V_LAMP_MINUS                 @ 5017 */
  VIN_ADC,                /**< VIN_ADC                      @ 5018 */
  CO2_BOUND_VALUE,        /**< CO2_BOUND_VALUE              @ 5019 */
  FOOBAR1,                /**< FOOBAR1                      @ 5020 */
  FOOBAR2,                /**< FOOBAR2                      @ 5021 */
  RT_OPS_FLAG,            /**< RT_OPS_FLAG                  @ 5022 */
  UP_TIME_HI,             /**< UP_TIME_HI                   @ 5023 */
  UP_TIME_LO,             /**< UP_TIME_LO                   @ 5024 */
  END_INPUT_REGISTERS     /*                                @ 5025 */
};

/** @} */

#ifdef __cplusplus
extern "C" {
#endif


/**
 *  @brief Implements the Read Input Registers MODBUS function.
 *  @return NO_ERROR (0) or one of the MODBUS Exception Codes.
 *  @see MODBUS Application Protocol Specification V1.1b.
 */
uint8_t read_input_registers(void);

#ifdef __cplusplus
}
#endif

#endif /* READINPUTREGISTERS_H */

