/** @file           eeprom.h
 *  @brief          Functions and data structures associated with EEPROM
 *                  emulation.
 *
 *  @author         David S. Cooper
 *  @copyright      Copyright (c) 2014-2018 by Telaire Products, Goleta CA
 *  @copyright      All rights reserved.
 *
 *  $Date: 2019-01-18 14:54:47 -0500 (Fri, 18 Jan 2019) $
 *  $Revision: 202 $
 */
#if !defined(EEPROM_H)
#define EEPROM_H

#define FLASH_COMMIT_TIMEOUT		3			//  Keep Here ???

/** @brief          Used to emulate a 32-bit quantity, i.e., two unsigned
 *                  integers or a float. */
union reg2
{
  struct
  {
    uint16_t  lo;   /**< low unsigned short comes first in ARM little-endian */
    uint16_t  hi;   /**< high unsigned short follows in memory. */
  } ui;             /**< anonymous structure to hold 16 or 32-bit entities. */
  float     re;     /**< floats are 32-bits in this architecture. */
};

/** @brief          Data structure used to aggregate the n-tuple pair that
 *                  control the lamp turn on characteristics. */
typedef struct lamp_ctrl_tag
{
  uint16_t    time; /**< number of ticks to wait at this voltage. */
  uint16_t    volt; /**< level to set the lamp driver to. */
} lamp_ctrl;

/** @brief          Defines the array size, and therefore the number of steps
 *                  that the lamp on function in the co2 measurement task is. */
const uint16_t  LAMP_ON_STEPS = 3;

/** @brief          Defines the number of coefficients in the tcor1_coeff[]
 *                  array. */
const uint16_t  NUM_TCOR1_COEFF = 4;

/** @brief          Defines the number of coefficients in the tcor2_coeff[]
 *                  array. */
const uint16_t  NUM_TCOR2_COEFF = 4;

/** @brief          Defines the number of coefficients in the tcor3_coeff[]
 *                  array. */
const uint16_t  NUM_TCOR3_COEFF = 4;

/** @brief          Defines the number of coefficients in the gas1_ppm_coeff[]
 *                  array. */
const uint16_t  NUM_GAS1_PPM_COEFF = 4;

/** @brief          Defines the number of coefficients in the gas2_ppm_coeff[]
 *                  array. */
const uint16_t  NUM_GAS2_PPM_COEFF = 8;

/** @name           These are the default values for the eeprom emulation in
 *                  flash.
 *                  These values are used to populate the eeprom data structure.
 *  @see            eeprom_create_initial_active_block() in eeprom.cpp.
 *  @{ */
