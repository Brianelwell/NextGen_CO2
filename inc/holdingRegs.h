/** @file       holdingRegs.h
 *  @brief      Functions that read/write to holding registers.
 *
 *              Read Holding Registers, MODBUS Function Code 03 (0x03)
 *
 *                This function code is used to read the contents of a
 *                contiguous block of holding registers in a remote device. The
 *                Request PDU specifies the starting register address and the
 *                number of registers. In the PDU Registers are addressed
 *                starting at zero.  Therefore registers numbered 1-16 are
 *                addressed as 0-15.
 *
 *                The register data in the response message are packed as two
 *                bytes per register, with the binary contents right justified
 *                within each byte. For each register, the first byte contains
 *                the high order bits and the second contains the low order
 *                bits.
 *
 *              Write Single Register, MODBUS Function Code 06 (0x06)
 *
 *                This function code is used to write a single holding register
 *                in a remote device. The Request PDU specifies the address of
 *                the register to be written. Registers are addressed starting
 *                at zero. Therefore register numbered 1 is addressed as 0. The
 *                normal response is an echo of the request, returned after the
 *                register contents have been written.
 *
 *              Write Multiple Registers, MODBUS Function Code 16 (0x10)
 *
 *                This function code is used to write a block of contiguous
 *                registers (1 to 123 registers) in a remote device.
 *
 *                The requested written values are specified in the request data
 *                field. Data is packed as two bytes per register.
 *
 *                The normal response returns the function code, starting
 *                address, and quantity of registers written.
 *
 *  @author     David S. Cooper (DSC)
 *  @copyright  Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *  @copyright  All rights reserved.
 *
 *  $Date: 2019-01-11 16:16:40 -0500 (Fri, 11 Jan 2019) $
 *  $Revision: 200 $
 */
#if !defined(HOLDINGREGS_H)
#define HOLDINGREGS_H

/** @brief Definition for the Read Holding Registers MODBUS function 6 (0x03). */
const uint8_t READ_HOLDING_REGISTERS = 3;

/** @brief Definition for the Write Single Register MODBUS function 6 (0x06). */
const uint8_t WRITE_SINGLE_REGISTER = 6;

/** @brief Definition for the Write Multiple Registers MODBUS Function 16
 * (0x10). */
const uint8_t WRITE_MULTIPLE_REGISTERS = 16;

/** @defgroup holding_registers_memory_map Holding Registers Memory Map
 *
 *  See @ref T67xx_base_address_definitions for the HOLDING_REGISTERS_OFFSET
 *  definition.
 *
 * @{ */

/** @brief Definitions for the MODBUS registers accessible with the Read Holding
 *  Registers Function (4, 0x04), Write Single Register Function 6 (0x06) and
 *  Write Multiple Registers MODBUS Function 16 (0x10). */
