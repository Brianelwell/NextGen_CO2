/* -----------------------------------------------------------------------------
 *            file: eeprom.cpp
 *        synopsis: Functions that are responsible for eeprom emulation.
 *
 *                  The update function (i.e., eeprom_write_new_active_block)
 *                  ping-pongs between two pages of flash - as defined by the
 *                  eeprom_block_pointer_array[] data structure. The function
 *                  checks the first page of flash to see if it's the current
 *                  (eeprom) emulation memory and if it is the second page will
 *                  automatically be used, guaranteeing eventually that one
 *                  pages gets marked as 'dirty' and one page as 'clean'.
 *
 *                  The definition for the eeprom layout is in eeprom.h.
 *
 *          author: David S. Cooper (DSC)
 *       copyright: Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *                  All rights reserved.
 *           $Date: 2019-07-15 12:48:09 -0400 (Mon, 15 Jul 2019) $
 *       $Revision: 207 $
 *
 *  Revision history
 *
 *  Date          Name            Description
 *  ============= =============== ==============================================
 *  6/12/2013     DSC             Modified from the Ventostat BACnet code for
 *                                this (Freescale) ARM Cortex-M0+ CPU.
 */
#include "main.h"

/*
#if defined(WIN32)
#include  <stdint.h>
#include  <stdlib.h>
#else
*/
//#include  <cstdint>               /* uintxx_t */
//#include  <cstdlib>               /* NULL */
//#endif
//#include  "eeprom.h"              /* eeprom* */
/*
#include  "IO_Map.h"              //  ESFC (Enable Stalling Flash Controller)
#include  "common.h"              //  process request ADT's, functions, etc.
*/
//#include  "flash.h"               //  low level flash r/w functions
/*
#include  "queue.h"               //  required before globals.h
#include  "status.h"              //  status definitions
#include  "globals.h"             //  global status register
#include  "abc_logic.h"           //  abc_logic_get_count
#include  "modbus.h"              //  MODBUS definitions
*/
/*
#include  "T68xx.h"               //  needed for modbus register definitions
#include  "holdingRegs.h"         //  EEPROM related definitions
#include  "readInputRegisters.h"  //  EEPROM related definitions
*/

/* There are several casts used in this module that lint complains about. All
 * involve;
 *  cast from unsigned int to pointer (MISRA 2004 Rule 11.3, advisory)
 *  cast from pointer to unsigned int (MISRA 2004 Rule 11.3, advisory)
 *  cast from pointer to pointer      (MISRA 2004 Rule 11.4, advisory)
 *
 * These rules are "elective" because they are not normally on (they are in this
 * case). Each cast has been validated through testing.
 */
/*lint -e9??  Ignore 'Elective' messages about casting pointers to uint32_t's */
/*lint -e1784 Functions are previously declared as "C" in header file */

/* ----- local declarations ------------------------------------------------- */

/* Pointer to the current eeprom block in memory. Functions that update the
 * eeprom area (see eeprom.c) are responsible for keeping this variable updated.
 * See eeprom_get_current_active_block() in this module. */
#if !defined(WIN32)
static eeprom_block* current_active_block = NULL; /* local */
#else
eeprom_block* current_active_block = NULL; /* global */
#endif

/* For the target device (i.e., STM32C0x1) the last EE_BLOCKS pages in flash
 * are used to emulate eeprom. Note that Lint is complaining about the fact that
 * the macro expansion for EE_START_ADDRESS has 0 (i.e., FLASH_START_ADDRESS)
 * for one of its terms.
 * Note: This should be 'const' but for unit testing it cannot be because the
 * arrays are dynamically allocated.
 */
/*lint -e{835} see comments above */
#if !defined(WIN32)
/*
static uint32_t* const eeprom_block_pointer_array[EE_BLOCKS] =
{
  (uint32_t*)((uint32_t)(EE_START_ADDRESS)),
  (uint32_t*)((uint32_t)(EE_START_ADDRESS + EE_SECTOR_SIZE)),
};
*/
#else
/*static*/ uint32_t* /*const*/ eeprom_block_pointer_array[EE_BLOCKS] =
{
  (uint32_t*)((uint32_t)(EE_START_ADDRESS)),
  (uint32_t*)((uint32_t)(EE_START_ADDRESS + EE_SECTOR_SIZE)),
};
#endif

/* ----- global declarations ------------------------------------------------ */
//uint32_t temp_block[ TEMP_BLOCK_SIZE ]; /* scratch pad buffer */

/* ----- forward declarations ----------------------------------------------- */
static bool eeprom_update(eeprom_block* source);
static bool eeprom_find_active_block(uint32_t** active_block);
static bool eeprom_find_free_block(uint32_t** free_block);

/* ----------------------------------------------------------------------------
 *       synopsis : Returns the offset to the CRC field in the eeprom_block
 *                  data structure.
 *     param [in] : block_arg - the block to examine.
 *         return : The offset to the CRC field in the data structure.
 */
uint32_t eeprom_calculate_crc_offset(eeprom_block const* block_arg)
{
  eeprom_block const* ebp = (eeprom_block const*)((void const*)block_arg);
  return (uint32_t)&(ebp->crc) - (uint32_t)&(ebp->block_marker);
} /* eeprom_calculate_crc_offset */

/* -----------------------------------------------------------------------------
 *       synopsis : Assumes that the temp block has the latest changes for the
 *                  eeprom emulation and writes the temp block to a next free
 *                  flash page used for emulation.
 *         return : true if successful, false otherwise.
 *            see : temp_block[] definition this module
 *           note : The call to eeprom_update() marks the temp_block as
 *                  inactive.
 */
bool eeprom_commit_changes(void)
{
  /*
   * Enable Stalling Flash Controller
   * This field is used to enable stalling flash controller when flash is busy.
   * 0 Disable stalling flash controller when flash is busy.
   * 1 Enable stalling flash controller when flash is busy.
   */


  bool result = true;
/*
  MCM_PLACR |= (MCM_PLACR_ESFC_MASK);
  eeprom_block* ebp = (eeprom_block*)((void*)&temp_block);

  //  update the count
  ebp->update_count++;
  uint16_t length = (uint16_t)eeprom_calculate_crc_offset(ebp);
  ebp->crc = crc16((uint8_t*)ebp, length);
  result = eeprom_update(ebp);
  MCM_PLACR &= (~MCM_PLACR_ESFC_MASK);
*/
  return result;

} /* eeprom_commit_changes */

