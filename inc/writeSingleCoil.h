/** @file       writeSingleCoil.h
 *  @brief      Implements MODBUS function 5, Write Single Coil (a.k.a, a
 *              discrete output, like a relay).
 *
 *              This function code is used to write a single discrete output to
 *              either ON or OFF in a remote device.
 *
 *              The requested ON/OFF state is specified by a constant in the
 *              request data field. A value of 'FF00'H hex requests the output
 *              to be ON. A value of '0000'H requests it to be OFF. All other
 *              values are illegal and will not affect the output.
 *
 *              The Request PDU specifies the address of the descrete output to
 *              be forced. Descrete outputs are addressed starting at zero.
 *
 *  @author     David S. Cooper (DSC)
 *  @copyright  Copyright (c) 2014-2017 by Telaire Products, Goleta CA
 *  @copyright  All rights reserved.
 *
 *  $Date: 2017-12-11 14:59:00 -0500 (Mon, 11 Dec 2017) $
 *  $Revision: 190 $
 */
#if !defined(WRITESINGLECOIL_H)
#define WRITESINGLECOIL_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief  Write Single Coil, MODBUS Function 5 (0x05).
 *  @see MODBUS Application Protocol Specification V1.1b.
 */
uint8_t write_single_coil(void);

#ifdef __cplusplus
}
#endif

#endif /* WRITESINGLECOIL_H */

