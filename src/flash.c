
#include "main.h"

bool FlashRestore(void)
{
	uint16_t i;
	uint16_t uiRecord, uiCRC;
	uint8_t *x;
	uint8_t *pCRC_Loc;
	uint8_t *pSource;
	uint8_t *pDestination;

	i = FlashGetRecordSize();
	uiRecord = 8u;
	for(x = (uint8_t*)FLASH_LAST_RECORD; x >= (uint8_t*)FLASH_E2_BASE; x -= FLASH_RECORD_ALLOCATION)
	{
		uiRecord --;
		pCRC_Loc = x + i;
		uiCRC = *pCRC_Loc << 8;
		pCRC_Loc ++;
		uiCRC += *pCRC_Loc;
		if(uiCRC != 0xffff)
		{
			uiCRC = Flash_CRC(x, i + 2);
			if(uiCRC != 0)
			{
				return false;
			}
			else
			{
				//  Copy Record to RAM
				pDestination = (uint8_t*)pHRegs;
				pSource = x;
				for(i = i; i > 0; i --)
				{
					*pDestination = *pSource;
					pDestination ++;
					pSource ++;
				}
				return true;
			}
		}
	}
	return false;			//  No Valid Record Found
}

uint16_t FlashGetRecordSize(void)
{
	uint16_t uiBytes, i;	
	uint8_t *pDataLoc;
	uint8_t *pTypeLoc;
	HRegTypes sHRegType;

	uiBytes = 0;															//  Initialize Offset Variables
	pDataLoc = (uint8_t*)pHRegs;
	pTypeLoc = (uint8_t*)&sHRegType;
	for(i = 0; i < QTY_HREGTYPES; i++)
	{
		if(*pTypeLoc == 1)
		{
			uiBytes += 2;
			pDataLoc += 2;
		}
		else
		{
			uiBytes += 4;
			if((uint64_t)pDataLoc % 4)
			{
				uiBytes += 2;
				pDataLoc += 2;
			}
			pDataLoc += 4;
		}
		pTypeLoc ++;
	}
	return uiBytes;
}

bool FlashErase(void)
{
	if(READ_BIT(FLASH->SR, FLASH_SR_BSY1) != 0)
	{
		return false;
	}

	FlashUnlock();
	SET_BIT(FLASH->CR, 0x7a);
	
	SET_BIT(FLASH->CR, FLASH_CR_STRT);
	
	while(READ_BIT(FLASH->SR, FLASH_SR_BSY1) == 1)
	{
		;
	}
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	CLEAR_BIT(FLASH->SR, FLASH_SR_PGSERR);
	SET_BIT(FLASH->CR, FLASH_CR_LOCK);
	
	return true;
}

bool FlashUnlock(void)
{
	bool bStatus = true;

	if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0)
	{
		WRITE_REG(FLASH->KEYR, FLASH_KEY1);
		WRITE_REG(FLASH->KEYR, FLASH_KEY2);
		
		if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0)
		{
			bStatus = false;
		}
	}
	return bStatus;
}

bool FlashInitialize(void)
{
	FlashErase();						//  First Ensure Sector is Blank
	FlashRestoreDefaults();

	return FlashCommit();
}

bool FlashCommit(void)
{
	uint16_t uiBytes, uiLines;
	uint16_t uiCRC, i;
	uint16_t uiRecord;
	uint64_t dwData;
	uint8_t *pDataLoc;
	uint32_t ulWriteLoc;
	bool bBlank;

	uiBytes = FlashGetRecordSize();					//  Initialize Record Parameters
	uiLines = uiBytes >> 3;
	if(uiBytes % 8)
	{
		uiLines ++;
	}

	//  Find First Blank Record and Verify Entire Record Blank
	bBlank = false;
	uiRecord = 0;
	while(bBlank == false)
	{
		pDataLoc = ((uint8_t*)FLASH_E2_BASE + (uiRecord * FLASH_RECORD_ALLOCATION));
		bBlank = true;
		for(i = 0; i < FLASH_RECORD_ALLOCATION; i++)
		{
			if(*(pDataLoc + i) != 0xff)
			{
				bBlank = false;
				i = FLASH_RECORD_ALLOCATION;	//  Force Exit from For Loop
			}
		}
		if(bBlank == false)
		{
			if(++uiRecord >= FLASH_NUMBER_OF_RECORDS)
			{
				uiRecord = 0;
				bBlank = FlashErase();
			}
		}
	}

	uiCRC = Flash_CRC((uint8_t*)pHRegs, uiBytes);
	
	//  Commit Sequence
	if(READ_BIT(FLASH->SR, FLASH_SR_BSY1) != 0)
	{
		return false;
	}
	FlashUnlock();
	SET_BIT(FLASH->SR, 0xc2fa);						//  Check for Errors and Clear if Needed

	pDataLoc = (uint8_t*)pHRegs;					//  Reinitialize Data Location
	ulWriteLoc = (uint32_t)(FLASH_E2_BASE + (uiRecord * FLASH_RECORD_ALLOCATION));
	
	for(i = 0; i < uiLines; i ++)
	{
		dwData = *(uint64_t*)pDataLoc;
		if(uiBytes < 8)
		{
			///// need better method of calculating offsets
			dwData += (uint64_t)((uiCRC >> 8) & 0xff) << 56;
			dwData += (uint64_t)(uiCRC & 0xff) << 48;
		}
		SET_BIT(FLASH->CR, FLASH_CR_PG);
		*(uint32_t*)ulWriteLoc = (uint32_t)dwData;
		*(uint32_t*)(ulWriteLoc + 4u)= (uint32_t)(dwData >> 32u);
		CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
		ulWriteLoc += 8;
		pDataLoc += 8;
		if(uiBytes >= 8)
		{
			uiBytes -= 8;
		}
	}
	SET_BIT(FLASH->CR, FLASH_CR_LOCK);			//  Lock Flash
	return true;
}

