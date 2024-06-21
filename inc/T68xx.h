/** @file       T67xx.h
 *  @brief      Addresses and offsets specific to the T67xx series of CO2
 *              sensors.
 *
 *              In general the register types and reference ranges recognized by
 *              the T67xx family will adhere to the following order;
 *
 *                            Coils: 1000-1999
 *                  Discrete Inputs: 2000-2999
 *                     File Records: 3000-3999
 *                Holding Registers: 4000-4999
 *                  Input Registers: 5000-5999
 *
 *              Note that these addresses are offsets relative to a base
 *              address.
 *
 *  @author     David S. Cooper (DSC)
 *  @copyright  Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *  @copyright  All rights reserved.
 *
 *  $Date: 2017-12-11 14:59:00 -0500 (Mon, 11 Dec 2017) $
 *  $Revision: 190 $
 */
#if !defined(T67XX_H)
#define T67XX_H


/** @defgroup T67xx_base_address_definitions T67xx Base Address Definitions
 * @{ */

/** @brief      This is the base MODBUS address that all other addresses are
 *              offset from.
 */
const uint16_t BASE_ADDRESS = 0;

/** @brief      Coils start at this offset. Discrete Output Coils (e.g., relays)
 *              can be read from using MODBUS Function 1 and written to using
 *              MODBUS Functions 5 (Write Single Coil) and 15 (Write Multiple
 *              Coils).
 */
const uint16_t COILS_OFFSET = 1000 + BASE_ADDRESS;

/** @brief      Discrete Inputs start at this offset. Discrete Input Contacts
 *              (e.g., switches) are read only and can be read using MODBUS
 *              Function 2 (Read Discrete Inputs).
 */
const uint16_t DISCRETE_INPUTS_OFFSET = 2000 + BASE_ADDRESS;

/** @brief      File Record access start at this offset. File records can be
 *              read from using MODBUS Function 20 (Read File Record) and
 *              written to using MODBUS Function 21 (Write File Record).
 */
const uint16_t FILE_RECORDS_OFFSET = 3000 + BASE_ADDRESS;

/** @brief      Holding Registers start at this offset. Analog Output Holding
 *              Registers (e.g., DAC values) can be read using MODBUS Function 3
 *              (Read Holding Registers) and written to using MODBUS Functions 6
 *              (Write Single Register) and 16 (Write Multiple Registers).
 */
const uint16_t HOLDING_REGISTERS_OFFSET = 4000 + BASE_ADDRESS;

/** @brief      Input Registers start at this offset. Analog Input Registers
 *              (e.g., ADC values) can be read using MODBUS Function 4 (Read
 *              Input Registers).
 */
const uint16_t INPUT_REGISTERS_OFFSET = 5000 + BASE_ADDRESS;

/** @} */

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* T67XX_H */

