

#define FLASH_COMMIT_TIMEOUT				3			//  Keep Here ???
#define FLASH_NUMBER_OF_RECORDS			8			//  Number of Active Pages in 2K Section
#define FLASH_E2_BASE								0x08007800
#define FLASH_RECORD_ALLOCATION			0x100
#define FLASH_LAST_RECORD						0x08007f00
#define FLASH_KEY1									0x45670123
#define FLASH_KEY2									0xcdef89ab

bool FlashRestore(void);
bool FlashInitialize(void);
bool FlashErase(void);
bool FlashUnlock(void);
void FlashRestoreDefaults(void);
bool FlashCommit(void);
uint16_t FlashGetRecordSize(void);
uint16_t Flash_CRC(uint8_t *, uint16_t);

#define	DEFAULT_MODEL_NUMBER								6713u								// 4000
#define	DEFAULT_MFG_DATA										0xffff
#define	DEFAULT_SERIAL_NUMBER								0xffffffff
#define	DEFAULT_OPS_FLAG										0xff1f
#define	DEFAULT_SLAVE_ADDRESS								0x15
#define	DEFAULT_BAUD_RATE										19200u
#define	DEFAULT_PARITY											2u
#define	DEFAULT_SAMPLE_TIME									500u
#define	DEFAULT_NADIR_TIME									50u
#define	DEFAULT_ZENITH_TIME									5u									// 4010
#define	DEFAULT_CAL_SAMPLES									64u
#define	DEFAULT_TEMP_DIVISOR								30000.0f
#define	DEFAULT_CO2_BOUND_LIMIT_HI					5000u
#define	DEFAULT_CO2_BOUND_LIMIT_LO					400u
#define	DEFAULT_ZERO_CAL_TARGET_PPM					400u
#define	DEFAULT_ZERO_CAL_RESULT							20000.0f
#define	DEFAULT_SPAN1_CAL_TARGET_PPM				980u
#define	DEFAULT_SPAN1_CAL_RESULT						19620.0f						// 4020
#define	DEFAULT_SPAN1_ZERO_RATIO						0.98100001f
#define	DEFAULT_SPAN2_CAL_TARGET_PPM				4820u
#define	DEFAULT_SPAN2_CAL_RESULT						19180.0f
#define	DEFAULT_SPAN2_ZERO_RATIO						0.95899999f
#define	DEFAULT_SNGPT_CAL_TARGET_PPM				500u
#define	DEFAULT_TCOR_FACTOR									1.0f								// 4030
#define	DEFAULT_TCOR1_COEFF_X0							1.261f
#define	DEFAULT_TCOR1_COEFF_X1							-0.3704f
#define	DEFAULT_TCOR1_COEFF_X2							0.1158f
#define	DEFAULT_TCOR1_COEFF_X3							-0.006f
#define	DEFAULT_TCOR2_COEFF_X0							1.261f							// 4040
#define	DEFAULT_TCOR2_COEFF_X1							-0.3704f
#define	DEFAULT_TCOR2_COEFF_X2							0.1158f
#define	DEFAULT_TCOR2_COEFF_X3							-0.006f
#define	DEFAULT_TCOR3_COEFF_X0							1.261f
#define	DEFAULT_TCOR3_COEFF_X1							-0.3704f						// 4050
#define	DEFAULT_TCOR3_COEFF_X2							0.1158f
#define	DEFAULT_TCOR3_COEFF_X3							-0.006f
#define	DEFAULT_GAS1_PPM_COEFF_X0						400.0f
#define	DEFAULT_GAS1_PPM_COEFF_X1						488.3849f
#define	DEFAULT_GAS1_PPM_COEFF_X2						95.3974f						// 4060
#define	DEFAULT_GAS1_PPM_COEFF_X3						23.8040f
#define	DEFAULT_GAS2_PPM_COEFF_X0						405.4495f
#define	DEFAULT_GAS2_PPM_COEFF_X1						548.8259f
#define	DEFAULT_GAS2_PPM_COEFF_X2						33.40278f
#define	DEFAULT_GAS2_PPM_COEFF_X3						14.50033f						// 4070
#define	DEFAULT_GAS2_PPM_COEFF_X4						0.0f
#define	DEFAULT_GAS2_PPM_COEFF_X5						0.0f
#define	DEFAULT_GAS2_PPM_COEFF_X6						0.0f
#define	DEFAULT_GAS2_PPM_COEFF_X7						0.0f
#define	DEFAULT_LAMP_DATA_TIME_0						10u									// 4080
#define	DEFAULT_LAMP_DATA_VOLT_0						2200u
#define	DEFAULT_LAMP_DATA_TIME_1						10u
#define	DEFAULT_LAMP_DATA_VOLT_1						1900u
#define	DEFAULT_LAMP_DATA_TIME_2						15u
#define	DEFAULT_LAMP_DATA_VOLT_2						1500u
#define	DEFAULT_ABC_CORRELATION_FACTOR			1.0f
#define	DEFAULT_ABC_SAMPLE_RATE							4u
#define	DEFAULT_ABC_EVAL_COUNT							2u
#define	DEFAULT_ABC_SAMPLE_COUNT						42u									// 4090
#define	DEFAULT_DSP_ALPHA_COEFF							0.019f
#define	DEFAULT_DSP_ADAPT_COEFF							1.9f
#define	DEFAULT_DSP_ADAPT_BOUND							0.2f
#define	DEFAULT_GAS_UPPER_BOUND_1						2000u
#define	DEFAULT_GAS_UPPER_BOUND_2						5000u
#define	DEFAULT_ALTITUDE										0.0f								// 4100
#define	DEFAULT_CAL_ALTITUDE								0.0f
#define	DEFAULT_PRESSURE										0.000038999f
#define	DEFAULT_WARM_UP_TIME								8u
#define	DEFAULT_DAC_CTRL										0u
#define	DEFAULT_FIRST_SAMPLE_CF							1.0f
#define	DEFAULT_AVG_CTRL										0u									// 4109
