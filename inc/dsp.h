/** @file       dsp.h
 *  @brief      This module has all the functions that are used to calculate the
 *              gas ppm from the ADC measurements. These measurements are done
 *              in the gas_daq_task() function in the gas_daq.cpp module.
 *  @author     David S. Cooper (DSC)
 *  @copyright  Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *  @copyright  All rights reserved.
 *
 *  $Date: 2019-01-11 16:16:40 -0500 (Fri, 11 Jan 2019) $
 *  $Revision: 200 $
 */
#if !defined(DSP_H)
#define DSP_H

/** @name       Temperature Correction Coefficients
 *              These marcos are convenience definitions for passing the address
 *              of the first coefficient as a function parameter.
 * @see         calculate_temperature_correction in dsp.cpp.
 * @{ */

/** @brief      Used in the temperature correction calculation for gas ppm less
 *              than the span (1) target gas ppm. */
#define TCOR1_COEFFS (float*)((void*)&block->tcor1_coeff[0].re)

/** @brief      Used in the temperature correction calculation for gas ppm less
 *              than, and greater than the span (1) target gas ppm. */
#define TCOR2_COEFFS (float*)((void*)&block->tcor2_coeff[0].re)

/** @brief      Used in the temperature correction calculation for gas ppm
 *              greater than the span (1) target gas ppm. */
#define TCOR3_COEFFS (float*)((void*)&block->tcor3_coeff[0].re)
/**@}*/

/** @name       Gas Calculation Coefficients
 *              This is documentation related to the gas ppm coefficients.
 * @see dsp_calculate_gas_ppm in dsp.cpp.
 * @{ */

/** @brief      Coefficients used in the ppm calculation for gas concentrations
 *              of 1000 ppm or less. */
#define GAS1_PPM_COEFFS  (float*)((void*)&block->gas1_ppm_coeff[0].re)

/** @brief      Coefficients used in the ppm calculation for gas concentrations
 *              of 1000 ppm more less. */
#define GAS2_PPM_COEFFS  (float*)((void*)&block->gas2_ppm_coeff[0].re)

#define DSP_ADAPT_COEFF_QUAD (block->dsp_adapt_coeff.re)*(block->dsp_adapt_coeff.re)*(block->dsp_adapt_coeff.re)*(block->dsp_adapt_coeff.re)*(block->dsp_adapt_coeff.re)

/** @brief      Special firmware feature for Media company. This is to adjust 
 *              starting PPM. */
#define SPECIAL_MEDIA_FEATURE   10              /**< we are using avg_ctrl reg for this feature. */   
#define LOWEST_PPM              400
#define INITIAL_PPM             550
#define ADJUST_PPM_BOUNDRY      800   

/** @brief      Values define for dsp alphe coefficient register, for different 
 *              cases. */
#define NO_AVERAGING_VALUE              1.0
#define MINIMAL_AVERAGING_VALUE         0.9
#define HALF_AVERAGING_VALUE            0.5
#define QUARTER_AVERAGING_VALUE         0.25    
#define EIGHTH_AVERAGING_VALUE          0.125

/** @brief      This is to define different states of averaging filter */
typedef enum avg_ctrl_tag
{
  DEFAULT_AVERAGING   = 0,      /**< use programmed filter coefficients and filter counters and median filter. */
  NO_AVERAGING        = 1,      /**< dsp alpha coefficient set to 1.0, overriding filter.                       */
  MINIMAL_AVERAGING   = 2,      /**< dsp alpha coefficient set to 0.9, overriding filter.                       */
  HALF_AVERAGING      = 3,      /**< dsp alpha coefficient set to 0.5, overriding filter.                       */
  QUARTER_AVERAGING   = 4,      /**< dsp alpha coefficient set to 0.25, overriding filter.                      */
  EIGHTH_AVERAGING    = 5,      /**< dsp alpha coefficient set to 0.125, overriding filter.                     */
} Avg_Ctrl_States;

/** @brief The most recent processed gas mesurement. */
extern uint16_t gas_ppm;

/** @brief      This is the normalized signal, which is the temperature
 *              corrected signal normalized using the calibration results. It is
 *              visible as a Modbus Input Register. */
extern float norm_sig;

/** @brief      This represents the heavily filtered normalized signal,
 *              historically known at the normalized signal average. */
extern float norm_sig_avg;

/** @brief      This is the filter coefficient for the exponential filter used
 *              to filter the normailzed signal. */
extern float norm_sig_avg_alpha;

extern uint16_t dsp_adapt_counter;

#ifdef __cplusplus
extern "C" {
#endif

/** @brief          Calculate the gas ppm based on the current raw signal,
 *                  temperature correction and calibration factors.
 */
void dsp_calculate_gas_ppm(void);


/** @brief          Implements the algorithm that adjusts the gas ppm by
 *                  considering the altitude and pressure.
 *  @param [in]     x
 *                  Assumed to be the current gas calculation.
 *  @return         The result of the altitude correction.
 */
float dsp_altitude_correction(float x);

/** @brief          Implements the mathematical operation:
 *
 *  \f$x = \frac{(zero\_cal\_result - norm\_sig\_avg) *
 *  (zero\_cal\_result-span1\_cal\_result)}{abc\_correction\_factor}\f$
 *
 *  @return         The result of the calculation
 */
float dsp_calculate_tcor_hat(void);

/** @brief          Re-initialize the adaptive filter. */
void dsp_initialize_adaptive_filter(float gas);

/** @brief          Initializes data structures and the first gas ppm. */
void dsp_initialize_gas_ppm(void);

#ifdef __cplusplus
}
#endif

#endif /* DSP_H */

