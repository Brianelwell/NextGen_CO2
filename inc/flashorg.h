/** @file       flash.h
 *  @brief      Low level routines that read, write and erase the flash.
 *  @author     David S. Cooper (DSC)
 *  @copyright  Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *  @copyright  All rights reserved.
 *
 *  $Date: 2017-12-11 14:59:00 -0500 (Mon, 11 Dec 2017) $
 *  $Revision: 190 $
 */
#if !defined(FLASH_H)
#define FLASH_H

/** @name       Definitions that deal with the eeprom emulation in flash. Most
 *              of these definitions are from the Freescale code and are used as
 *              is.
 *  @{ */

/* Common SSD information */
#define FTFx_REG_BASE         (uint32_t)0x40020000
#define PFLASH_BLOCK_BASE     0x00000000
#define PFLASH_BLOCK_SIZE     0x00008000    /* 32K KB size */

/* Flash block to use for emulation - default PFlash */
#define FLASH_START_ADDRESS   (uint32_t)PFLASH_BLOCK_BASE
#define FLASH_END_ADDRESS     (uint32_t)(PFLASH_BLOCK_BASE + \
                                         PFLASH_BLOCK_SIZE - 1)
#define FLASH_SECTOR_SIZE     (uint32_t)0x00000400  /* 1K (byte) sector size */

/* Number of EE blocks set aside */
#define EE_BLOCKS             2

/* EEPROM start address and sector size which is separated from Flash
 * configuration field */
#define EE_START_ADDRESS      (uint32_t)((FLASH_END_ADDRESS+ \
                                          1) - (EE_BLOCKS*FLASH_SECTOR_SIZE))
#define EE_SECTOR_SIZE        FLASH_SECTOR_SIZE

#define ACTIVE_BLOCK_MARKER   ((uint32_t)0xAAAAAAAA)
#define INACTIVE_BLOCK_MARKER ((uint32_t)0x00000000)
#define EMPTY_BLOCK_MARKER    ((uint32_t)0xFFFFFFFF)

/* Return Codes */
#define EE_OK               0x00000000
#define EE_ERR_NOTBLANK     0x00000001
#define EE_ERR_NOTFOUND     0x00000008
#define EE_ERR_SECTORSTATUS 0x00000020
#define EE_ERR_VERIFY       0x00000040
#define EE_ERR_IDRANGE      0x00000100
#define EE_ERR_ACCERR       0x00000200
#define EE_ERR_PVIOL        0x00000400
#define EE_ERR_MGSTAT0      0x00000800
#define EE_ERR_SECURE       0x00001000
#define EE_NOT_OK						0x10000000

/******************************************************************************/
/*               Read/Write/Set/Clear Operation Macros                        */
/******************************************************************************/

#define REG_BIT_TEST(address, mask) (*(volatile uint8_t *)(address) & (mask))
#define REG_WRITE(address, value)   (*(volatile uint8_t *)(address) = (value))
#define REG_READ(address)           ((uint8_t)(*(volatile uint8_t *)(address)))
#define READ8(address)              ((uint8_t)(*(volatile uint8_t *)(address)))

/* Flash Status Register (FSTAT)*/
#define FTFx_SSD_FSTAT_OFFSET             0x00000000
#define FTFx_SSD_FSTAT_CCIF               0x80
#define FTFx_SSD_FSTAT_RDCOLERR           0x40
#define FTFx_SSD_FSTAT_ACCERR             0x20
#define FTFx_SSD_FSTAT_FPVIOL             0x10
#define FTFx_SSD_FSTAT_MGSTAT0            0x01

/* Flash configuration register (FCNFG)*/
#define FTFx_SSD_FCNFG_OFFSET             0x00000001
#define FTFx_SSD_FCNFG_CCIE               0x80
#define FTFx_SSD_FCNFG_RDCOLLIE           0x40
#define FTFx_SSD_FCNFG_ERSAREQ            0x20
#define FTFx_SSD_FCNFG_ERSSUSP            0x10

/* Flash Security Register */
#define FTFx_SSD_FSEC_OFFSET              0x00000002
#define FTFx_SSD_FSEC_KEYEN               0xC0
#define FTFx_SSD_FSEC_FSLACC              0x0C
#define FTFx_SSD_FSEC_SEC                 0x03

/* Flash Option Register (FOPT) */
#define FTFx_SSD_FOPT_OFFSET              0x00000003