/* -----------------------------------------------------------------------------
 *       synopsis : Not only updates the flash block used for eeprom emulation
 *                  but adjusts the global 'block' pointer so the changes take
 *                  effect immediately.
 *         return : true if successful, false otherwise.
 *            see : Comments in the eeprom_commit_changes() synopsis.
 *           note : The call to eeprom_update() marks the temp_block as
 *                  inactive.
 */
bool eeprom_commit_changes_and_update(void)
{
  bool result = eeprom_commit_changes();

  if (result)
  {
    result = eeprom_update_block_pointer();
  }

  return result;
} /* eeprom_commit_changes_and_update */

/* -----------------------------------------------------------------------------
 *       synopsis : Updates the global eeprom 'block' pointer. This is used by
 *                  the manufacturing software as an alternative to resetting
 *                  the device between writes.
 *            see : main.cpp
 */
bool eeprom_update_block_pointer(void)
{

  bool result = true;
/*
  eeprom_block* ebp = eeprom_get_current_active_block();

  if (NULL != ebp)
  {
    block = ebp;  // 'block' is the global pointer to eeprom emulation
  }
  else
  {
    result = false;
  }
*/
  return result;

} /* update_block_pointer */

/* -----------------------------------------------------------------------------
 *       synopsis : Copy the current_active_block to the temp_block. This is
 *                  useful for updating the eeprom. If there are data in the
 *                  abc_sample_data array then up to the last
 *                  ABC_MAX_SAMPLE_COUNT will be copied to the end of the
 *                  temp_block copy.
 *     param [in] : copy_abc_memory - determines whether the ABC data in flash
 *                  is copied along with the configuration data or not.
 *                  true means the ABC data is copied, false means the ABC data
 *                  is cleared.
 *                  In most cases it will be copied but the calibration routines
 *                  require that it not be. Also, turning ABC logic off will
 *                  clear the data.
 *         return : pointer to the copy of the current active block or NULL if a
 *                  current active block does not exist.
 *           note : This call uses a statically allocated block of memory (see
 *                  temp_block[] in this module) as the destination.
 */
eeprom_block* eeprom_create_active_block_copy(bool copy_abc_memory)
{
  eeprom_block* src_block = eeprom_get_current_active_block();
//  uint16_t count;

  /* initialize the temp_block */
/*
  for (uint16_t i=0; i<TEMP_BLOCK_SIZE; i++)
  {
    temp_block[i] = (uint32_t)-1;
  }
*/
  if (NULL != src_block)
  {
    uint32_t* src = (uint32_t*)src_block;
    uint32_t* copy = (uint32_t*)temp_block;

    /* Note: the -1 on the terminal count takes into account the abc_sample_data
     * field, which is the last element in the data structure. Don't want to
     * copy that' */
    for (uint16_t index=0; index<SIZEOF_EEPROM_BLOCK/sizeof(uint32_t)-1; index++)
    {
      *copy++ = *src++;
    }

    if (true==copy_abc_memory)
    {
      /* move the abc_sample_data data to the temp block */
      copy = &((eeprom_block*)temp_block)->abc_sample_data;

      /* get a pointer to the abc_sample_data, first element */
      src = &src_block->abc_sample_data;

      /* Get a count of the elements in the abc_sample_data array.
       * Note that count will return -1 if the global block pointer is NULL or the
       * flash page which currently represents the current_active_block is full.
       * If we get this far we know the global block pointer can't be NULL so the
       * flash page must be full. Take care of that situation in the else
       * statement.
       */
/*
      if (abc_logic_get_count(&count))
      {
*/
        /* more than ABC_MAX_SAMPLE_COUNT elements, just take the last
         * ABC_MAX_SAMPLE_COUNT elements */
 /*
       if (count > ABC_MAX_SAMPLE_COUNT)
        {
          count -= ABC_MAX_SAMPLE_COUNT;
          src += count;
          for (uint16_t i=0; i<ABC_MAX_SAMPLE_COUNT; i++)
          {
            *copy++ = *src++;
          }
        }
        else if (count > 0)
        {
          for (uint16_t i=0; i<count; i++)
          {
            *copy++ = *src++;
          }
        }
      }
      else  // the flash is full, so get to the end to get the data
      {
        src += ABC_MAX_ARRAY_COUNT - ABC_MAX_SAMPLE_COUNT;
        for (uint16_t i=0; i<ABC_MAX_SAMPLE_COUNT; i++)
        {
          *copy++ = *src++;
        }
      }
*/
    } // end - copy_abc_memory == true

    /* update the return value */
    src_block = (eeprom_block*)temp_block;
  }

  return src_block;
} /* eeprom_create_active_block_copy */

/* -----------------------------------------------------------------------------
 *       synopsis : Create a new active block in temporary data storage.
 *            see : eeprom.h for definitions of the default values used below.
 */