/*
const uint16_t  DEFAULT_UPDATE_COUNT = 1U;
const uint16_t  DEFAULT_MODEL_NUMBER = 6713;
const uint16_t  DEFAULT_MFG_DATA = 0xFFFFU;
const uint32_t  DEFAULT_SERIAL_NUMBER = 0xFFFFFFFFUL;
const uint16_t  DEFAULT_OPS_FLAG = 0xFFFFU;
const uint16_t  DEFAULT_SLAVE_ADDRESS = 21;

// Definitions for the serial port interface.
const uint16_t  NO_PARITY = 0;
const uint16_t  ODD_PARITY = 1;
const uint16_t  EVEN_PARITY = 2;
const uint16_t  DEFAULT_BAUD = 19200;
const uint16_t  DEFAULT_PARITY = EVEN_PARITY;

// Default hardware timing definitions.
const uint16_t  DEFAULT_SAMPLE_TIME = 500; // ticks between measurements
const uint16_t  DEFAULT_NADIR_TIME = 50;   // ticks from start of measurement
const uint16_t  DEFAULT_ZENITH_TIME = 5;   // ticks from nadir measurement

const uint16_t  DEFAULT_DAC_LAMP_ON_VOLTAGE = 130;
const float     DEFAULT_TEMP_DIVISOR = 30000.0f;

const uint16_t  DEFAULT_CO2_BOUND_LIMIT_HI = 5000U;
const uint16_t  DEFAULT_CO2_BOUND_LIMIT_LO = 400U;

// Default calibration variables.
const uint16_t  DEFAULT_CAL_SAMPLES = 64;  // number of calibration samples

// Zero calibration constants
const uint16_t  DEFAULT_ZERO_CAL_TARGET_PPM = 400;
const float     DEFAULT_ZERO_CAL_RESULT = 20000.0f;

// Span(1) calibration constants
const uint16_t  DEFAULT_SPAN1_CAL_TARGET_PPM = 980;
const float     DEFAULT_SPAN1_CAL_RESULT = 19620.0f;
const float     DEFAULT_SPAN1_ZERO_RATIO = 9.8100001e-1f;

// brief Span(2) calibration constants
const uint16_t  DEFAULT_SPAN2_CAL_TARGET_PPM = 4820;
const float     DEFAULT_SPAN2_CAL_RESULT = 19180.0f;
const float     DEFAULT_SPAN2_ZERO_RATIO = 9.5899999e-1f;

// Single-point calibration constants
const uint16_t  DEFAULT_SNGPT_CAL_TARGET_PPM = 500;

// Temperature correlation initial constant
const float     DEFAULT_TCOR_FACTOR = 1.0f;

// Default TCOR coefficients
const float     DEFAULT_TCOR1_COEFF_X0 =  1.261f;
const float     DEFAULT_TCOR1_COEFF_X1 = -0.3704f;
const float     DEFAULT_TCOR1_COEFF_X2 =  0.1158f;
const float     DEFAULT_TCOR1_COEFF_X3 = -0.006f;

const float     DEFAULT_TCOR2_COEFF_X0 =  1.261f;
const float     DEFAULT_TCOR2_COEFF_X1 = -0.3704f;
const float     DEFAULT_TCOR2_COEFF_X2 =  0.1158f;
const float     DEFAULT_TCOR2_COEFF_X3 = -0.006f;

const float     DEFAULT_TCOR3_COEFF_X0 =  1.261f;
const float     DEFAULT_TCOR3_COEFF_X1 = -0.3704f;
const float     DEFAULT_TCOR3_COEFF_X2 =  0.1158f;
const float     DEFAULT_TCOR3_COEFF_X3 = -0.006f;

// Default gas ppm polynomial coefficients
const float     DEFAULT_GAS1_PPM_COEFF_X0 = 400.0f;
const float     DEFAULT_GAS1_PPM_COEFF_X1 = 488.3849f;
const float     DEFAULT_GAS1_PPM_COEFF_X2 = 95.3974f;
const float     DEFAULT_GAS1_PPM_COEFF_X3 = 23.8040f;

const float     DEFAULT_GAS2_PPM_COEFF_X0 = 405.4495f;
const float     DEFAULT_GAS2_PPM_COEFF_X1 = 548.8259f;
const float     DEFAULT_GAS2_PPM_COEFF_X2 = 33.40278f;
const float     DEFAULT_GAS2_PPM_COEFF_X3 = 14.50033f;
const float     DEFAULT_GAS2_PPM_COEFF_X4 = 0.0f;
const float     DEFAULT_GAS2_PPM_COEFF_X5 = 0.0f;
const float     DEFAULT_GAS2_PPM_COEFF_X6 = 0.0f;
const float     DEFAULT_GAS2_PPM_COEFF_X7 = 0.0f;

// Default values for the lamp control array
const uint16_t  DEFAULT_LAMP_DATA_TIME_0 = 10;
const uint16_t  DEFAULT_LAMP_DATA_VOLT_0 = 2200;
const uint16_t  DEFAULT_LAMP_DATA_TIME_1 = 10;
const uint16_t  DEFAULT_LAMP_DATA_VOLT_1 = 1900;
const uint16_t  DEFAULT_LAMP_DATA_TIME_2 = 15;
const uint16_t  DEFAULT_LAMP_DATA_VOLT_2 = 1500;

// ABC Logic terms
const float     DEFAULT_ABC_CORRECTION_FACTOR = 1.0f;

// Default ABC Sample Rate is in hours.
const uint16_t  DEFAULT_ABC_SAMPLE_RATE = 4;       // sample every 4 hours
*/
/* Default ABC evaluation count, in data samples. In this (default) case if
 * there are 6 samples * 4 hr/sample = 24 hours worth of data in the flash it is
 * time to evaluate the ABC minimum based on, up to, the
 * DEFAULT_ABC_SAMPLE_COUNT samples. */

