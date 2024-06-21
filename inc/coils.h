/** @file       coils.h
 *  @brief      Definitions related to the writing of discrete outputs (i.e.,
 *              'coils'). For this project the discreet outputs are bits.
 *  @author     David S. Cooper (DSC)
 *  @copyright  Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *  @copyright  All rights reserved.
 *
 *  $Date: 2019-01-11 16:16:40 -0500 (Fri, 11 Jan 2019) $
 *  $Revision: 200 $
 */
#if !defined(COILS_H)
#define COILS_H


/** @brief Definition for the Write Single Coil MODBUS function 5 (0x05). */
const uint8_t WRITE_SINGLE_COIL = 5;


/** @defgroup coils_memory_map Coils
 *
 *  See @ref T67xx_base_address_definitions for the COILS_OFFSET definition.
 *
 * @{ */

/** @brief Definitions for the MODBUS registers accessible with the Read Coils
 * Function (1, 0x01), Write Single Coil Function 5 (0x05) and Write Multiple
 * Coils MODBUS Function 15 (0x0F). Note that only the Write Single Coil
 * Function is implemented in this code. */
enum Coils
{
  /** @brief Reset the device. Note there will be no MODBUS response from the
   * device as it resets immediately. This is useful when changing some of the
   * operating parameters of the device (e.g., CO2 measurement task period. */
  RESET_DEVICE = COILS_OFFSET,

  /** @brief  Setting this bit starts the zero calibration function of the
   * module. Clearing this bit stops the data acquisition of samples for the
   * calibration procedure but once the actual calibration function is called it
   * cannot be stopped. This bit is cleared automatically when the calibration
   * has finished or has been stopped.  */
  START_ZERO_CAL,

  /** @brief  Setting this bit starts the span (1) calibration function of the
   * module. Clearing this bit stops the data acquisition of samples for the
   * calibration procedure but once the actual calibration function is called it
   * cannot be stopped. This bit is cleared automatically when the calibration
   * has finished or has been stopped.  */
  START_SPAN1_CAL,

  /** @brief  Setting this bit starts the span (2) calibration function of the
   * module. Clearing this bit stops the data acquisition of samples for the
   * calibration procedure but once the actual calibration function is called it
   * cannot be stopped. This bit is cleared automatically when the calibration
   * has finished or has been stopped.  */
  START_SPAN2_CAL,

  /** @brief  Setting this bit starts the single-point calibration function of
   * the module. Clearing this bit stops the data acquisition of samples for the
   * calibration procedure but once the actual calibration function is called it
   * cannot be stopped. This bit is cleared automatically when the calibration
   * has finished or has been stopped.  */
  START_SINGLE_POINT_CAL,

  /** @brief  Setting this bit causes the device to update it's eeprom block
   * pointer. This has the same effect as writing twice or resetting the device.
   * It is a helper function for manufacturing. */
  UPDATE_EEPROM_BLOCK,

  /** @brief  Control for ABC logic. A write of 0xFF00 (i.e., ON) will enable
   * the ABC logic and a write of 0x0000 (i.e., OFF) will disable the ABC
   * logic function. */
  ABC_LOGIC_CTRL,

  /** @brief Enter or leave test mode. This mode turns off the CO2 measurement
   * task and allows the user to affect hardware in the module. */
  TEST_MODE_0,

  /** @brief Reset the slave address to DEFAULT_SLAVE_ADDRESS. This action
   * requires that the device be rebooted or the eeprom pointer updated. */
  TEST_MODE_1,

  /** @brief  Used by the test jig to put the device on a local bus by setting
   * J-5 low (multiplexes away from the RS-485 drop bus). WRITE_SINGLE_COIL_ON
   * pulls J-5 high, WRITE_SINGLE_COIL_OFF pulls J-5 low. */
  TEST_MODE_2,

  /** @brief  Control the LAMP_DRIVER_AMP_POWER pin. WRITE_SINGLE_COIL_ON
   * turns the lamp on, WRITE_SINGLE_COIL_OFF turns the lamp off. */
  TEST_MODE_3,
  
  /** @brief Enter or leave test mode. This mode turns off the CO2 measurement
   * task on demand and allows the user to affect hardware in the module. */
  TEST_MODE_4,

  END_COILS /**< @brief End of coils marker. */
};

/** @} */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* COILS_H */