/*
eeprom_block* eeprom_create_initial_active_block(void)
{
  eeprom_block* ebp = (eeprom_block*)((void*)temp_block);

  ebp->block_marker                 = ACTIVE_BLOCK_MARKER;
  ebp->update_count                 = DEFAULT_UPDATE_COUNT;
  ebp->model_number                 = DEFAULT_MODEL_NUMBER;
  ebp->mfg_data                     = DEFAULT_MFG_DATA;
  ebp->serial_number.ui.hi          = (uint16_t)((DEFAULT_SERIAL_NUMBER & 0xFFFF0000) >> 16);
  ebp->serial_number.ui.lo          = (uint16_t)(DEFAULT_SERIAL_NUMBER & 0x0000FFFF);
  ebp->ops_flag                     = DEFAULT_OPS_FLAG;
  ebp->slave_address                = DEFAULT_SLAVE_ADDRESS;
  ebp->baud_rate                    = DEFAULT_BAUD;
  ebp->parity                       = DEFAULT_PARITY;
  ebp->sample_time                  = DEFAULT_SAMPLE_TIME;
  ebp->nadir_time                   = DEFAULT_NADIR_TIME;
  ebp->zenith_time                  = DEFAULT_ZENITH_TIME;
  ebp->cal_samples                  = DEFAULT_CAL_SAMPLES;
  ebp->temp_divisor.re              = DEFAULT_TEMP_DIVISOR;
  ebp->co2_bound_limit_hi           = DEFAULT_CO2_BOUND_LIMIT_HI;
  ebp->co2_bound_limit_lo           = DEFAULT_CO2_BOUND_LIMIT_LO;
  ebp->zero_cal_target_ppm          = DEFAULT_ZERO_CAL_TARGET_PPM;
  ebp->zero_cal_result.re           = DEFAULT_ZERO_CAL_RESULT;
  ebp->span1_cal_target_ppm         = DEFAULT_SPAN1_CAL_TARGET_PPM;
  ebp->span1_cal_result.re          = DEFAULT_SPAN1_CAL_RESULT;
  ebp->span1_zero_ratio.re          = DEFAULT_SPAN1_ZERO_RATIO;
  ebp->span2_cal_target_ppm         = DEFAULT_SPAN2_CAL_TARGET_PPM;
  ebp->span2_cal_result.re          = DEFAULT_SPAN2_CAL_RESULT;
  ebp->span2_zero_ratio.re          = DEFAULT_SPAN2_ZERO_RATIO;
  ebp->sngpt_cal_target_ppm         = DEFAULT_SNGPT_CAL_TARGET_PPM;
  ebp->tcor_factor.re               = DEFAULT_TCOR_FACTOR;
  ebp->tcor1_coeff[0].re            = DEFAULT_TCOR1_COEFF_X0;
  ebp->tcor1_coeff[1].re            = DEFAULT_TCOR1_COEFF_X1;
  ebp->tcor1_coeff[2].re            = DEFAULT_TCOR1_COEFF_X2;
  ebp->tcor1_coeff[3].re            = DEFAULT_TCOR1_COEFF_X3;
  ebp->tcor2_coeff[0].re            = DEFAULT_TCOR2_COEFF_X0;
  ebp->tcor2_coeff[1].re            = DEFAULT_TCOR2_COEFF_X1;
  ebp->tcor2_coeff[2].re            = DEFAULT_TCOR2_COEFF_X2;
  ebp->tcor2_coeff[3].re            = DEFAULT_TCOR2_COEFF_X3;
  ebp->tcor3_coeff[0].re            = DEFAULT_TCOR3_COEFF_X0;
  ebp->tcor3_coeff[1].re            = DEFAULT_TCOR3_COEFF_X1;
  ebp->tcor3_coeff[2].re            = DEFAULT_TCOR3_COEFF_X2;
  ebp->tcor3_coeff[3].re            = DEFAULT_TCOR3_COEFF_X3;
  ebp->gas1_ppm_coeff[0].re         = DEFAULT_GAS1_PPM_COEFF_X0;
  ebp->gas1_ppm_coeff[1].re         = DEFAULT_GAS1_PPM_COEFF_X1;
  ebp->gas1_ppm_coeff[2].re         = DEFAULT_GAS1_PPM_COEFF_X2;
  ebp->gas1_ppm_coeff[3].re         = DEFAULT_GAS1_PPM_COEFF_X3;
  ebp->gas2_ppm_coeff[0].re         = DEFAULT_GAS2_PPM_COEFF_X0;
  ebp->gas2_ppm_coeff[1].re         = DEFAULT_GAS2_PPM_COEFF_X1;
  ebp->gas2_ppm_coeff[2].re         = DEFAULT_GAS2_PPM_COEFF_X2;
  ebp->gas2_ppm_coeff[3].re         = DEFAULT_GAS2_PPM_COEFF_X3;
  ebp->gas2_ppm_coeff[4].re         = DEFAULT_GAS2_PPM_COEFF_X4;
  ebp->gas2_ppm_coeff[5].re         = DEFAULT_GAS2_PPM_COEFF_X5;
  ebp->gas2_ppm_coeff[6].re         = DEFAULT_GAS2_PPM_COEFF_X6;
  ebp->gas2_ppm_coeff[7].re         = DEFAULT_GAS2_PPM_COEFF_X7;
  ebp->lamp_data[0].time            = DEFAULT_LAMP_DATA_TIME_0;
  ebp->lamp_data[0].volt            = DEFAULT_LAMP_DATA_VOLT_0;
  ebp->lamp_data[1].time            = DEFAULT_LAMP_DATA_TIME_1;
  ebp->lamp_data[1].volt            = DEFAULT_LAMP_DATA_VOLT_1;
  ebp->lamp_data[2].time            = DEFAULT_LAMP_DATA_TIME_2;
  ebp->lamp_data[2].volt            = DEFAULT_LAMP_DATA_VOLT_2;
  ebp->abc_correction_factor.re     = DEFAULT_ABC_CORRECTION_FACTOR;
  ebp->abc_sample_rate              = DEFAULT_ABC_SAMPLE_RATE;
  ebp->abc_eval_count               = DEFAULT_ABC_EVAL_COUNT;
  ebp->abc_sample_count             = DEFAULT_ABC_SAMPLE_COUNT;
  ebp->dsp_alpha_coeff.re           = DEFAULT_DSP_ALPHA_COEFF;
  ebp->dsp_adapt_coeff.re           = DEFAULT_DSP_ADAPT_COEFF;
  ebp->dsp_adapt_bound.re           = DEFAULT_DSP_ADAPT_BOUND;
  ebp->gas_ppm_upper_bound_1        = DEFAULT_GAS_PPM_UPPER_BOUND_1;
  ebp->gas_ppm_upper_bound_2        = DEFAULT_GAS_PPM_UPPER_BOUND_2;
  ebp->altitude.re                  = DEFAULT_ALTITUDE;
  ebp->cal_altitude.re              = DEFAULT_CAL_ALTITUDE;
  ebp->pressure.re                  = DEFAULT_PRESSURE;
  ebp->first_sample_cf.re           = DEFAULT_FIRST_SAMPLE_CF;
  ebp->warm_up_time                 = DEFAULT_WARM_UP_TIME;
  ebp->avg_ctrl                     = DEFAULT_AVG_CTRL;
*/
  /* calculate the CRC for the eeprom block. */
/*
  ebp->crc =
    crc16((uint8_t*)temp_block, (uint16_t)eeprom_calculate_crc_offset(ebp));
*/
  /* this is probably redundant but make sure that the first ABC sample data
   * location defaults to (uint32_t)-1. This is required by the search
   * algorithm. */
/*  ebp->abc_sample_data              = 0xFFFFFFFF;

  return ebp;
} *//* eeprom_create_initial_active_block */

/* -----------------------------------------------------------------------------
 *       synopsis : Returns the current active (eeprom) block. Creates a new one
 *                  if necessary.
 *         return : The region in flash memory that represents the current
 *                  eeprom emulation memory.
 */