enum Holding_Registers
{
  MODEL_NUMBER = HOLDING_REGISTERS_OFFSET,   /**< MODEL_NUMBER    @ 4000  */
  MFG_DATA,                     /**< MFG_DATA                     @ 4001  */
  SERIAL_NUMBER_HI,             /**< SERIAL_NUMBER_HI             @ 4002  */
  SERIAL_NUMBER_LO,             /**< SERIAL_NUMBER_LO             @ 4003  */
  OPS_FLAG,                     /**< OPS_FLAG                     @ 4004  */
  SLAVE_ADDRESS,                /**< SLAVE_ADDRESS                @ 4005  */
  BAUD_RATE,                    /**< BAUD_RATE                    @ 4006  */
  PARITY,                       /**< PARITY                       @ 4007  */
  SAMPLE_TIME,                  /**< SAMPLE_TIME                  @ 4008  */
  NADIR_TIME,                   /**< NADIR_TIME                   @ 4009  */
  ZENITH_TIME,                  /**< ZENITH_TIME                  @ 4010  */
  CAL_SAMPLES,                  /**< N_CAL_SAMPLES                @ 4011  */
  TEMP_DIVISOR_HI,              /**< TEMP_DIVISOR_HI              @ 4012  */
  TEMP_DIVISOR_LO,              /**< TEMP_DIVISOR_LO              @ 4013  */
  CO2_BOUND_LIMIT_HI,           /**< CO2_BOUND_LIMIT_HI           @ 4014  */
  CO2_BOUND_LIMIT_LO,           /**< CO2_BOUND_LIMIT_LO           @ 4015  */
  ZERO_CAL_TARGET_PPM,          /**< ZERO_CAL_TARGET_PPM          @ 4016  */
  ZERO_CAL_RESULT_HI,           /**< ZERO_CAL_RESULT_HI           @ 4017  */
  ZERO_CAL_RESULT_LO,           /**< ZERO_CAL_RESULT_LO           @ 4018  */
  SPAN1_CAL_TARGET_PPM,         /**< SPAN1_CAL_TARGET_PPM         @ 4019  */
  SPAN1_CAL_RESULT_HI,          /**< SPAN1_CAL_RESULT_HI          @ 4020  */
  SPAN1_CAL_RESULT_LO,          /**< SPAN1_CAL_RESULT_LO          @ 4021  */
  SPAN1_ZERO_RATIO_HI,          /**< SPAN1_ZERO_RATIO_HI          @ 4022  */
  SPAN1_ZERO_RATIO_LO,          /**< SPAN1_ZERO_RATIO_LO          @ 4023  */
  SPAN2_CAL_TARGET_PPM,         /**< SPAN2_CAL_TARGET_PPM         @ 4024  */
  SPAN2_CAL_RESULT_HI,          /**< SPAN2_CAL_RESULT_HI          @ 4025  */
  SPAN2_CAL_RESULT_LO,          /**< SPAN2_CAL_RESULT_LO          @ 4026  */
  SPAN2_ZERO_RATIO_HI,          /**< SPAN2_ZERO_RATIO_HI          @ 4027  */
  SPAN2_ZERO_RATIO_LO,          /**< SPAN2_ZERO_RATIO_LO          @ 4028  */
  SNGPT_CAL_TARGET_PPM,         /**< SNGPT_CAL_TARGET_PPM         @ 4029  */
  TCOR_FACTOR_HI,               /**< TCOR_FACTOR_HI               @ 4030  */
  TCOR_FACTOR_LO,               /**< TCOR_FACTOR_LO               @ 4031  */
  TCOR1_COEFF_X0_HI,            /**< TCOR1_COEFF_X0_HI            @ 4032  */
  TCOR1_COEFF_X0_LO,            /**< TCOR1_COEFF_X0_LO            @ 4033  */
  TCOR1_COEFF_X1_HI,            /**< TCOR1_COEFF_X1_HI            @ 4034  */
  TCOR1_COEFF_X1_LO,            /**< TCOR1_COEFF_X1_LO            @ 4035  */
  TCOR1_COEFF_X2_HI,            /**< TCOR1_COEFF_X2_HI            @ 4036  */
  TCOR1_COEFF_X2_LO,            /**< TCOR1_COEFF_X2_LO            @ 4037  */
  TCOR1_COEFF_X3_HI,            /**< TCOR1_COEFF_X3_HI            @ 4038  */
  TCOR1_COEFF_X3_LO,            /**< TCOR1_COEFF_X3_LO            @ 4039  */
  TCOR2_COEFF_X0_HI,            /**< TCOR2_COEFF_X0_HI            @ 4040  */
  TCOR2_COEFF_X0_LO,            /**< TCOR2_COEFF_X0_LO            @ 4041  */
  TCOR2_COEFF_X1_HI,            /**< TCOR2_COEFF_X1_HI            @ 4042  */
  TCOR2_COEFF_X1_LO,            /**< TCOR2_COEFF_X1_LO            @ 4043  */
  TCOR2_COEFF_X2_HI,            /**< TCOR2_COEFF_X2_HI            @ 4044  */
  TCOR2_COEFF_X2_LO,            /**< TCOR2_COEFF_X2_LO            @ 4045  */
  TCOR2_COEFF_X3_HI,            /**< TCOR2_COEFF_X3_HI            @ 4046  */
  TCOR2_COEFF_X3_LO,            /**< TCOR2_COEFF_X3_LO            @ 4047  */
  TCOR3_COEFF_X0_HI,            /**< TCOR3_COEFF_X0_HI            @ 4048  */
  TCOR3_COEFF_X0_LO,            /**< TCOR3_COEFF_X0_LO            @ 4049  */
  TCOR3_COEFF_X1_HI,            /**< TCOR3_COEFF_X1_HI            @ 4050  */
  TCOR3_COEFF_X1_LO,            /**< TCOR3_COEFF_X1_LO            @ 4051  */
  TCOR3_COEFF_X2_HI,            /**< TCOR3_COEFF_X2_HI            @ 4052  */
  TCOR3_COEFF_X2_LO,            /**< TCOR3_COEFF_X2_LO            @ 4053  */
  TCOR3_COEFF_X3_HI,            /**< TCOR3_COEFF_X3_HI            @ 4054  */
  TCOR3_COEFF_X3_LO,            /**< TCOR3_COEFF_X3_LO            @ 4055  */
  GAS1_PPM_COEFF_X0_HI,         /**< GAS1_PPM_COEFF_X0_HI         @ 4056  */
  GAS1_PPM_COEFF_X0_LO,         /**< GAS1_PPM_COEFF_X0_LO         @ 4057  */
  GAS1_PPM_COEFF_X1_HI,         /**< GAS1_PPM_COEFF_X1_HI         @ 4058  */
  GAS1_PPM_COEFF_X1_LO,         /**< GAS1_PPM_COEFF_X1_LO         @ 4059  */
  GAS1_PPM_COEFF_X2_HI,         /**< GAS1_PPM_COEFF_X2_HI         @ 4060  */
  GAS1_PPM_COEFF_X2_LO,         /**< GAS1_PPM_COEFF_X2_LO         @ 4061  */
  GAS1_PPM_COEFF_X3_HI,         /**< GAS1_PPM_COEFF_X3_HI         @ 4062  */
  GAS1_PPM_COEFF_X3_LO,         /**< GAS1_PPM_COEFF_X3_LO         @ 4063  */
  GAS2_PPM_COEFF_X0_HI,         /**< GAS2_PPM_COEFF_X0_HI         @ 4064  */
  GAS2_PPM_COEFF_X0_LO,         /**< GAS2_PPM_COEFF_X0_LO         @ 4065  */
  GAS2_PPM_COEFF_X1_HI,         /**< GAS2_PPM_COEFF_X1_HI         @ 4066  */
  GAS2_PPM_COEFF_X1_LO,         /**< GAS2_PPM_COEFF_X1_LO         @ 4067  */
  GAS2_PPM_COEFF_X2_HI,         /**< GAS2_PPM_COEFF_X2_HI         @ 4068  */
  GAS2_PPM_COEFF_X2_LO,         /**< GAS2_PPM_COEFF_X2_LO         @ 4069  */
  GAS2_PPM_COEFF_X3_HI,         /**< GAS2_PPM_COEFF_X3_HI         @ 4070  */
  GAS2_PPM_COEFF_X3_LO,         /**< GAS2_PPM_COEFF_X3_LO         @ 4071  */
  GAS2_PPM_COEFF_X4_HI,         /**< GAS2_PPM_COEFF_X4_HI         @ 4072  */
  GAS2_PPM_COEFF_X4_LO,         /**< GAS2_PPM_COEFF_X4_LO         @ 4073  */
  GAS2_PPM_COEFF_X5_HI,         /**< GAS2_PPM_COEFF_X5_HI         @ 4074  */
  GAS2_PPM_COEFF_X5_LO,         /**< GAS2_PPM_COEFF_X5_LO         @ 4075  */
  GAS2_PPM_COEFF_X6_HI,         /**< GAS2_PPM_COEFF_X6_HI         @ 4076  */
  GAS2_PPM_COEFF_X6_LO,         /**< GAS2_PPM_COEFF_X6_LO         @ 4077  */
  GAS2_PPM_COEFF_X7_HI,         /**< GAS2_PPM_COEFF_X7_HI         @ 4078  */
  GAS2_PPM_COEFF_X7_LO,         /**< GAS2_PPM_COEFF_X7_LO         @ 4079  */
  LAMP_DATA_TIME_0,             /**< LAMP_DATA_TIME_0             @ 4080  */
  LAMP_DATA_VOLT_0,             /**< LAMP_DATA_VOLT_0             @ 4081  */
  LAMP_DATA_TIME_1,             /**< LAMP_DATA_TIME_1             @ 4082  */
  LAMP_DATA_VOLT_1,             /**< LAMP_DATA_VOLT_1             @ 4083  */
  LAMP_DATA_TIME_2,             /**< LAMP_DATA_TIME_2             @ 4084  */
  LAMP_DATA_VOLT_2,             /**< LAMP_DATA_VOLT_2             @ 4085  */
  ABC_CORRECTION_FACTOR_HI,     /**< ABC_CORRECTION_FACTOR_HI     @ 4086  */
  ABC_CORRECTION_FACTOR_LO,     /**< ABC_CORRECTION_FACTOR_LO     @ 4087  */
  ABC_SAMPLE_RATE,              /**< ABC_SAMPLE_RATE              @ 4088  */
  ABC_EVAL_COUNT,               /**< ABC_EVAL_COUNT               @ 4089  */
  ABC_SAMPLE_COUNT,             /**< ABC_SAMPLE_COUNT             @ 4090  */
  DSP_ALPHA_COEFF_HI,           /**< DSP_ALPHA_COEFF_HI           @ 4091  */
  DSP_ALPHA_COEFF_LO,           /**< DSP_ALPHA_COEFF_LO           @ 4092  */
  DSP_ADAPT_COEFF_HI,           /**< DSP_ADAPT_COEFF_HI           @ 4093  */
  DSP_ADAPT_COEFF_LO,           /**< DSP_ADAPT_COEFF_LO           @ 4094  */
  DSP_ADAPT_BOUND_HI,           /**< DSP_ADAPT_BOUND_HI           @ 4095  */
  DSP_ADAPT_BOUND_LO,           /**< DSP_ADAPT_BOUND_LO           @ 4096  */
  GAS_PPM_UPPER_BOUND_1,        /**< GAS_PPM_UPPER_BOUND_1        @ 4097  */
  GAS_PPM_UPPER_BOUND_2,        /**< GAS_PPM_UPPER_BOUND_2        @ 4098  */
  ALTITUDE_HI,                  /**< ALTITUDE_HI                  @ 4099  */
  ALTITUDE_LO,                  /**< ALTITUDE_LO                  @ 4100  */
  CAL_ALTITUDE_HI,              /**< CAL_ALTITUDE_HI              @ 4101  */
  CAL_ALTITUDE_LO,              /**< CAL_ALTITUDE_LO              @ 4102  */
  PRESSURE_HI,                  /**< PRESSURE_HI                  @ 4103  */
  PRESSURE_LO,                  /**< PRESSURE_LO                  @ 4104  */
  WARM_UP_TIME,                 /**< WARM_UP_TIME                 @ 4105  */
  DAC_CTRL,                     /**< DAC_CTRL                     @ 4106  */
  FIRST_SAMPLE_CF_HI,           /**< FIRST_SAMPLE_CF_HI           @ 4107  */
  FIRST_SAMPLE_CF_LO,           /**< FIRST_SAMPLE_CF_LO           @ 4108  */
  // Akash for averaging
  AVG_CTRL,                      /**< AVG_CTRL                    @ 4109  */
  END_HOLDING_REGISTERS         /*                                @ 4109  */
};

/** @} */

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief          MODBUS request processing - MODBUS Function 0x03 - Read
 *                  Holding Registers.
 *  @return         NO_ERROR (0) if there was no error in processing the
 *                  request, else the relevant exception code is returned.
 *  @see            MODBUS Application Protocol Specification V1.1b
 */
uint8_t read_holding_registers(void);

/**
 *  @brief          MODBUS request processing - MODBUS Function 0x06 - Write
 *                  Single (Holding) Register.
 *  @return         NO_ERROR (0) if there was no error in processing the
 *                  request, else the relevant exception code is returned.
 *  @see            MODBUS Application Protocol Specification V1.1b
 */
uint8_t write_single_register(void);


/**
 *  @brief  Write Multiple (Holding) Registers, MODBUS Function Code 16 (0x10).
 *  This function is used to write a block of contiguous registers (1 to 123
 *  registers) to the device. The requested written values are specified in the
 *  request data field. Data is packed as two bytes per register.
 *  @return NO_ERROR (0) if there was no error in processing the request, else
 *  the relevant exception code is returned.
 *  @see MODBUS Application Protocol Specification V1.1b
 */
uint8_t write_multiple_registers(void);

#ifdef __cplusplus
}
#endif

#endif /* HOLDINGREGS_H */