//const uint16_t  DEFAULT_ABC_EVAL_COUNT = 6;        // every 24 hours

/* Maximum number of samples used in the ABC correction factor. */
//const uint16_t  ABC_MAX_SAMPLE_COUNT = 42;         // 7 days

/* Default ABC Sample Count is the maximum number of samples looked at to
 * determine the ABC correction factor. For example, if a 7 day look back was
 * desired and the ABC Sample Rate was 4 hours/sample then 24/4 = 6 samples/day
 * times 7 days = 42 samples, times 14 days = 84. */
//const uint16_t  DEFAULT_ABC_SAMPLE_COUNT = ABC_MAX_SAMPLE_COUNT;

/* Default parameters for the power filter that is responsible for quieting the
 * signal. Collectively they are used in calculating norm_sig_avg_alpha. See
 * calculate_normalized_signal_average() in dsp.cpp */
/*
const float     DEFAULT_DSP_ALPHA_COEFF  = 1.9e-2f; // Advanced DSP coefficient
const float     DEFAULT_DSP_ADAPT_COEFF  = 1.9f;    // Adapting coefficient
const float     DEFAULT_DSP_ADAPT_BOUND  = 2.0e-1f; // Adapting bound

// Default upper bound for the PWM calculations
const uint16_t  DEFAULT_GAS_PPM_UPPER_BOUND_1 = 2000;
const uint16_t  DEFAULT_GAS_PPM_UPPER_BOUND_2 = 5000;

// Default altitude, in feet
const float     DEFAULT_ALTITUDE = 0.0f;

// Default calibration altitude, in feet
const float     DEFAULT_CAL_ALTITUDE = 0.0f;

// Default pressure. Units are unknown
const float     DEFAULT_PRESSURE = 3.8999999e-5f;
*/
/* Default warm-up time (in seconds). Note that the status register indicates
 * whether the device is currently in warm up mode or not. */
//const uint16_t  DEFAULT_WARM_UP_TIME = 8;

/* Default first sample correction factor. Used in the function
 * dsp_initialize_gas_ppm() to zero in on a good first sample ppm. */
//const float     DEFAULT_FIRST_SAMPLE_CF = 1.0f;

/* Default value for the averaging control */
//const uint16_t DEFAULT_AVG_CTRL = 0;

/** @} */

/** @brief          Persistent data. This is the layout for the data stored in
 *                  the Flash memory that masquerades as EEPROM.
 *
 * @note            The block_marker field is used to indicate the validity of
 *                  the block.
 *
 *                  Generally;
 *                    (a) EMPTY_BLOCK_MARKER would indicate that the block was
 *                        erased and available.
 *                    (b) ACTIVE_BLOCK_MARKER would indicate an active block and
 *                    (c) INACTIVE_BLOCK_MARKER would indicate an inactive block
 *                        that was previously active (needs to be erased)
 *
 * @note            IMPORTANT - Changes to this data structure require changes
 *                  to the function eeprom_create_initial_active_block() in
 *                  eeprom.cpp, and to a lesser extent init() in main.cpp.
 */