eeprom_block* eeprom_get_current_active_block(void)
{
  uint32_t* ebp;

  /* look for an active block first. Call iterates through all available
     blocks. */
  if (eeprom_find_active_block(&ebp))
  {
    current_active_block = (eeprom_block*)((void*)ebp);
  }
  else /* none found, create one */
  {
    /* get first block */
//    ebp = eeprom_block_pointer_array[0];

    /* erase if not all 1's, ignore error */
/*
		if (EE_OK != flash_read_ones_section((uint32_t)ebp, EE_SECTOR_SIZE/4))
    {
      (void)flash_erase_flash_sector((uint32_t)ebp);
    }
*/
    eeprom_create_initial_active_block();
    int32_t size = SIZEOF_EEPROM_BLOCK;
/*
    if (EE_OK==
        flash_program_longword((uint32_t)ebp, size, (uint32_t)temp_block))
    {
      bool good = true;
      for (int32_t i=0; i<size/4; i++)
      {
        uint32_t a = *((uint32_t*)ebp+i);
        uint32_t b = *((uint32_t*)temp_block+i);

        if (a != b)
        {
          good = false;
          break;
        }
      }

      if (good)
      {
        current_active_block = (eeprom_block*)((void*)ebp);
      }
      else
      {
        current_active_block = NULL;
      }
    }
    else
    {
      current_active_block = NULL;
    }
*/		
  }

  return current_active_block;
} /* eeprom_get_current_active_block */

/*lint -e{788} not all enum constants (e.g, 'CRC') are used. This is expected.*/
/* -----------------------------------------------------------------------------
 *       synopsis : Generic method to read fields from the eeprom emulation.
 *     param [in] : key - The specific field in the eeprom memory layout.
 *    param [out] : value - The value of the requested field.
 *         return : true if the requested field is available and the value
 *                  parameter has been updated. False otherwise.
 */