void FlashRestoreDefaults(void)
{
	sHRegs.model_number = DEFAULT_MODEL_NUMBER;										// 4000
	sHRegs.mfg_data = DEFAULT_MFG_DATA;
	sHRegs.serial_number = DEFAULT_SERIAL_NUMBER;
	sHRegs.ops_flag = DEFAULT_OPS_FLAG;
	sHRegs.slave_address = DEFAULT_SLAVE_ADDRESS;
	sHRegs.baud_rate = DEFAULT_BAUD_RATE;
	sHRegs.parity = DEFAULT_PARITY;
	sHRegs.sample_time = DEFAULT_SAMPLE_TIME;
	sHRegs.nadir_time = DEFAULT_NADIR_TIME;
	sHRegs.zenith_time = DEFAULT_ZENITH_TIME;											// 4010
	sHRegs.cal_samples = DEFAULT_CAL_SAMPLES;
	sHRegs.temp_divisor = DEFAULT_TEMP_DIVISOR;
	sHRegs.co2_bound_limit_hi = DEFAULT_CO2_BOUND_LIMIT_HI;
	sHRegs.co2_bound_limit_lo = DEFAULT_CO2_BOUND_LIMIT_LO;
	sHRegs.zero_cal_target_ppm = DEFAULT_ZERO_CAL_TARGET_PPM;
	sHRegs.zero_cal_result = DEFAULT_ZERO_CAL_RESULT;
	sHRegs.span1_cal_target_ppm = DEFAULT_SPAN1_CAL_TARGET_PPM;
	sHRegs.span1_cal_result = DEFAULT_SPAN1_CAL_RESULT;						// 4020
	sHRegs.span1_zero_ratio = DEFAULT_SPAN1_ZERO_RATIO;
	sHRegs.span2_cal_target_ppm = DEFAULT_SPAN2_CAL_TARGET_PPM;
	sHRegs.span2_cal_result = DEFAULT_SPAN2_CAL_RESULT;
	sHRegs.span2_zero_ratio = DEFAULT_SPAN2_ZERO_RATIO;
	sHRegs.sngpt_cal_target_ppm = DEFAULT_SNGPT_CAL_TARGET_PPM;
	sHRegs.tcor_factor = DEFAULT_TCOR_FACTOR;											// 4030
	sHRegs.tcor1_coeff_x0 = DEFAULT_TCOR1_COEFF_X0;
	sHRegs.tcor1_coeff_x1 = DEFAULT_TCOR1_COEFF_X1;
	sHRegs.tcor1_coeff_x2 = DEFAULT_TCOR1_COEFF_X2;
	sHRegs.tcor1_coeff_x3 = DEFAULT_TCOR1_COEFF_X3;
	sHRegs.tcor2_coeff_x0 = DEFAULT_TCOR2_COEFF_X0;								// 4040
	sHRegs.tcor2_coeff_x1 = DEFAULT_TCOR2_COEFF_X1;
	sHRegs.tcor2_coeff_x2 = DEFAULT_TCOR2_COEFF_X2;
	sHRegs.tcor2_coeff_x3 = DEFAULT_TCOR2_COEFF_X3;
	sHRegs.tcor3_coeff_x0 = DEFAULT_TCOR3_COEFF_X0;
	sHRegs.tcor3_coeff_x1 = DEFAULT_TCOR3_COEFF_X1;								// 4050
	sHRegs.tcor3_coeff_x2 = DEFAULT_TCOR3_COEFF_X2;
	sHRegs.tcor3_coeff_x3 = DEFAULT_TCOR3_COEFF_X3;
	sHRegs.gas1_ppm_coeff_x0 = DEFAULT_GAS1_PPM_COEFF_X0;
	sHRegs.gas1_ppm_coeff_x1 = DEFAULT_GAS1_PPM_COEFF_X1;
	sHRegs.gas1_ppm_coeff_x2 = DEFAULT_GAS1_PPM_COEFF_X2;					// 4060
	sHRegs.gas1_ppm_coeff_x3 = DEFAULT_GAS1_PPM_COEFF_X3;
	sHRegs.gas2_ppm_coeff_x0 = DEFAULT_GAS2_PPM_COEFF_X0;
	sHRegs.gas2_ppm_coeff_x1 = DEFAULT_GAS2_PPM_COEFF_X1;
	sHRegs.gas2_ppm_coeff_x2 = DEFAULT_GAS2_PPM_COEFF_X2;
	sHRegs.gas2_ppm_coeff_x3 = DEFAULT_GAS2_PPM_COEFF_X3;					// 4070
	sHRegs.gas2_ppm_coeff_x4 = DEFAULT_GAS2_PPM_COEFF_X4;
	sHRegs.gas2_ppm_coeff_x5 = DEFAULT_GAS2_PPM_COEFF_X5;
	sHRegs.gas2_ppm_coeff_x6 = DEFAULT_GAS2_PPM_COEFF_X6;
	sHRegs.gas2_ppm_coeff_x7 = DEFAULT_GAS2_PPM_COEFF_X7;
	sHRegs.lamp_data_time_0 = DEFAULT_LAMP_DATA_TIME_0;						// 4080
	sHRegs.lamp_data_volt_0 = DEFAULT_LAMP_DATA_VOLT_0;
	sHRegs.lamp_data_time_1 = DEFAULT_LAMP_DATA_TIME_1;
	sHRegs.lamp_data_volt_1 = DEFAULT_LAMP_DATA_VOLT_1;
	sHRegs.lamp_data_time_2 = DEFAULT_LAMP_DATA_TIME_2;
	sHRegs.lamp_data_volt_2 = DEFAULT_LAMP_DATA_VOLT_2;
	sHRegs.abc_correlation_factor = DEFAULT_ABC_CORRELATION_FACTOR;
	sHRegs.abc_sample_rate = DEFAULT_ABC_SAMPLE_RATE;
	sHRegs.abc_eval_count = DEFAULT_ABC_EVAL_COUNT;
	sHRegs.abc_sample_count = DEFAULT_ABC_SAMPLE_COUNT;						// 4090
	sHRegs.dsp_alpha_coeff = DEFAULT_DSP_ALPHA_COEFF;
	sHRegs.dsp_adapt_coeff = DEFAULT_DSP_ADAPT_COEFF;
	sHRegs.dsp_adapt_bound = DEFAULT_DSP_ADAPT_BOUND;
	sHRegs.gas_ppm_upper_bound_1 = DEFAULT_GAS_UPPER_BOUND_1;
	sHRegs.gas_ppm_upper_bound_2 = DEFAULT_GAS_UPPER_BOUND_2;
	sHRegs.altitude = DEFAULT_ALTITUDE;														// 4100
	sHRegs.cal_altitude = DEFAULT_CAL_ALTITUDE;
	sHRegs.pressure = DEFAULT_PRESSURE;
	sHRegs.warm_up_time = DEFAULT_WARM_UP_TIME;
	sHRegs.dac_ctrl = DEFAULT_DAC_CTRL;
	sHRegs.first_sample_cf = DEFAULT_FIRST_SAMPLE_CF;
	sHRegs.avg_ctrl = DEFAULT_AVG_CTRL;														// 4109
}

/* -----------------------------------------------------------------------------
 *       synopsis : Calculates the CRC of the data in the queue buffer. If the
 *                  entire buffer, including CRC, is calculated the resulting
 *                  correct CRC will be 0.
 *      param[in] : q - Reference to the queue that requires the CRC be
 *                  calculated on.
 *         return : The calculated CRC. This could be zero. See comments above.
 *            see : MODBUS Over Seial Line Specification and Implementation
 *                  Guide, V1.02
 *           note : This is a friend function.
 */
uint16_t Flash_CRC(uint8_t *buffer, uint16_t uiLength)
{
  uint8_t  bits;
  uint16_t crc = 0xffff;
	uint16_t uiIndex = 0;

	while(uiIndex < uiLength)
  {
    crc ^= (uint16_t)*buffer ++;
    for (bits=0; bits < 8u; bits++)
    {
      if (crc & 0x0001)
      {
        crc = (crc >> 1) ^ 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
		uiIndex ++;
  }

  return crc;
}  // Flash_CRC
