

#include "main.h"


void Handle_Rcvd_Msg(void)
{
	uint16_t uiCommand;
	uint16_t uiAddress, uiStartAddress, uiEndAddress;
	uint16_t uiQty_Regs;
	uint16_t uiData_Offset, uiType_Offset;
	uint16_t uiCRC;
	uint16_t i;
	uint8_t ucErrorCommand;
	uint8_t ucErrorCode = 0;
	HRegTypes sHRegType;
	IRegTypes sIRegType;
	uint8_t *pType, *pData;
	uint8_t *pDataLoc;
	uint8_t *pTypeLoc;

	//  Decipher Receive Parameters
	uiCommand = RxBuff.Buff[1];
	ucErrorCommand = uiCommand & 0xff;
	uiStartAddress = RxBuff.Buff[2] << 8;
	uiStartAddress += RxBuff.Buff[3];
	uiQty_Regs = RxBuff.Buff[4] << 8;
	uiQty_Regs += RxBuff.Buff[5];
	
	//  Setup Transmit Buffer
	TxBuff.Buff[0] = sHRegs.slave_address;
	TxBuff.Buff[1] = uiCommand;
	TxBuff.ptr = 2;

	switch(uiCommand)
	{
	case 3:															//  Read Holding Registers
	case 4:															//  Read Input Registers
		//  Decipher Receive Request
		if(uiCommand == 3)
		{
			if(uiStartAddress < HOLDING_REGISTERS_OFFSET)
			{
				ucErrorCode = 2u;
				break;
			}
			uiStartAddress = uiStartAddress - HOLDING_REGISTERS_OFFSET;
			uiEndAddress = uiStartAddress + uiQty_Regs;
			pData = (uint8_t*)pHRegs;
			pType = (uint8_t*)&sHRegType;
		}
		else
		{
			if(uiStartAddress < INPUT_REGISTERS_OFFSET)
			{
				ucErrorCode = 2u;
				break;
			}
			uiStartAddress = uiStartAddress - INPUT_REGISTERS_OFFSET;
			uiEndAddress = uiStartAddress + uiQty_Regs;
			pData = (uint8_t*)pIRegs;
			pType = (uint8_t*)&sIRegType;
		}
		if(uiQty_Regs > 0x7d)							//  Confirm Number of Registers in Range
		{
			ucErrorCode = 3u;
			break;
		}
		
		TxBuff.Buff[TxBuff.ptr++] = uiQty_Regs * 2;			//  Use Register Qty to Calc Qty of Bytes
		uiData_Offset = 0;															//  Initialize Offset Variables
		uiType_Offset = 0;

		for(uiAddress = 0; uiAddress < uiEndAddress; uiAddress += 0)
		{
			//  Reset Type Location Pointer
			pTypeLoc = pType;
			pTypeLoc += uiType_Offset;
			pDataLoc = pData;
			pDataLoc += uiData_Offset;
			
			//  If in Range, Read Register Values
			if(uiAddress >= uiStartAddress)
			{
				if(*pTypeLoc > 1)
				{
					if((uint64_t)pDataLoc % 4 > 0)
					{
						pDataLoc += 2;
					}
					TxBuff.Buff[TxBuff.ptr + 3] = *pDataLoc;
					pDataLoc ++;
					TxBuff.Buff[TxBuff.ptr + 2] = *pDataLoc;
					pDataLoc ++;
					TxBuff.Buff[TxBuff.ptr + 1] = *pDataLoc;
					pDataLoc ++;
					TxBuff.Buff[TxBuff.ptr] = *pDataLoc;
					pDataLoc ++;
					TxBuff.ptr += 4u;
				}
				else
				{
					TxBuff.Buff[TxBuff.ptr + 1] = *pDataLoc;
					pDataLoc ++;
					TxBuff.Buff[TxBuff.ptr] = *pDataLoc;
					pDataLoc ++;
					TxBuff.ptr += 2u;
				}
			}
			if(*pTypeLoc > 1)
			{
				if((uint64_t)pDataLoc % 4 > 0)
				{
					uiData_Offset += 2;
				}
				uiData_Offset += 4;
				uiAddress += 2;
			}
			else
			{
				uiData_Offset += 2;
				uiAddress ++;
			}
			uiType_Offset ++;
		}

		TxBuff.end = TxBuff.ptr;					//  Needed for CRC Routine
		uiCRC = Calc_crc(TxBuff);
		TxBuff.Buff[TxBuff.ptr++] = uiCRC & 0xff;
		TxBuff.Buff[TxBuff.ptr++] = (uiCRC >> 8) & 0xff;
		TxBuff.end = TxBuff.ptr;

		if(uiFlags.bI2C_Mode == true)
		{
			I2C_SendMessage();
		}
		else
		{
			USART1_SendMessage();
		}
		break;
		
	case 5:													//  Write Single Coil Register
		;		// Coil Action
		
		TxBuff.Buff[TxBuff.ptr++] = (uiStartAddress >> 8) & 0xff;
		TxBuff.Buff[TxBuff.ptr++] = uiStartAddress & 0xff;
		TxBuff.Buff[TxBuff.ptr++] = (uiQty_Regs >> 8) & 0xff;
		TxBuff.Buff[TxBuff.ptr++] = uiQty_Regs & 0xff;
		
		TxBuff.end = TxBuff.ptr;					//  Needed for CRC Routine
		uiCRC = Calc_crc(TxBuff);
		TxBuff.Buff[TxBuff.ptr++] = uiCRC & 0xff;
		TxBuff.Buff[TxBuff.ptr++] = (uiCRC >> 8) & 0xff;
		TxBuff.end = TxBuff.ptr;

		if(uiFlags.bI2C_Mode == true)
		{
			I2C_SendMessage();
		}
		else
		{
			USART1_SendMessage();
		}
		break;
		
	case 16:																				//  Write Multiple Holding Registers
		uiStartAddress = uiStartAddress - HOLDING_REGISTERS_OFFSET;
		uiEndAddress = uiStartAddress + uiQty_Regs;
		pData = (uint8_t*)pHRegs;
		pType = (uint8_t*)&sHRegType;
		
		uiData_Offset = 0;									// Initialize Variables
		uiType_Offset = 0;
		i = 7;															//  First Data Position in Receive Buffer

		for(uiAddress = 0; uiAddress < uiEndAddress; uiAddress += 0)
		{
			//  Reset Type Location Pointer
			pTypeLoc = pType;
			pTypeLoc += uiType_Offset;
			pDataLoc = pData;
			pDataLoc += uiData_Offset;
			if(uiAddress >= uiStartAddress)
			{
				//  If in Range, Write Register Values
				if(*pTypeLoc > 1)
				{
					if((uint32_t)pDataLoc % 4 > 0)
					{
						pDataLoc += 2;
					}
					*pDataLoc = RxBuff.Buff[i + 3];
					pDataLoc ++;
					*pDataLoc = RxBuff.Buff[i + 2];
					pDataLoc ++;
					*pDataLoc = RxBuff.Buff[i + 1];
					pDataLoc ++;
					*pDataLoc = RxBuff.Buff[i];
					pDataLoc ++;
					i += 4;
				}
				else
				{
					*pDataLoc = RxBuff.Buff[i + 1];
					pDataLoc ++;
					*pDataLoc = RxBuff.Buff[i];
					pDataLoc ++;
					i += 2;
				}
			}
			if(*pTypeLoc > 1)
			{
				if((uint32_t)pDataLoc % 4 > 0)
				{
					uiData_Offset += 2;
				}
				uiData_Offset += 4;
				uiAddress += 2;
			}
			else
			{
				uiData_Offset += 2;
				uiAddress ++;
			}
			uiType_Offset ++;
		}
		
		//  Setup and Send Response
		TxBuff.Buff[2] = RxBuff.Buff[2];
		TxBuff.Buff[3] = RxBuff.Buff[3];
		TxBuff.Buff[4] = RxBuff.Buff[4];
		TxBuff.Buff[5] = RxBuff.Buff[5];
		TxBuff.end = 6;
		TxBuff.ptr = 6;
		uiCRC = Calc_crc(TxBuff);
		TxBuff.Buff[TxBuff.ptr++] = uiCRC & 0xff;
		TxBuff.Buff[TxBuff.ptr++] = (uiCRC >> 8) & 0xff;
		TxBuff.end = TxBuff.ptr;

		if(uiFlags.bI2C_Mode == true)
		{
			I2C_SendMessage();
		}
		else
		{
			USART1_SendMessage();
		}
		uiFlags.bFlashCommitInProcess = true;				//  Setup Commit Timer
		uiCommit_Timer = 0;
		break;

	default:
		uiCommand = 0;				//  Replace Command With Unsupported Code
		if(ucErrorCode == 0)
		{
			ucErrorCode = 1;
		}
		break;
	}
	
	//  Handle Any Receive or Protocol Errors
	if(ucErrorCode)
	{
		TxBuff.Buff[1] = 0x80 + ucErrorCommand;
		TxBuff.Buff[TxBuff.ptr++] = ucErrorCode;
		TxBuff.end = 3u;
		uiCRC = Calc_crc(TxBuff);
		TxBuff.Buff[TxBuff.ptr++] = uiCRC & 0xff;
		TxBuff.Buff[TxBuff.ptr++] = (uiCRC >> 8) & 0xff;
		TxBuff.end = TxBuff.ptr;
		if(uiFlags.bI2C_Mode == true)
		{
			I2C_SendMessage();
		}
		else
		{
			USART1_SendMessage();
		}
	}
/*
	//  Reset Receive Buffer
	for(RxBuff.ptr = 0; RxBuff.ptr < RxBuff.end; RxBuff.ptr ++)
	{
		RxBuff.Buff[RxBuff.ptr] = 0;
	}
	RxBuff.end = 0;
	RxBuff.ptr = 0;
*/
}

uint16_t Calc_crc(Buffer buff)
{
  uint8_t  bits;
  uint16_t crc = 0xffff;

	buff.ptr = 0;
  while(buff.ptr < buff.end)
  {
    crc ^= (uint16_t)buff.Buff[buff.ptr];
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
	  if(++buff.ptr >= BUFFER_SIZE)
	  {
			return 0;
		}
  }

  return crc;
}  /* crc16 */
