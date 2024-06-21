/** @file       gas_daq.h
 *  @brief      Definitions for the task and support functions to measure CO2.
 *  @author     David S. Cooper (DSC)
 *  @copyright  Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *  @copyright  All rights reserved.
 *
 *  $Date: 2017-12-11 14:59:00 -0500 (Mon, 11 Dec 2017) $
 *  $Revision: 190 $
 */
#if !defined(GAS_DAQ_H)
#define GAS_DAQ_H

#define LAMP_ON_STEPS		3
	 
/* Measurement variables found in this module. */

/** @brief      The most recent nadir (point 'below') measurement. */
extern uint16_t nadir;

/** @brief      The most recent zenith (point 'above') measurement.*/
extern uint16_t zenith;

/** @brief      The most recent detector temperature. */
extern uint16_t detector_temperature;

/** @brief      The most recent input voltage. */
extern uint16_t input_volts;

/** @brief      The most recent high-side lamp voltage, while the lamp is on. */
extern uint16_t  lamp_plus_volts;

/** @brief      The most recent low-side lamp voltage, while the lamp is on. */
extern uint16_t  lamp_minus_volts;

/** @brief      This is the temperature corrected zenith-nadir signal. It is
 *              visible as the Modbus Input Register TCOR_SIG.
 */
extern float tcor_sig;

/** @brief      This is the temperature corrected zenith-nadir signal (i.e.,
 *              tcor_sig) processed by a median filter. This is the basic raw
 *              signal out of the gas measurement task.
 */
extern uint16_t raw_sig;


#ifdef __cplusplus
extern "C" {
#endif

/** @brief      Periodic task that does the CO2 measurement. The entire
 *              measurement cycle (lamp control, ADC measurements, etc.) are
 *              handled in this function.
 *  @param [in] tcb
 *              Pointer to the task control block (Tcb) for this task.
 */
//void gas_daq_task(pTcb tcb);
void gas_daq_task(void);

#ifdef __cplusplus
}
#endif

#endif /* GAS_DAQ_H */