/*
uint8_t eeprom_read(const uint16_t key, uint16_t* value)
{
//  MCM_PLACR |= (MCM_PLACR_ESFC_MASK);
  uint8_t retval = 0;	//NO_ERROR;

  eeprom_block *ebp = 0;	// block;

  if (ebp != NULL)
  {
    switch (key)
    {
      case UPDATE_COUNT :             { *value = ebp->update_count; } break;
      case MODEL_NUMBER :             { *value = ebp->model_number; } break;
      case MFG_DATA :                 { *value = ebp->mfg_data; } break;
      case SERIAL_NUMBER_HI :         { *value = ebp->serial_number.ui.hi; } break;
      case SERIAL_NUMBER_LO :         { *value = ebp->serial_number.ui.lo; } break;
      case OPS_FLAG :                 { *value = ebp->ops_flag; } break;
      case SLAVE_ADDRESS :            { *value = ebp->slave_address; } break;
      case BAUD_RATE :                { *value = ebp->baud_rate; } break;
      case PARITY :                   { *value = ebp->parity; } break;
      case SAMPLE_TIME :              { *value = ebp->sample_time; } break;
      case NADIR_TIME :               { *value = ebp->nadir_time; } break;
      case ZENITH_TIME :              { *value = ebp->zenith_time; } break;
      case CAL_SAMPLES :              { *value = ebp->cal_samples; } break;
      case TEMP_DIVISOR_HI :          { *value = ebp->temp_divisor.ui.hi; } break;
      case TEMP_DIVISOR_LO :          { *value = ebp->temp_divisor.ui.lo; } break;
      case CO2_BOUND_LIMIT_HI :       { *value = ebp->co2_bound_limit_hi;  } break;
      case CO2_BOUND_LIMIT_LO :       { *value = ebp->co2_bound_limit_lo; } break;
      case ZERO_CAL_TARGET_PPM :      { *value = ebp->zero_cal_target_ppm; } break;
      case ZERO_CAL_RESULT_HI :       { *value = ebp->zero_cal_result.ui.hi; } break;
      case ZERO_CAL_RESULT_LO :       { *value = ebp->zero_cal_result.ui.lo; } break;
      case SPAN1_CAL_TARGET_PPM :     { *value = ebp->span1_cal_target_ppm; } break;
      case SPAN1_CAL_RESULT_HI :      { *value = ebp->span1_cal_result.ui.hi; } break;
      case SPAN1_CAL_RESULT_LO :      { *value = ebp->span1_cal_result.ui.lo; } break;
      case SPAN1_ZERO_RATIO_HI :      { *value = ebp->span1_zero_ratio.ui.hi; } break;
      case SPAN1_ZERO_RATIO_LO :      { *value = ebp->span1_zero_ratio.ui.lo; } break;
      case SPAN2_CAL_TARGET_PPM :     { *value = ebp->span2_cal_target_ppm; } break;
      case SPAN2_CAL_RESULT_HI :      { *value = ebp->span2_cal_result.ui.hi; } break;
      case SPAN2_CAL_RESULT_LO :      { *value = ebp->span2_cal_result.ui.lo; } break;
      case SPAN2_ZERO_RATIO_HI :      { *value = ebp->span2_zero_ratio.ui.hi; } break;
      case SPAN2_ZERO_RATIO_LO :      { *value = ebp->span2_zero_ratio.ui.lo; } break;
      case SNGPT_CAL_TARGET_PPM :     { *value = ebp->sngpt_cal_target_ppm; } break;
      case TCOR_FACTOR_HI :           { *value = ebp->tcor_factor.ui.hi; } break;
      case TCOR_FACTOR_LO :           { *value = ebp->tcor_factor.ui.lo; } break;
      case TCOR1_COEFF_X0_HI :        { *value = ebp->tcor1_coeff[0].ui.hi; } break;
      case TCOR1_COEFF_X0_LO :        { *value = ebp->tcor1_coeff[0].ui.lo; } break;
      case TCOR1_COEFF_X1_HI :        { *value = ebp->tcor1_coeff[1].ui.hi; } break;
      case TCOR1_COEFF_X1_LO :        { *value = ebp->tcor1_coeff[1].ui.lo; } break;
      case TCOR1_COEFF_X2_HI :        { *value = ebp->tcor1_coeff[2].ui.hi; } break;
      case TCOR1_COEFF_X2_LO :        { *value = ebp->tcor1_coeff[2].ui.lo; } break;
      case TCOR1_COEFF_X3_HI :        { *value = ebp->tcor1_coeff[3].ui.hi; } break;
      case TCOR1_COEFF_X3_LO :        { *value = ebp->tcor1_coeff[3].ui.lo; } break;
      case TCOR2_COEFF_X0_HI :        { *value = ebp->tcor2_coeff[0].ui.hi; } break;
      case TCOR2_COEFF_X0_LO :        { *value = ebp->tcor2_coeff[0].ui.lo; } break;
      case TCOR2_COEFF_X1_HI :        { *value = ebp->tcor2_coeff[1].ui.hi; } break;
      case TCOR2_COEFF_X1_LO :        { *value = ebp->tcor2_coeff[1].ui.lo; } break;
      case TCOR2_COEFF_X2_HI :        { *value = ebp->tcor2_coeff[2].ui.hi; } break;
      case TCOR2_COEFF_X2_LO :        { *value = ebp->tcor2_coeff[2].ui.lo; } break;
      case TCOR2_COEFF_X3_HI :        { *value = ebp->tcor2_coeff[3].ui.hi; } break;
      case TCOR2_COEFF_X3_LO :        { *value = ebp->tcor2_coeff[3].ui.lo; } break;
      case TCOR3_COEFF_X0_HI :        { *value = ebp->tcor3_coeff[0].ui.hi; } break;
      case TCOR3_COEFF_X0_LO :        { *value = ebp->tcor3_coeff[0].ui.lo; } break;
      case TCOR3_COEFF_X1_HI :        { *value = ebp->tcor3_coeff[1].ui.hi; } break;
      case TCOR3_COEFF_X1_LO :        { *value = ebp->tcor3_coeff[1].ui.lo; } break;
      case TCOR3_COEFF_X2_HI :        { *value = ebp->tcor3_coeff[2].ui.hi; } break;
      case TCOR3_COEFF_X2_LO :        { *value = ebp->tcor3_coeff[2].ui.lo; } break;
      case TCOR3_COEFF_X3_HI :        { *value = ebp->tcor3_coeff[3].ui.hi; } break;
      case TCOR3_COEFF_X3_LO :        { *value = ebp->tcor3_coeff[3].ui.lo; } break;
      case GAS1_PPM_COEFF_X0_HI :     { *value = ebp->gas1_ppm_coeff[0].ui.hi; } break;
      case GAS1_PPM_COEFF_X0_LO :     { *value = ebp->gas1_ppm_coeff[0].ui.lo; } break;
      case GAS1_PPM_COEFF_X1_HI :     { *value = ebp->gas1_ppm_coeff[1].ui.hi; } break;
      case GAS1_PPM_COEFF_X1_LO :     { *value = ebp->gas1_ppm_coeff[1].ui.lo; } break;
      case GAS1_PPM_COEFF_X2_HI :     { *value = ebp->gas1_ppm_coeff[2].ui.hi; } break;
      case GAS1_PPM_COEFF_X2_LO :     { *value = ebp->gas1_ppm_coeff[2].ui.lo; } break;
      case GAS1_PPM_COEFF_X3_HI :     { *value = ebp->gas1_ppm_coeff[3].ui.hi; } break;
      case GAS1_PPM_COEFF_X3_LO :     { *value = ebp->gas1_ppm_coeff[3].ui.lo; } break;
      case GAS2_PPM_COEFF_X0_HI :     { *value = ebp->gas2_ppm_coeff[0].ui.hi; } break;
      case GAS2_PPM_COEFF_X0_LO :     { *value = ebp->gas2_ppm_coeff[0].ui.lo; } break;
      case GAS2_PPM_COEFF_X1_HI :     { *value = ebp->gas2_ppm_coeff[1].ui.hi; } break;
      case GAS2_PPM_COEFF_X1_LO :     { *value = ebp->gas2_ppm_coeff[1].ui.lo; } break;
      case GAS2_PPM_COEFF_X2_HI :     { *value = ebp->gas2_ppm_coeff[2].ui.hi; } break;
      case GAS2_PPM_COEFF_X2_LO :     { *value = ebp->gas2_ppm_coeff[2].ui.lo; } break;
      case GAS2_PPM_COEFF_X3_HI :     { *value = ebp->gas2_ppm_coeff[3].ui.hi; } break;
      case GAS2_PPM_COEFF_X3_LO :     { *value = ebp->gas2_ppm_coeff[3].ui.lo; } break;
      case GAS2_PPM_COEFF_X4_HI :     { *value = ebp->gas2_ppm_coeff[4].ui.hi; } break;
      case GAS2_PPM_COEFF_X4_LO :     { *value = ebp->gas2_ppm_coeff[4].ui.lo; } break;
      case GAS2_PPM_COEFF_X5_HI :     { *value = ebp->gas2_ppm_coeff[5].ui.hi; } break;
      case GAS2_PPM_COEFF_X5_LO :     { *value = ebp->gas2_ppm_coeff[5].ui.lo; } break;
      case GAS2_PPM_COEFF_X6_HI :     { *value = ebp->gas2_ppm_coeff[6].ui.hi; } break;
      case GAS2_PPM_COEFF_X6_LO :     { *value = ebp->gas2_ppm_coeff[6].ui.lo; } break;
      case GAS2_PPM_COEFF_X7_HI :     { *value = ebp->gas2_ppm_coeff[7].ui.hi; } break;
      case GAS2_PPM_COEFF_X7_LO :     { *value = ebp->gas2_ppm_coeff[7].ui.lo; } break;
      case LAMP_DATA_TIME_0 :         { *value = ebp->lamp_data[0].time; } break;
      case LAMP_DATA_VOLT_0 :         { *value = ebp->lamp_data[0].volt; } break;
      case LAMP_DATA_TIME_1 :         { *value = ebp->lamp_data[1].time; } break;
      case LAMP_DATA_VOLT_1 :         { *value = ebp->lamp_data[1].volt; } break;
      case LAMP_DATA_TIME_2 :         { *value = ebp->lamp_data[2].time; } break;
      case LAMP_DATA_VOLT_2 :         { *value = ebp->lamp_data[2].volt; } break;
      case ABC_CORRECTION_FACTOR_HI : { *value = ebp->abc_correction_factor.ui.hi; } break;
      case ABC_CORRECTION_FACTOR_LO : { *value = ebp->abc_correction_factor.ui.lo; } break;
      case ABC_SAMPLE_RATE :          { *value = ebp->abc_sample_rate; } break;
      case ABC_EVAL_COUNT :           { *value = ebp->abc_eval_count; } break;
      case ABC_SAMPLE_COUNT :         { *value = ebp->abc_sample_count; } break;
      case DSP_ALPHA_COEFF_HI :       { *value = ebp->dsp_alpha_coeff.ui.hi; } break;
      case DSP_ALPHA_COEFF_LO :       { *value = ebp->dsp_alpha_coeff.ui.lo; } break;
      case DSP_ADAPT_COEFF_HI :       { *value = ebp->dsp_adapt_coeff.ui.hi; } break;
      case DSP_ADAPT_COEFF_LO :       { *value = ebp->dsp_adapt_coeff.ui.lo; } break;
      case DSP_ADAPT_BOUND_HI :       { *value = ebp->dsp_adapt_bound.ui.hi; } break;
      case DSP_ADAPT_BOUND_LO :       { *value = ebp->dsp_adapt_bound.ui.lo; } break;
      case GAS_PPM_UPPER_BOUND_1 :    { *value = ebp->gas_ppm_upper_bound_1; } break;
      case GAS_PPM_UPPER_BOUND_2 :    { *value = ebp->gas_ppm_upper_bound_2; } break;
      case ALTITUDE_HI :              { *value = ebp->altitude.ui.hi; } break;
      case ALTITUDE_LO :              { *value = ebp->altitude.ui.lo; } break;
      case CAL_ALTITUDE_HI :          { *value = ebp->cal_altitude.ui.hi; } break;
      case CAL_ALTITUDE_LO :          { *value = ebp->cal_altitude.ui.lo; } break;
      case PRESSURE_HI :              { *value = ebp->pressure.ui.hi; } break;
      case PRESSURE_LO :              { *value = ebp->pressure.ui.lo; } break;
      case WARM_UP_TIME :             { *value = ebp->warm_up_time; } break;
      case FIRST_SAMPLE_CF_HI :       { *value = ebp->first_sample_cf.ui.hi; } break;
      case FIRST_SAMPLE_CF_LO :       { *value = ebp->first_sample_cf.ui.lo; } break;
      case AVG_CTRL :                 { *value = ebp->avg_ctrl; } break;
//      default :                       { retval = SERVER_DEVICE_FAILURE; } break;
      default :                       { retval = 0; } break;
    } // end-switch 
  } // end-if (ebp... 
  else
  {
    retval = 0;	//SERVER_DEVICE_FAILURE;
  }
//  MCM_PLACR &= (~MCM_PLACR_ESFC_MASK);

  return retval;
}*/ /* eeprom_read */