typedef struct eeprom_block_tag
{
  /** @brief        Block marker active or dirty. */
  uint32_t  block_marker;                                         // 0x00000000

  /** @brief        The number of flash updates. */
  uint16_t  update_count;                                         // 0x00000004

  /** @brief        Model number for device. Default is 6713. */
  uint16_t  model_number;                                         // 0x00000006

  /** @brief        Manufacturing data. This is dependent on the end-of-line
   *                testing. */
  uint16_t  mfg_data;                                             // 0x00000008

  /** @brief        Serial number to be programmed during manufacture */
  reg2      serial_number;                                        // 0x0000000C

  /** @brief        Configuration register (i.e., operations flag). */
  uint16_t  ops_flag;                                             // 0x00000010

  /** @brief        MODBUS slave address. */
  uint16_t  slave_address;                                        // 0x00000012

  /** @brief        The UART Baud rate. */
  uint16_t  baud_rate;                                            // 0x00000014

  /** @brief        The UART parity. */
  uint16_t  parity;                                               // 0x00000016

  /** @brief        Periodic sampling time in ticks. If ticks are 10ms (1/100)
   *                then to achieve a 5 second measurement period we have
   *                5/10E-3 = 500. */
  uint16_t  sample_time;                                          // 0x00000018

  /** @brief        Nadir & temp measurement, lamp on (in ticks). This is from
   *                the start of the measurement cycle which includes turning
   *                on the power to the ASIC. If ticks are 10ms (1/100) then to
   *                measure the nadir signal level 500ms after the ASIC is
   *                powered we have 0.5/10E-3 = 50. */
  uint16_t  nadir_time;                                           // 0x0000001A

  /** @brief        Zenith, input voltage, lamp off, power off (in ticks).
   *                This is from the end of the nadir measurement. If ticks are
   *                10ms (1/100) then to measure the zenith signal level 1.25s
   *                after the nadir measurement we have 1.25/10E-3 = 125. */
  uint16_t  zenith_time;                                          // 0x0000001C

  /** @brief        The number of calibration samples to take. */
  uint16_t  cal_samples;                                          // 0x0000001E

  /** @brief        This is used to normalize the temperature measurements. */
  reg2      temp_divisor;                                         // 0x00000020

  /* @brief         The upper CO2 limit for the bounded input register,
   *                 CO2_BOUND_VALUE. */
  uint16_t  co2_bound_limit_hi;                                   // 0x00000024

  /* @brief         The lower CO2 limit for the bounded input register,
   *                 CO2_BOUND_VALUE. */
  uint16_t  co2_bound_limit_lo;                                   // 0x00000026

  /** @brief        zero calibration target ppm (usually 400) */
  uint16_t  zero_cal_target_ppm;                                  // 0x00000028

  /** @brief        Calibration result derived from the zero gas calibration. */
  reg2      zero_cal_result;                                     // 0x0000002C

  /** @name         Span gas variables.
   *                These are modified by calibration routines. In general
   *                there are 3 variables associated with each span gas group;
   *                  1. target calibration ppm
   *                  2. calibration result
   *                  3. ratio between span and zero calibrations
   */
  uint16_t  span1_cal_target_ppm;                                 // 0x00000030
  reg2      span1_cal_result;                                     // 0x00000034
  reg2      span1_zero_ratio;                                     // 0x00000038
  uint16_t  span2_cal_target_ppm;                                 // 0x0000003C
  reg2      span2_cal_result;                                     // 0x00000040
  reg2      span2_zero_ratio;                                     // 0x00000044

  /** @brief        Single-point calibration target ppm. */
  uint16_t  sngpt_cal_target_ppm;                                 // 0x00000048

  /** @brief        Temperature correlation factor. */
  reg2      tcor_factor;                                          // 0x0000004C

  /** @name         TCOR polynomial coefficients.
   *                Note that these are interpreted as 32-bit floating point
   *                numbers (i.e., float).
   * @{ */
  reg2      tcor1_coeff[NUM_TCOR1_COEFF];                         // 0x00000050
  reg2      tcor2_coeff[NUM_TCOR2_COEFF];                         // 0x00000060
  reg2      tcor3_coeff[NUM_TCOR3_COEFF];                         // 0x00000070
  /** @} */

  /** @name         Gas ppm polynomial coefficients.
   *                Note that these are interpreted as 32-bit floating point
   *                numbers (i.e., float).
   *  @{ */
  reg2      gas1_ppm_coeff[NUM_GAS1_PPM_COEFF];                   // 0x00000080
  reg2      gas2_ppm_coeff[NUM_GAS2_PPM_COEFF];                   // 0x00000090
  /** @} */

  /** @brief        These are the values written to the DAC that drives the lamp
   *                circuitry at the associated times.
   * @see           lamp_ctrl data structure.*/
  lamp_ctrl lamp_data[LAMP_ON_STEPS];                             // 0x000000B0

  /** @brief        ABC logic correction factor. */
  reg2      abc_correction_factor;                                // 0x000000BC

  /** @brief        ABC sample rate (in samples/hour) */
  uint16_t  abc_sample_rate;                                      // 0x000000C0

  /** @brief        ABC logic evaluation count (in hours). Determines when to
   *                calculate the ABC correction factor. abc_sample_rate
   *                abc_eval_count = time to sample. */
   uint16_t abc_eval_count;                                       // 0x000000C2

  /** @brief        ABC logic sample count, for determining the ABC minimum.
   *                This is the maximum number of samples considered when
   *                determining the ABC minimum value. */
  uint16_t abc_sample_count;                                      // 0x000000C4

  /** @name         Power filter control.
   *                See calculate_normalized_signal_average() in dsp.cpp for an
   *                explanation of how the filter works and the purpose of these
   *                parameters.
   *  @{ */
  reg2      dsp_alpha_coeff;                                      // 0x000000C8
  reg2      dsp_adapt_coeff;                                      // 0x000000CC
  reg2      dsp_adapt_bound;                                      // 0x000000D0
  /** @} */

  /** @brief        This represents the upper bound for the gas measurement.
   *                It is used in the 'fast' PWM calculations. */
  uint16_t  gas_ppm_upper_bound_1;                                // 0x000000D4

  /** @brief        This represents the upper bound for the gas measurement.
   *                It is used in the 'slow' PWM calculations. */
  uint16_t  gas_ppm_upper_bound_2;                                // 0x000000D6

  /** @brief        Used in the ppm calculation. It represents the user
   *                adjustable altitude. */
  reg2      altitude;                                             // 0x000000D8

  /** @brief        Used in the ppm calculation. It represents the altitude at
   *                the calibration location. */
  reg2      cal_altitude;                                         // 0x000000DC

  /** @brief        Used in the ppm calculation. It represents the user
   *                adjustable pressure. */
  reg2      pressure;                                             // 0x000000E0

  /** @brief        The first sample correction factor.  Used in the function
   *                dsp_initialize_gas_ppm to zero in on a good first sample ppm.
   */
  reg2      first_sample_cf;                                      // 0x000000E4

  /** @brief        This is the warm-up time (in seconds). After this time the
   *                dsp filtering is reset. */
  uint16_t  warm_up_time;                                         // 0x000000E8

  /** @brief        This is the CRC for the preceding data elements. */
  uint16_t  crc;                                                  // 0x000000EA
  
  /** @brief        This is to control averaging filter in different cases. */
  uint16_t avg_ctrl;                                              // 0x000000EC

  /** @brief        Array of periodic ABC minimum samples, after the CRC because
   *                they get updated on the fly. Note that this is the first
   *                element of the ABC sample array, the rest follow this
   *                address to the end of the block. It is also important to
   *                know that the uint32_t type declaration forces this
   *                variable to be placed on a 32-bit boundary, which is
   *                important so the subsequent address are all contiguous.
   *                This element is initialized to -1 and is used by the search
   *                routine to delimit the array bounds.*/
  uint32_t  abc_sample_data;                                      // 0x000000EF
} eeprom_block;