/* Flash common command object registers (FCCOB0-B) */
#define FTFx_SSD_FCCOB0_OFFSET            0x00000007
#define FTFx_SSD_FCCOB1_OFFSET            0x00000006
#define FTFx_SSD_FCCOB2_OFFSET            0x00000005
#define FTFx_SSD_FCCOB3_OFFSET            0x00000004
#define FTFx_SSD_FCCOB4_OFFSET            0x0000000B
#define FTFx_SSD_FCCOB5_OFFSET            0x0000000A
#define FTFx_SSD_FCCOB6_OFFSET            0x00000009
#define FTFx_SSD_FCCOB7_OFFSET            0x00000008
#define FTFx_SSD_FCCOB8_OFFSET            0x0000000F
#define FTFx_SSD_FCCOB9_OFFSET            0x0000000E
#define FTFx_SSD_FCCOBA_OFFSET            0x0000000D
#define FTFx_SSD_FCCOBB_OFFSET            0x0000000C

/* P-Flash protection registers (FPROT0-3) */
#define FTFx_SSD_FPROT0_OFFSET            0x00000013
#define FTFx_SSD_FPROT1_OFFSET            0x00000012
#define FTFx_SSD_FPROT2_OFFSET            0x00000011
#define FTFx_SSD_FPROT3_OFFSET            0x00000010

/* SGF commands */
#define FTFx_VERIFY_BLOCK               	0x00
#define FTFx_VERIFY_SECTION             	0x01
#define FTFx_PROGRAM_CHECK              	0x02
#define FTFx_READ_RESOURCE              	0x03
#define FTFx_PROGRAM_LONGWORD           	0x06
#define FTFx_PROGRAM_PHRASE             	0x07
#define FTFx_ERASE_BLOCK                	0x08
#define FTFx_ERASE_SECTOR               	0x09
#define FTFx_PROGRAM_SECTION            	0x0B
#define FTFx_VERIFY_ALL_BLOCK           	0x40
#define FTFx_READ_ONCE                  	0x41
#define FTFx_PROGRAM_ONCE               	0x43
#define FTFx_ERASE_ALL_BLOCK            	0x44
#define FTFx_SECURITY_BY_PASS           	0x45
#define FTFx_PFLASH_SWAP                	0x46
#define FTFx_PROGRAM_PARTITION          	0x80
#define FTFx_SET_EERAM                  	0x81

/* Flash margin read settings */
#define FTFx_NORMAL_MARGIN              	0x0000
#define FTFx_USER_MARGIN		          	  0x0001
#define FTFx_FACTORY_MARGIN			       	  0x0002

/* Command Sequence Index Macros */
#define COMMAND_INDEX0                  	0x00
#define COMMAND_INDEX1                  	0x01
#define COMMAND_INDEX2                  	0x02
#define COMMAND_INDEX3                  	0x03
#define COMMAND_INDEX4                  	0x04
#define COMMAND_INDEX5                  	0x05
#define COMMAND_INDEX6                  	0x06
#define COMMAND_INDEX7                  	0x07
#define COMMAND_INDEX11                 	0x11

/*-------------------- Macro definition for FTFx ----------------------*/
#define FTFx_WORD_SIZE                   	0x02
#define FTFx_DWORD_SIZE                  	0x04
#define FTFx_PHRASE_SIZE                 	0x08
#define FTFx_DPHRASE_SIZE                	0x10

/* NULL pointer */
#define NULL_PTR                          ((UINT32*)0xFFFFFFFF)

/** @} */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief      Program long-word command. Programs four (4) previously erased
 *              bytes in program flash memory using an embedded algorighm.
 *  @param [in] destination
 *              Destination flash address.
 *  @param [in] size
 *              The number of bytes to write.
 *  @param [in] source
 *              The 32-bit longword to program at the destination address.
 *  @return     Returns EE_OK if no error, otherwise one of several possible
 *              errors returned in the FTFA_FSTAT register.
 */
uint32_t flash_program_longword(uint32_t destination, int32_t size, uint32_t source);

/** @brief      Erases all addresses in a flash sector.
 *  @param [in] destination
 *              The flash sector to be erased.
 *  @return     Returns EE_OK if no error, otherwise one of several possible
 *              errors returned in the FTFA_FSTAT register.
 */
uint32_t flash_erase_flash_sector(uint32_t destination);

/** @brief      Checks if a section of program flash memory is erased to the
 *              normal read margin level.
 *  @param [in] section
 *              First longword to be verified.
 *  @param [in] num_longwords
 *              Number of longwords to be verified.
 *  @return     Returns EE_OK if no error, otherwise one of several possible
 *              errors returned in the FTFA_FSTAT register.
 */
uint32_t flash_read_ones_section(uint32_t section, uint16_t num_longwords);

/** @brief      Generalized function for sending a command to the flash memory
 *              module.
 *  @param [in] index
 *              Number of elements to expect in the command array.
 *  @param [in] commandArray
 *              Array elements map to the registers used by the flash memory
 *              module.
 *  @return     Returns EE_OK if no error, otherwise one of several possible
 *              errors returned in the FTFA_FSTAT register.
 */
uint32_t flash_command_sequence(uint8_t index, uint8_t *commandArray);

#ifdef __cplusplus
}
#endif

#endif /* FLASH_H */