/*lint -e{788} not all enum constants (e.g, 'CRC') are used. */
/* -----------------------------------------------------------------------------
 *       synopsis : All the updates to EEPROM are handled here. If the update is
 *                  successful then the local eeprom_block variable is updated.
 *     param [in] : key - Which field in the eeprom data structure to change.
 *     param [in] : value - The value to change the field to.
 *         return : true if the update was successful, false otherwise.
 *           note : The caller is responsible for ensuring that the function is
 *                  called with valid data.
 */
/*
uint8_t eeprom_write(const uint16_t key, uint16_t* value)
{
//  MCM_PLACR |= (MCM_PLACR_ESFC_MASK);
  uint8_t retval = 0;	//NO_ERROR;
  eeprom_block* ebp = (eeprom_block*)((void*)&temp_block);

  if ((ebp != NULL) && (ACTIVE_BLOCK_MARKER == *((uint32_t*)ebp)) && value != NULL)
  {
    switch (key)
    {
      case MODEL_NUMBER :             { ebp->model_number = *value; } break;
      case MFG_DATA :                 { ebp->mfg_data = *value; } break;
      case SERIAL_NUMBER_HI :         { ebp->serial_number.ui.hi = *value; } break;
      case SERIAL_NUMBER_LO :         { ebp->serial_number.ui.lo = *value; } break;
      case OPS_FLAG :                 { ebp->ops_flag = *value; } break;
      case SLAVE_ADDRESS :            { ebp->slave_address = *value; } break;
      case BAUD_RATE :                { ebp->baud_rate = *value; } break;
      case PARITY :                   { ebp->parity = *value; } break;
      case SAMPLE_TIME :              { ebp->sample_time = *value; } break;
      case NADIR_TIME :               { ebp->nadir_time = *value; } break;
      case ZENITH_TIME :              { ebp->zenith_time = *value; } break;
      case CAL_SAMPLES :              { ebp->cal_samples = *value; } break;
      case TEMP_DIVISOR_HI :          { ebp->temp_divisor.ui.hi = *value; } break;
      case TEMP_DIVISOR_LO :          { ebp->temp_divisor.ui.lo = *value; } break;
      case CO2_BOUND_LIMIT_HI :       { ebp->co2_bound_limit_hi = *value; } break;
      case CO2_BOUND_LIMIT_LO :       { ebp->co2_bound_limit_lo = *value; } break;
      case ZERO_CAL_TARGET_PPM :      { ebp->zero_cal_target_ppm = *value; } break;
      case ZERO_CAL_RESULT_HI :       { ebp->zero_cal_result.ui.hi = *value; } break;
      case ZERO_CAL_RESULT_LO :       { ebp->zero_cal_result.ui.lo = *value; } break;
      case SPAN1_CAL_TARGET_PPM :     { ebp->span1_cal_target_ppm = *value; } break;
      case SPAN1_CAL_RESULT_HI :      { ebp->span1_cal_result.ui.hi = *value; } break;
      case SPAN1_CAL_RESULT_LO :      { ebp->span1_cal_result.ui.lo = *value; } break;
      case SPAN1_ZERO_RATIO_HI :      { ebp->span1_zero_ratio.ui.hi = *value; } break;
      case SPAN1_ZERO_RATIO_LO :      { ebp->span1_zero_ratio.ui.lo = *value; } break;
      case SPAN2_CAL_TARGET_PPM :     { ebp->span2_cal_target_ppm = *value; } break;
      case SPAN2_CAL_RESULT_HI :      { ebp->span2_cal_result.ui.hi = *value; } break;
      case SPAN2_CAL_RESULT_LO :      { ebp->span2_cal_result.ui.lo = *value; } break;
      case SPAN2_ZERO_RATIO_HI :      { ebp->span2_zero_ratio.ui.hi = *value; } break;
      case SPAN2_ZERO_RATIO_LO :      { ebp->span2_zero_ratio.ui.lo = *value; } break;
      case SNGPT_CAL_TARGET_PPM :     { ebp->sngpt_cal_target_ppm = *value; } break;
      case TCOR_FACTOR_HI :           { ebp->tcor_factor.ui.hi = *value; } break;
      case TCOR_FACTOR_LO :           { ebp->tcor_factor.ui.lo = *value; } break;
      case TCOR1_COEFF_X0_HI :        { ebp->tcor1_coeff[0].ui.hi = *value; } break;
      case TCOR1_COEFF_X0_LO :        { ebp->tcor1_coeff[0].ui.lo = *value; } break;
      case TCOR1_COEFF_X1_HI :        { ebp->tcor1_coeff[1].ui.hi = *value; } break;
      case TCOR1_COEFF_X1_LO :        { ebp->tcor1_coeff[1].ui.lo = *value; } break;
      case TCOR1_COEFF_X2_HI :        { ebp->tcor1_coeff[2].ui.hi = *value; } break;
      case TCOR1_COEFF_X2_LO :        { ebp->tcor1_coeff[2].ui.lo = *value; } break;
      case TCOR1_COEFF_X3_HI :        { ebp->tcor1_coeff[3].ui.hi = *value; } break;
      case TCOR1_COEFF_X3_LO :        { ebp->tcor1_coeff[3].ui.lo = *value; } break;
      case TCOR2_COEFF_X0_HI :        { ebp->tcor2_coeff[0].ui.hi = *value; } break;
      case TCOR2_COEFF_X0_LO :        { ebp->tcor2_coeff[0].ui.lo = *value; } break;
      case TCOR2_COEFF_X1_HI :        { ebp->tcor2_coeff[1].ui.hi = *value; } break;
      case TCOR2_COEFF_X1_LO :        { ebp->tcor2_coeff[1].ui.lo = *value; } break;
      case TCOR2_COEFF_X2_HI :        { ebp->tcor2_coeff[2].ui.hi = *value; } break;
      case TCOR2_COEFF_X2_LO :        { ebp->tcor2_coeff[2].ui.lo = *value; } break;
      case TCOR2_COEFF_X3_HI :        { ebp->tcor2_coeff[3].ui.hi = *value; } break;
      case TCOR2_COEFF_X3_LO :        { ebp->tcor2_coeff[3].ui.lo = *value; } break;
      case TCOR3_COEFF_X0_HI :        { ebp->tcor3_coeff[0].ui.hi = *value; } break;
      case TCOR3_COEFF_X0_LO :        { ebp->tcor3_coeff[0].ui.lo = *value; } break;
      case TCOR3_COEFF_X1_HI :        { ebp->tcor3_coeff[1].ui.hi = *value; } break;
      case TCOR3_COEFF_X1_LO :        { ebp->tcor3_coeff[1].ui.lo = *value; } break;
      case TCOR3_COEFF_X2_HI :        { ebp->tcor3_coeff[2].ui.hi = *value; } break;
      case TCOR3_COEFF_X2_LO :        { ebp->tcor3_coeff[2].ui.lo = *value; } break;
      case TCOR3_COEFF_X3_HI :        { ebp->tcor3_coeff[3].ui.hi = *value; } break;
      case TCOR3_COEFF_X3_LO :        { ebp->tcor3_coeff[3].ui.lo = *value; } break;
      case GAS1_PPM_COEFF_X0_HI :     { ebp->gas1_ppm_coeff[0].ui.hi = *value; } break;
      case GAS1_PPM_COEFF_X0_LO :     { ebp->gas1_ppm_coeff[0].ui.lo = *value; } break;
      case GAS1_PPM_COEFF_X1_HI :     { ebp->gas1_ppm_coeff[1].ui.hi = *value; } break;
      case GAS1_PPM_COEFF_X1_LO :     { ebp->gas1_ppm_coeff[1].ui.lo = *value; } break;
      case GAS1_PPM_COEFF_X2_HI :     { ebp->gas1_ppm_coeff[2].ui.hi = *value; } break;
      case GAS1_PPM_COEFF_X2_LO :     { ebp->gas1_ppm_coeff[2].ui.lo = *value; } break;
      case GAS1_PPM_COEFF_X3_HI :     { ebp->gas1_ppm_coeff[3].ui.hi = *value; } break;
      case GAS1_PPM_COEFF_X3_LO :     { ebp->gas1_ppm_coeff[3].ui.lo = *value; } break;
      case GAS2_PPM_COEFF_X0_HI :     { ebp->gas2_ppm_coeff[0].ui.hi = *value; } break;
      case GAS2_PPM_COEFF_X0_LO :     { ebp->gas2_ppm_coeff[0].ui.lo = *value; } break;
      case GAS2_PPM_COEFF_X1_HI :     { ebp->gas2_ppm_coeff[1].ui.hi = *value; } break;
      case GAS2_PPM_COEFF_X1_LO :     { ebp->gas2_ppm_coeff[1].ui.lo = *value; } break;
      case GAS2_PPM_COEFF_X2_HI :     { ebp->gas2_ppm_coeff[2].ui.hi = *value; } break;
      case GAS2_PPM_COEFF_X2_LO :     { ebp->gas2_ppm_coeff[2].ui.lo = *value; } break;
      case GAS2_PPM_COEFF_X3_HI :     { ebp->gas2_ppm_coeff[3].ui.hi = *value; } break;
      case GAS2_PPM_COEFF_X3_LO :     { ebp->gas2_ppm_coeff[3].ui.lo = *value; } break;
      case GAS2_PPM_COEFF_X4_HI :     { ebp->gas2_ppm_coeff[4].ui.hi = *value; } break;
      case GAS2_PPM_COEFF_X4_LO :     { ebp->gas2_ppm_coeff[4].ui.lo = *value; } break;
      case GAS2_PPM_COEFF_X5_HI :     { ebp->gas2_ppm_coeff[5].ui.hi = *value; } break;
      case GAS2_PPM_COEFF_X5_LO :     { ebp->gas2_ppm_coeff[5].ui.lo = *value; } break;
      case GAS2_PPM_COEFF_X6_HI :     { ebp->gas2_ppm_coeff[6].ui.hi = *value; } break;
      case GAS2_PPM_COEFF_X6_LO :     { ebp->gas2_ppm_coeff[6].ui.lo = *value; } break;
      case GAS2_PPM_COEFF_X7_HI :     { ebp->gas2_ppm_coeff[7].ui.hi = *value; } break;
      case GAS2_PPM_COEFF_X7_LO :     { ebp->gas2_ppm_coeff[7].ui.lo = *value; } break;
      case LAMP_DATA_TIME_0 :         { ebp->lamp_data[0].time = *value; } break;
      case LAMP_DATA_VOLT_0 :         { ebp->lamp_data[0].volt = *value; } break;
      case LAMP_DATA_TIME_1 :         { ebp->lamp_data[1].time = *value; } break;
      case LAMP_DATA_VOLT_1 :         { ebp->lamp_data[1].volt = *value; } break;
      case LAMP_DATA_TIME_2 :         { ebp->lamp_data[2].time = *value; } break;
      case LAMP_DATA_VOLT_2 :         { ebp->lamp_data[2].volt = *value; } break;
      case ABC_CORRECTION_FACTOR_HI : { ebp->abc_correction_factor.ui.hi = *value; } break;
      case ABC_CORRECTION_FACTOR_LO : { ebp->abc_correction_factor.ui.lo = *value; } break;
      case ABC_SAMPLE_RATE :          { ebp->abc_sample_rate = *value; } break;
      case ABC_EVAL_COUNT :           { ebp->abc_eval_count = *value; } break;
      case ABC_SAMPLE_COUNT :         { ebp->abc_sample_count = *value; } break;
      case DSP_ALPHA_COEFF_HI :       { ebp->dsp_alpha_coeff.ui.hi = *value; } break;
      case DSP_ALPHA_COEFF_LO :       { ebp->dsp_alpha_coeff.ui.lo = *value; } break;
      case DSP_ADAPT_COEFF_HI :       { ebp->dsp_adapt_coeff.ui.hi = *value; } break;
      case DSP_ADAPT_COEFF_LO :       { ebp->dsp_adapt_coeff.ui.lo = *value; } break;
      case DSP_ADAPT_BOUND_HI :       { ebp->dsp_adapt_bound.ui.hi = *value; } break;
      case DSP_ADAPT_BOUND_LO :       { ebp->dsp_adapt_bound.ui.lo = *value; } break;
      case GAS_PPM_UPPER_BOUND_1 :    { ebp->gas_ppm_upper_bound_1 = *value; } break;
      case GAS_PPM_UPPER_BOUND_2 :    { ebp->gas_ppm_upper_bound_2 = *value; } break;
      case ALTITUDE_HI :              { ebp->altitude.ui.hi = *value; } break;
      case ALTITUDE_LO :              { ebp->altitude.ui.lo = *value; } break;
      case CAL_ALTITUDE_HI :          { ebp->cal_altitude.ui.hi = *value; } break;
      case CAL_ALTITUDE_LO :          { ebp->cal_altitude.ui.lo = *value; } break;
      case PRESSURE_HI :              { ebp->pressure.ui.hi = *value; } break;
      case PRESSURE_LO :              { ebp->pressure.ui.lo = *value; } break;
      case WARM_UP_TIME :             { ebp->warm_up_time = *value; } break;
      case FIRST_SAMPLE_CF_HI :       { ebp->first_sample_cf.ui.hi = *value; } break;
      case FIRST_SAMPLE_CF_LO :       { ebp->first_sample_cf.ui.lo = *value; } break;
      case AVG_CTRL :                 { ebp->avg_ctrl = *value; } break;
//      default :                       { retval = SERVER_DEVICE_FAILURE; } break;
      default :                       { retval = 0; } break;
    } // end-switch 
  } // end-if 
  else
  {
    retval = 0;	//SERVER_DEVICE_FAILURE;
  }
//  MCM_PLACR &= (~MCM_PLACR_ESFC_MASK);

  return retval;
}*/ /* eeprom_write */