/** @brief          This is the size of the eeprom_block. It includes only the
 *                  first sample of ABC data. Currently the size of the
 *                  eeprom_block is 240 (0xF0) bytes */
#define SIZEOF_EEPROM_BLOCK     sizeof(eeprom_block)

/** @brief          This is the size of a static buffer, temp_block, that is
 *                  defined in eeprom.cpp. It is used primarily as temporary
 *                  storage for the contents of the active block. It can be
 *                  used for other storage requirements as well and
 *                  circumvents the need to allocate storage in a small embedded
 *                  system.
 *
 *                  Note:
 *                  SIZEOF_EEPROM_BLOCK is on bytes and the array is on 32-bit
 *                  words and TEMP_BLOCK_SIZE represents 32-bit words.
 *                  If SIZEOF_EEPROM_BLOCK = 240 and ABC_MAX_SAMPLE_COUNT = 42,
 *                  then we have:
 *                    240/4 + 42 + 1 = 103 words (412 bytes)
 */
/*
const uint32_t TEMP_BLOCK_SIZE =
  SIZEOF_EEPROM_BLOCK/sizeof(uint32_t) + ABC_MAX_SAMPLE_COUNT + 1;
*/

/** @brief          This static buffer is defined in eeprom.cpp */
extern uint32_t temp_block[];

