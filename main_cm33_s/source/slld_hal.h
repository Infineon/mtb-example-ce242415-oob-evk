/******************************************************************************
* File Name: slld_hal.h
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

#ifndef __INC_lld_halh
#define __INC_lld_halh

// Users should define what controller has been used for their customerized
// enviroment. The following code is just an implementation example, which should
// be modified by the user accordingly to match their specific hardware environment.

#ifdef USER_CONTROLLER
extern unsigned long base_addr_g;
#define CONTROLLER_BUFFER_SIZE   1024
#define SPIADDR   ((volatile unsigned char *)base_addr_g)
#define SPIDATA   (*SPIADDR)
#define SPIDATA_D (*(SPIADDR + 0x01))
#define SPIDATA_Q (*(SPIADDR + 0x02))
#define SPIHOLD   (*(SPIADDR + 0x04))
#define SPICS     (*(SPIADDR + 0x10))
#define SPIWP     (*(SPIADDR + 0x30))
#define PISMOLED  (*(SPIADDR + 0x40))
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* public function prototypes */

// HAL read functions API

/* device number to which operation will be done */

SLLD_STATUS init_spi(void);

/* system address to be used */
SLLD_STATUS FLASH_READ(BYTE  device_num,BYTE  command, ADDRESS sys_addr, BYTE  *data_buffer, int Number_Of_Read_Bytes );

// HAL write functions API
SLLD_STATUS FLASH_WRITE(BYTE device_num,BYTE  command,ADDRESS sys_addr,BYTE   *data_buffer,int Number_Of_Written_Bytes );



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __INC_lld_halh */