/* -----------------------------------------------------------------------------
 *       synopsis : Looks for the active data block in the eeprom emulation
 *                  flash.
 *    param [out] : active_block - pointer to the active data block, if one
 *                  found.
 *         return : true if an active data block was found, false otherwise.
 */
static bool eeprom_find_active_block(uint32_t** active_block)
{
  bool      block_found = false;
  uint32_t* ebp = NULL;

  /* look for active block */
/*
  for (uint16_t i=0; i<EE_BLOCKS; i++)
  {
    ebp = eeprom_block_pointer_array[i];
    block_found = (ACTIVE_BLOCK_MARKER == *ebp);

    if (block_found)
    {
      *active_block = ebp;
      break;
    }
  }
*/
  return block_found;
} /* eeprom_find_active_block */

/* -----------------------------------------------------------------------------
 *       synopsis : Looks for the active data block in the eeprom emulation
 *                  flash.
 *    param [out] : free_block - pointer to an inactive data block, if one
 *                  found.
 *         return : true if an active data block was found, false otherwise.
 */
static bool eeprom_find_free_block(uint32_t** free_block)
{
  bool      block_found = false;
  uint32_t* ebp = NULL;

  /* look for active block */
/*
  for (uint16_t i=0; i<EE_BLOCKS; i++)
  {
    ebp = eeprom_block_pointer_array[i];
    block_found = (*ebp != ACTIVE_BLOCK_MARKER);

    if (block_found)
    {
      *free_block = ebp;
      break;
    }
  }
*/
  return block_found;
} /* eeprom_find_free_block */