#ifdef __cplusplus
extern "C" {
#endif

/** @brief          Returns the offset to the CRC field in the eeprom_block data
 *                  structure.
 *  @param [in]     block
 *                    The block to examine.
 *  @return         The offset to the CRC field in the data structure.
 */
uint32_t eeprom_calculate_crc_offset(eeprom_block const* block);

/** @brief          Assumes that the temp block has the latest changes for the
 *                  eeprom emulation and writes the temp block to a next free
 *                  flash page used for emulation.
 *  @return         true if successful, false otherwise.
 */
bool eeprom_commit_changes(void);

/** @brief          Not only updates the flash block used for eeprom emulation
 *                  but adjusts the global 'block' pointer so the changes take
 *                  effect immediately.
 *  @return         true if successful, false otherwise.
 */
bool eeprom_commit_changes_and_update(void);

/** @brief          Get a copy of the current active memory block. This is
 *                  useful for updating the eeprom.
 *  @param [in]     copy_abc_memory
 *                    This parameter determines whether the ABC data in flash is
 *                    copied along with the configuration data or not.
 *                    <i>true</i> means the ABC data is copied, <i>false</i>
 *                    means the ABC data is cleared.
 *                    In most cases it will be copied but the zero and single
 *                    point calibration routines require that it not be.
 *                    Also, turning ABC logic off will clear the data.
 *  @return         A pointer to the copy of the current active block or NULL
 *                  if a current active block does not exist.
 */
eeprom_block* eeprom_create_active_block_copy(bool copy_abc_memory);

/** @brief          Create a new active block in temporary data storage. */
eeprom_block* eeprom_create_initial_active_block(void);

/** @brief          Returns the current active (eeprom) block. Creates a new one
 *                  if necessary.
 *  @return         The region in flash memory that represents the current
 *                  eeprom emulation memory.
 */
eeprom_block* eeprom_get_current_active_block(void);

/** @brief          Generic method to read fields from the EEPROM emulation.
 *  @param [in]     key
 *                    The specific field in the EEPROM memory layout.
 *  @param [out]    value
 *                    The value of the requested field.
 *  @return         true if the requested field is available and the value
 *                  parameter has been updated. False otherwise.
 */
uint8_t eeprom_read(const uint16_t key, uint16_t* value);

/** @brief          Updates the global eeprom 'block' pointer. This is used by
 *                  the manufacturing software as an alternative to resetting
 *                  the device between writes.
 */
bool eeprom_update_block_pointer(void);

/** @brief          All the updates to EEPROM are handled here. If the update is
 *                  successful then change the pointer in the main.c module that
 *                  amounts to a global variable: (eeprom_block*) block.
 *  @param [in]     key
 *                    Which field in the EEPROM data structure to change
 *  @param [in]     value
 *                    The value to change the field to
 *  @return         true if the update was successful, false otherwise.
 */
uint8_t eeprom_write(const uint16_t key, uint16_t* value);

#ifdef __cplusplus
}
#endif

#endif /* EEPROM_H */

