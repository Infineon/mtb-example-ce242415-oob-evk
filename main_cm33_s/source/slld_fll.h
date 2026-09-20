/******************************************************************************
* File Name: slld_fll.h
*
* Description: This is a header file for SPI based external flash
*              
********************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/

#ifndef  __INC_slldtargetspecifich
#define __INC_slldtargetspecifich

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


// don't need to sunifdef
//INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
//USE_4B_ADDR_CMDS

// Flash page size
#define PAGE_SIZE (256)
#define PAGE_MASK (PAGE_SIZE-1)

// Status register error bits definition
#define STATUS_ERROR_FLAGS  (HAS_STATBIT5_ERROR | HAS_STATBIT6_ERROR)

// Read ID command
#define SPI_READ_ID_CMD SPI_RDID_AB_CMD
#define SPI_RDID_CMD SPI_RDID_9F_CMD

// defin block protect bit mask
#define BLOCK_PROTECT_BITS_MASK  (0x1C)

//************************************************************************
// enables code to execute commands from a file instead of directly from *
// the command line.                                                     *
//************************************************************************
#define ENABLE_SCRIPTING_MACRO

/************************************************************************
 * Creates lld_printf.log file and writes all the LLD_PRINTFs to it      *
 ************************************************************************/
//#define LLD_PRINTF_LOGFILE


//*******************************************************
// Define system base address   *
// Followed is just example code, user have to modify the mask and shift bit number based on own system configuration
//*******************************************************
#define BASE_ADDR_MASK    0xC0000000
#define BASE_ADDR_SHIFT_BIT 30
//*******************************************************
// Define flash simulator R/W debug macro (no trace)    *
//*******************************************************
#ifdef __FLASHMODEL
// The number of clocks to write various types of data
#define WRITEDATALEN 8
#define WRITECMDLEN  8
#define WRITEADDR24  24
#define WRITEADDR32  32

#define DEBUG_FLASH_WR(b,d)   spiSignalManagerAdapter->SM_ADPT_WR((UINT32)(b), WRITEDATA, d, WRITEDATALEN)
#define DEBUG_FLASH_RD(b)     spiSignalManagerAdapter->SM_ADPT_RD()
#else
// The number of clocks to write various types of data
// these values are only used by the model
#define WRITEDATALEN 0
#define WRITECMDLEN  0
#define WRITEADDR24  0
#define WRITEADDR32  0

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __INC_slldtargetspecifich */