/* ----------------------------------------------------------------------------
 *       synopsis : Update the flash area that keeps the eeprom data with new
 *                  data.
 *     param [in] : source - a pointer to a new data block that will be used as
 *                  the source of the flash emulated eeprom.
 *         return : true if the update operation was successful, false
 *                  otherwise.
 *            see : eeprom_write (caller)
 */
static bool eeprom_update(eeprom_block* source)
{
  bool      result = false;
  uint32_t* free_block;
  uint32_t* active_block;

  if (eeprom_find_free_block(&free_block) && eeprom_find_active_block(&active_block))
  {
/*
    if (EE_ERR_MGSTAT0==flash_read_ones_section((uint32_t)((void*)free_block), TEMP_BLOCK_SIZE))
    {
      // erase the free (available) flash block
      result = (EE_OK==flash_erase_flash_sector((uint32_t)((void*)free_block)));
    }
    else
    {
      result = true;
    }
*/
    if (result)
    {
      /* program the new data (source) into the recently erased block */
/*
      result = (EE_OK==flash_program_longword((uint32_t)((void*)free_block),
                                              (SIZEOF_EEPROM_BLOCK-sizeof(uint32_t)),
                                              (uint32_t)((void*)source)));
*/
      /* if successful, add the ABC sample data if it exists */
      if (result)
      {
/*
        uint32_t* dst = &((eeprom_block*)free_block)->abc_sample_data;
        uint32_t* src = &((eeprom_block*)temp_block)->abc_sample_data;
        uint32_t  data = *src;
        uint16_t index = 0;

        while ((data != (uint32_t)-1) && (index<ABC_MAX_SAMPLE_COUNT) && (true==result))
        {
          result = (EE_OK==flash_program_longword((uint32_t)dst, 4, (uint32_t)&data));
          src++;
          data = *src;
          dst++;
          index++;
        }
*/
      }

      /* and if still successful, mark the original active block as inactive */
/*
			if (result)
      {
        source->block_marker = INACTIVE_BLOCK_MARKER;
        result =
          (EE_OK == flash_program_longword((uint32_t)((void*)active_block),
                                           FTFx_DWORD_SIZE,
                                           (uint32_t)((void*)source)));
      }
*/
    }
  }
  else
  {
    result = false;
  }

  /* if we successfully wrote to flash clear any errors associated with that */
/*
  if (result)
  {
    status.clear_device_status_bits(STATUS_EEPROM_BLOCK_ERROR);
  }
  else
  {
    status.set_device_status_bits(STATUS_EEPROM_BLOCK_ERROR);
  }
*/
  return result;
} /* eeprom_update */

