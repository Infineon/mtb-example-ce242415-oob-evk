/******************************************************************************
* File Name: slld.c
*
* Description: This is a source file for SPI based external flash
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

#include <stdio.h>


#include "slld.h"
#include "slld_hal.h"

#ifdef TRACE
#include "trace.h"
#endif

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
/* static variable to store SPI flash software protect status */
static DEV_SOFTWARE_PROTECT_STATUS sys_software_protect_status = FLASH_SOFTWARE_UNPROTECTED;
#endif

BYTE modebit_char;
BYTECOUNT g_PAGE_SIZE=PAGE_SIZE;











/******************************************************************************
 *
 * slld_Read_IDCmd - Read ID from SPI Flash
 *
 * This function issues the Read_ID command to SPI Flash and reads out the ID.
 * This command sets the target device in software-unprotected state
 * so this function also sets sys_software_protect_status to FLASH_SOFTWARE_UNPROTECTED.
 *
 * RETURNS: SLLD_OK or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Read_IDCmd(BYTE device_num, BYTE* target)
{
    SLLD_STATUS status = SLLD_OK;

    status = FLASH_RD(device_num,SPI_READ_ID_CMD, (ADDRESS)ADDRESS_NOT_USED, target, 1);

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    sys_software_protect_status = FLASH_SOFTWARE_UNPROTECTED;
#endif

    return(status);
}

/******************************************************************************
 *
 * slld_RDIDCmd - Read Edentification from SPI Flash
 *
 * This function issues the RDID command to SPI Flash and reads out the ID.
 * On some devices this command is documented in the data sheet as
 * Release from deep power down and read electronic signature
 *
 * RETURNS: SLLD_OK or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RDIDCmd(BYTE device_num, BYTE* target, BYTECOUNT len_in_bytes)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_RDID_CMD, (ADDRESS)ADDRESS_NOT_USED, target, len_in_bytes);

    return(status);
}


/******************************************************************************
 *
 * slld_RDSRCmd - Read from Status Register
 *
 * This function issues the RDSR command to SPI Flash and reads from status register.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RDSRCmd(BYTE device_num, BYTE* target)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    status = FLASH_RD(device_num,SPI_RDSR_CMD, (ADDRESS)ADDRESS_NOT_USED, target, 1);

    return(status);
}






/******************************************************************************
 *
 * slld_WRENCmd - Issue the write enable command
 *
 * This function issues the WREN command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRENCmd(BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_WREN_CMD, (ADDRESS)ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_WRDICmd - Issue the write disable command
 *
 * This function issues the WRDI command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRDICmd(BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_WRDI_CMD, (ADDRESS)ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}


void slld_VersionCmd(void)
{       
    printf("\n%s\n", SLLD_VERSION);
}









/******************************************************************************
 *
 * slld_DPCmd - Deep Power-down
 *
 * This function issues the Deep Power-down command to SPI Flash.
 * This command sets the target device in deep power-down state to reduce power consumption
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DPCmd(BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_DP_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    sys_software_protect_status = FLASH_SOFTWARE_PROTECTED;
#endif

    return(status);
}



/******************************************************************************
 *
 * slld_RESCmd - Release from Software Protect (Deep power-down)
 *
 * This function issues the Release from Software Protect command to SPI Flash.
 * This command sets the target device in software-unprotected state
 * so this function also sets sys_software_protect_status[device_num] to FLASH_SOFTWARE_UNPROTECTED.
 *
 * RETURNS: SLLD_OK or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RESCmd (BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

    status = FLASH_WR(device_num,SPI_RES_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    sys_software_protect_status = FLASH_SOFTWARE_UNPROTECTED;
#endif

    return(status);
}

/******************************************************************************
 *
 * slld_ClearStatusRegisterCmd - Performs a clear status register command.
 *
 * This function issues the clear status register command to the SPI flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_ClearStatusRegisterCmd (BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_CLSR_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_WRRCmd - Write to Status and Configure Registers
 *
 * This function issues the Write Registers command to the SPI Flash.
 * If configure value is NULL, the configure value will not be sent.
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRRCmd(BYTE device_num, BYTE* status_val, BYTE* config_val, BYTE* status2_val)
{
    SLLD_STATUS status = SLLD_OK;
    BYTE Buffer[3];

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    if(!status_val)return SLLD_ERROR;
    Buffer[0] = *status_val;
    if(status2_val != NULL)
    {
        if(!config_val || !status_val) return SLLD_ERROR;
        Buffer[1] = *config_val;
        Buffer[2] = *status2_val;
        status = FLASH_WR(device_num,SPI_WRR_CMD, ADDRESS_NOT_USED, Buffer, 3);
        return status;
    }
    if (config_val == NULL)
    {
        status = FLASH_WR(device_num,SPI_WRR_CMD, ADDRESS_NOT_USED, Buffer, 1);
    } else {
        Buffer[1] = *config_val;
        status = FLASH_WR(device_num,SPI_WRR_CMD, ADDRESS_NOT_USED, Buffer, 2);
    }

    return(status);
}

/******************************************************************************
 *
 * slld_WRROp - Write to Status and Config Registers
 *
 * This function issues the Write Registers command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRROp(BYTE device_num, BYTE* status_val, BYTE* config_val, BYTE* status2_val, DEVSTATUS* dev_status_ptr)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRRCmd(device_num, status_val, config_val,status2_val);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);       /* just in case WRROp is operated on protected area */

    return(status);
}






/******************************************************************************
 *
 * slld_RCRCmd - Read from the configuration register
 *
 * This function issues the RCR command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RCRCmd(BYTE device_num, BYTE* target)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_RCR_CMD, ADDRESS_NOT_USED, target, 1);

    return(status);
}



/******************************************************************************
 *
 * slld_ReadCmd - Read from SPI Flash
 *
 * This function issues the Read command to SPI Flash and reads from the array.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_ReadCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_READ_CMD, sys_addr, target, len_in_bytes);

    return(status);
}


/******************************************************************************
 *
 * slld_Read_4BCmd - Read from SPI Flash in 4 bytes addressing scheme
 *
 * This function issues the Read_4B command to SPI Flash and reads from the array.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Read_4BCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_READ_4B_CMD, sys_addr, target, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_Fast_ReadCmd - Fast Read from SPI Flash
 *
 * This function issues the Fast Read command to SPI Flash and reads from the array.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Fast_ReadCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes)
{
    SLLD_STATUS         status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_FAST_READ_CMD, sys_addr, target, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_Fast_Read_4BCmd - Fast Read from SPI Flash in 4 bytes addressing scheme
 *
 * This function issues the 4-bytes Fast Read command to SPI Flash and reads from the array.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Fast_Read_4BCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_FAST_READ_4B_CMD, sys_addr, target, len_in_bytes);

    return(status);
}



/******************************************************************************
 *
 * slld_DORCmd - Read flash using dual output
 *
 * This function issues the dual output read command and reads the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_DOR_CMD, sys_addr, target, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_4DORCmd - Read flash using dual output - 4 Bytes address
 *
 * This function issues the 4B dual output read command and reads the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4DORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_4DOR_CMD, sys_addr, target, len_in_bytes);

    return(status);
}



/******************************************************************************
 *
 * slld_QORCmd - Read flash using quad output
 *
 * This function issues the quad output read command and reads the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QORCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS    sys_addr,        /* device address given by system */
        BYTE      *target,          /* variable in which to store read data */
        BYTECOUNT  len_in_bytes     /* number of bytes to read */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_QOR_CMD, sys_addr, target, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_4QORCmd - Read flash using quad output in 4-bytes addressing scheme
 *
 * This function issues the quad output read command and reads the requested data in 4-bytes
 * addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4QORCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS    sys_addr,        /* device address given by system */
        BYTE      *target,          /* variable in which to store read data */
        BYTECOUNT  len_in_bytes     /* number of bytes to read */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_4QOR_CMD, sys_addr, target, len_in_bytes);

    return(status);
}



/******************************************************************************
 *
 * slld_DIORCmd - Read flash using dual IO
 *
 * This function issues the dual IO read command and reads the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DIORCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS    sys_addr,        /* device address given by system */
        BYTE      *target,          /* variable in which to store read data */
        BYTE                modebit,          // The read mode to be passed to the device
        BYTECOUNT  len_in_bytes     /* number of bytes to read */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    modebit_char = modebit;
    status = FLASH_RD(device_num,SPI_DIOR_CMD, sys_addr, target, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_4DIORCmd - Read flash using dual IO - 4 Bytes address
 *
 * This function issues the 4B dual IO read command and reads the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4DIORCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS    sys_addr,        /* device address given by system */
        BYTE      *target,          /* variable in which to store read data */
        BYTE                modebit,          // The read mode to be passed to the device
        BYTECOUNT  len_in_bytes     /* number of bytes to read */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    modebit_char = modebit;
    status = FLASH_RD(device_num,SPI_4DIOR_CMD, sys_addr, target, len_in_bytes);

    return(status);
}



/******************************************************************************
 *
 * slld_QIORCmd - Read flash using quad IO
 *
 * This function issues the quad IO read command and reads the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QIORCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS    sys_addr,        /* device address given by system */
        BYTE      *target,          /* variable in which to store read data */
        BYTE      modebit,        /* mode bit */
        BYTECOUNT  len_in_bytes     /* number of bytes to read */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    modebit_char = modebit;
    status = FLASH_RD(device_num,SPI_QIOR_CMD, sys_addr, target, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_4QIORCmd - Read flash using quad IO in 4-bytes addressing scheme
 *
 * This function issues the quad IO read command and reads the requested data in 4-bytes
 * addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4QIORCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS    sys_addr,        /* device address given by system */
        BYTE      *target,          /* variable in which to store read data */
        BYTE      modebit,        /* mode bit */
        BYTECOUNT  len_in_bytes     /* number of bytes to read */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    modebit_char = modebit;
    status = FLASH_RD(device_num,SPI_4QIOR_CMD, sys_addr, target, len_in_bytes);

    return(status);
}








/******************************************************************************
 *
 * slld_DDRQIORCmd - Read flash using DDR Quad IO
 *
 * This function issues the DDR Quad IO read command and reads the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DDRQIORCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS                sys_addr,      // device address given by system
        BYTE                *target,          // variable in which to store read data
        BYTE                modebit,          // mode bit
        BYTECOUNT        len_in_bytes         // number of bytes to read
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    modebit_char = modebit;
    status = FLASH_RD(device_num,SPI_DDRQIOR_CMD, sys_addr, target, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_4DDRQIORCmd - Read flash using DDR Quad IO in 4-bytes addressing scheme
 *
 * This function issues the DDR Quad IO read command and reads the requested data in 4-bytes
 * addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4DDRQIORCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS                sys_addr,      // device address given by system
        BYTE                *target,          // variable in which to store read data
        BYTE                modebit,          // The read mode to be passed to the device
        BYTECOUNT        len_in_bytes         // number of bytes to read
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    modebit_char = modebit;
    status = FLASH_RD(device_num,SPI_4DDRQIOR_CMD, sys_addr, target, len_in_bytes);

    return(status);
}


/******************************************************************************
 *
 * slld_SECmd - Sector Erase
 *
 * This function issues the Sector Erase command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SECmd
(
        BYTE      device_num,                     //device number
        ADDRESS  sys_addr                     /* device address given by system */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_SE_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_SEOp - Performs a Sector Erase Operation
 *
 * Function erases specified sector.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SEOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                 /* device address given by system */
        DEVSTATUS  *dev_status_ptr            /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

#ifdef USE_4B_ADDR_CMDS
    status = slld_SE_4BCmd(device_num, sys_addr);
#else
    status = slld_SECmd(device_num, sys_addr);
#endif
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);        /* just in case SEOp is operated on a protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_SE_4BCmd - Sector Erase using 4-bytes addressing scheme
 *
 * This function issues the Sector Erase command to SPI Flash using 4-bytes addressing scheme
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SE_4BCmd
(
        BYTE      device_num,                     //device number
        ADDRESS  sys_addr               /* device address given by system */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_SE_4B_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_SE_4BOp - Performs a Sector Erase Operation using 4-bytes addressing scheme
 *
 * Function erases specified sector.
 * Function issues all required commands and polls for completion using 4-bytes addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SE_4BOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                 /* device address given by system */
        DEVSTATUS  *dev_status_ptr            /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_SE_4BCmd(device_num, sys_addr);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);        /* just in case SE_4BOp is operated on a protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_HBECmd - Half Block Erase
 *
 * This function issues the Half Block Erase command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_HBECmd
(
        BYTE      device_num,                     //device number
        ADDRESS  sys_addr                     /* device address given by system */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_HBE_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_HBEOp - Performs a Half Block Erase Operation
 *
 * Function erases specified sector.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_HBEOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                 /* device address given by system */
        DEVSTATUS  *dev_status_ptr            /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

#ifdef USE_4B_ADDR_CMDS
    status = slld_4HBECmd(device_num, sys_addr);
#else
    status = slld_HBECmd(device_num, sys_addr);
#endif
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);        /* just in case HBEOp is operated on a protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_4HBECmd - Half Block Erase using 4-bytes addressing scheme
 *
 * This function issues the Half Block Erase command to SPI Flash using 4-bytes addressing scheme
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4HBECmd
(
        BYTE      device_num,                     //device number
        ADDRESS  sys_addr               /* device address given by system */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_4HBE_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_4BECmd - BLOCK Erase - 4 Bytes address
 *
 * This function issues the 4B BLOCK Erase command to SPI Flash. 
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4BECmd 
(
        BYTE      device_num,                     //device number
        ADDRESS  sys_addr
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_4BE_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return(status);
}


/******************************************************************************
 *
 * slld_CECmd - Chip erase
 *
 * This function issues the Chip Erase command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_CECmd 
(
        BYTE      device_num                     //device number
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_CE_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_CEOp - Performs a chip Erase Operation 
 *
 * Function erase all data in a device.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_CEOp
(
        BYTE  device_num,            /* device number */
        DEVSTATUS  *dev_status_ptr            /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_CECmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);       /* just in case CEOp is operated on protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_CE1Cmd - Chip erase alternate instruction
 *
 * This function issues the Chip Erase command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_CE1Cmd 
(
        BYTE      device_num                     //device number
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_CE1_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_CE1Op - Performs a chip Erase Operation alternative instruction
 *
 * Function erase all data in a device.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_CE1Op
(
        BYTE  device_num,            /* device number */
        DEVSTATUS  *dev_status_ptr       /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_CE1Cmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);       /* just in case CEOp is operated on protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_BECmd - BLOCK Erase
 *
 * This function issues the BLOCK Erase command to SPI Flash. The Block Erase
 * command sets all bits in the addressed 64 KB block to 1 (all bytes are FFh).
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BlockEraseCmd 
(
        BYTE      device_num,                     //device number
        ADDRESS  sys_addr
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_BLOCK_ERASE_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_BEOp - Performs a Bulk Erase Operation.
 *
 * Function erase all data in a device.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BlockEraseOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,             /* device address given by system */
        DEVSTATUS  *dev_status_ptr       /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_BlockEraseCmd(device_num, sys_addr);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);       /* just in case BEOp is operated on protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_PPCmd - Page Program
 *
 * This function issues a Page Program command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PPCmd
(
        BYTE      device_num,                     //device number
        ADDRESS     sys_addr,                  /* device address given by system */
        BYTE       *data_buf,                  /* variable containing data to program */
        BYTECOUNT   len_in_bytes               /* number of bytes to operate */
)
{
    SLLD_STATUS  status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_PP_CMD, sys_addr, data_buf, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_PPOp - Performs a Page Programming Operation.
 *
 * This function programs location in a page to the specified data.
 * Function issues all required commands and polls for completion.
 * Data size to program must be within PAZE_SIZE.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PPOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                 /* device address given by system */
        BYTE       *data_buf,                 /* variable containing data to program */
        BYTECOUNT   len_in_bytes,             /* number of bytes to program */
        DEVSTATUS  *dev_status_ptr            /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    if(len_in_bytes > PAGE_SIZE)
    {
        status = SLLD_ERROR;  /* Data Bytes are larger than Page_Size */
        return(status);
    }

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_PPCmd(device_num, sys_addr, data_buf, len_in_bytes);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num); /* just in case PPOp is operated on protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_PP_4BCmd - Page Program
 *
 * This function issues the Page Program command to SPI Flash in 4-bytes addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PP_4BCmd
(
        BYTE      device_num,                     //device number
        ADDRESS    sys_addr,                  /* device address given by system */
        BYTE      *data_buf,                  /* variable containing data to program */
        BYTECOUNT  len_in_bytes               /* number of bytes to operate */
)
{
    SLLD_STATUS  status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_PP_4B_CMD, sys_addr, data_buf, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_PP_4BOp - Performs a Page Programming Operation using 4-bytes addressing scheme.
 *
 * Function programs location in a page to the specified data.
 * Function issues all required commands and polls for completion.
 * Data size to program must be within PAZE_SIZE.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PP_4BOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                 /* device address given by system */
        BYTE       *data_buf,                 /* variable containing data to program */
        BYTECOUNT   len_in_bytes,             /* number of bytes to program */
        DEVSTATUS  *dev_status_ptr            /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    if(len_in_bytes > PAGE_SIZE)
    {
        status = SLLD_ERROR;  /* Data Bytes are larger than Page_Size */
        return(status);
    }

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_PP_4BCmd(device_num, sys_addr, data_buf, len_in_bytes);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);/* just in case PPOp is operated on protected area */

    return(status);
}





/******************************************************************************
 *
 * slld_QPPCmd - Page Program using quad IO
 *
 * This function issues the Page Program command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPPCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS    sys_addr,                  /* device address given by system */
        BYTE      *data_buf,                  /* variable containing data to program */
        BYTECOUNT  len_in_bytes               /* number of bytes to operate */
)
{
    SLLD_STATUS         status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_QPP_CMD, sys_addr, data_buf, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_QPPOp - Performs a Quad-Io Page Programming Operation.
 *
 * Function programs location in a page to the specified data.
 * Function issues all required commands and polls for completion.
 * Data size to program must be within PAZE_SIZE.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPPOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                 /* device address given by system */
        BYTE       *data_buf,                 /* variable containing data to program */
        BYTECOUNT   len_in_bytes,             /* number of bytes to program */
        DEVSTATUS  *dev_status_ptr            /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    if(len_in_bytes > PAGE_SIZE)
    {
        status = SLLD_ERROR;  /* Data Bytes are larger than Page_Size */
        return(status);
    }

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_QPPCmd(device_num, sys_addr, data_buf, len_in_bytes);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);/* just in case QPPOp is operated on protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_QPP_4BCmd - Page Program using quad IO in 4-bytes addressing scheme
 *
 * This function issues the Page Program command to the SPI Flash in 4-bytes addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPP_4BCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                  /* device address given by system */
        BYTE       *data_buf,                  /* variable containing data to program */
        BYTECOUNT   len_in_bytes               /* number of bytes to operate */
)
{
    SLLD_STATUS         status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_QPP_4B_CMD, sys_addr, data_buf, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_QPP_4BOp - Performs a Quad-Io Page Programming Operation using 4-bytes addressing scheme.
 *
 * Function programs location in a page to the specified data.
 * Function issues all required commands and polls for completion.
 * Data size to program must be within PAZE_SIZE.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPP_4BOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                 /* device address given by system */
        BYTE       *data_buf,                 /* variable containing data to program */
        BYTECOUNT   len_in_bytes,             /* number of bytes to program */
        DEVSTATUS  *dev_status_ptr            /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    if(len_in_bytes > PAGE_SIZE)
    {
        status = SLLD_ERROR;  /* Data Bytes are larger than Page_Size */
        return(status);
    }

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_QPP_4BCmd(device_num, sys_addr, data_buf, len_in_bytes);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);/* just in case QPPOp is operated on protected area */

    return(status);
}



/******************************************************************************
 *
 * slld_Poll - Polls flash device for embedded operation completion
 *
 * This function polls the Flash device to determine when an embedded
 * operation is finished.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Poll
(
        BYTE  device_num,            /* device number */
        DEVSTATUS  *dev_status_ptr              /* variable to store device status */
)
{
    SLLD_STATUS       status = SLLD_OK;

    *dev_status_ptr = dev_status_unknown;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    do
    {
        status = slld_StatusGet(device_num, dev_status_ptr);
        if(status != SLLD_OK)
            return(status);
    }
    while(*dev_status_ptr == dev_busy);

    return(status);
}

/******************************************************************************
 *
 * slld_StatusGet - Determines Flash Status
 *
 * This function gets the device status from the SPI flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_StatusGet
(
        BYTE  device_num,            /* device number */
        DEVSTATUS*  dev_status_ptr                  /* variable to store device status */
)
{
    SLLD_STATUS  status = SLLD_OK;
    BYTE poll_data;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = slld_RDSRCmd(device_num, &poll_data);
    if(status != SLLD_OK)
        return(status);

    // Check if steady state
    if (poll_data & B0_MASK) {                     // If b0 = 1 then device is busy
        status = slld_RDSR2Cmd(device_num, &poll_data);
        if(status != SLLD_OK)
            return(status);
        // If b5 = 1 then there was an erase error
        if ((STATUS_ERROR_FLAGS & HAS_STATBIT5_ERROR) && (poll_data & B5_MASK))
            *dev_status_ptr = dev_erase_error;
        // If b6 = 1 then there was a program error
        else if ((STATUS_ERROR_FLAGS & HAS_STATBIT6_ERROR) && (poll_data & B6_MASK))
            *dev_status_ptr = dev_program_error;
        else
            *dev_status_ptr = dev_busy;
    }
    else
    {
        // If b0 = 0 then device is not busy
        *dev_status_ptr = dev_not_busy;
    }

    return (status);
}

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
/******************************************************************************
 *
 * slld_SoftwareProtectStatusGet - Determines Flash Software Protection Status
 *
 * This function determines the flash software protection status.
 *
 * RETURNS: SLLD_OK.
 *
 */
SLLD_STATUS slld_SoftwareProtectStatusGet    ///xbill ????, how to add device_num?
(
        DEV_SOFTWARE_PROTECT_STATUS*  dev_softwareprotect_status_ptr  /* variable to store device software protect status */
)
{
    SLLD_STATUS status = SLLD_OK;

    *dev_softwareprotect_status_ptr = sys_software_protect_status;

    return(status);
}
#endif /* INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK */

/******************************************************************************
 *
 * slld_BufferedProgramOp - Performs a Programming Operation.
 *
 * Function programs data to the specfied address.
 * Function issues all required commands and polls for completion. Address doesn't have to be
 * page aligned.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BufferedProgramOp
(
        BYTE  device_num,            /* device number */
        ADDRESS    sys_addr,                   /* device address given by system */
        BYTE      *data_buf,                   /* variable containing data to program */
        BYTECOUNT  len_in_bytes,               /* number of bytes on which to operate */
        DEVSTATUS *dev_status_ptr              /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;
    ADDRESS     current_sys_addr;          /* current address to which program operation operates */
    BYTECOUNT   current_len_in_bytes;      /* number of bytes to program at a time */
    BYTE       *current_data_ptr;          /* pointer to data to program */

    /* initial setting of variables */
    current_sys_addr = sys_addr;
    current_data_ptr = data_buf;

    current_len_in_bytes = PAGE_SIZE - (sys_addr & PAGE_MASK);
    if (current_len_in_bytes > len_in_bytes)
        current_len_in_bytes = len_in_bytes;

    do
    {
        status = slld_PPOp(device_num, current_sys_addr, current_data_ptr, current_len_in_bytes, dev_status_ptr);
        if(status != SLLD_OK)
            return(status);

        len_in_bytes -= current_len_in_bytes;

        /* set variables for next programming */
        current_sys_addr += current_len_in_bytes;
        current_data_ptr += current_len_in_bytes;

        if (len_in_bytes > PAGE_SIZE)
        {
            current_len_in_bytes = PAGE_SIZE;
        }
        else
        {
            current_len_in_bytes = len_in_bytes;
        }
    }
    while (len_in_bytes != 0);

    status = slld_WRDICmd(device_num);          /* just in case BufferedProgramOp is operated on protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_BufferedProgram_4BOp - Performs a Programming Operation using 4-bytes addressing scheme.
 *
 * Function programs data to the specfied address.
 * Function issues all required commands and polls for completion. Address doesn't have to be
 * page aligned.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BufferedProgram_4BOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                  /* device address given by system */
        BYTE       *data_buf,                  /* variable containing data to program */
        BYTECOUNT   len_in_bytes,              /* number of bytes on which to operate */
        DEVSTATUS  *dev_status_ptr             /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;
    ADDRESS     current_sys_addr;          /* current address to which program operation operates */
    BYTECOUNT   current_len_in_bytes;      /* number of bytes to program at a time */
    BYTE       *current_data_ptr;          /* pointer to data to program */

    /* initial setting of variables */
    current_sys_addr = sys_addr;
    current_data_ptr = data_buf;

    current_len_in_bytes = PAGE_SIZE - (sys_addr & PAGE_MASK);
    if (current_len_in_bytes > len_in_bytes)
        current_len_in_bytes = len_in_bytes;

    do
    {
        status = slld_PP_4BOp(device_num, current_sys_addr, current_data_ptr, current_len_in_bytes, dev_status_ptr);
        if(status != SLLD_OK)
            return(status);

        len_in_bytes -= current_len_in_bytes;

        /* set variables for next programming */
        current_sys_addr += current_len_in_bytes;
        current_data_ptr += current_len_in_bytes;

        if (len_in_bytes > PAGE_SIZE)
        {
            current_len_in_bytes = PAGE_SIZE;
        }
        else
        {
            current_len_in_bytes = len_in_bytes;
        }
    }
    while (len_in_bytes != 0);

    status = slld_WRDICmd(device_num);          /* just in case BufferedProgram_4BOp is operated on protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_BlockProtectOp - Performs a Block Protection Operation.
 *
 * Function sets Block Protect bits to protect specified memory area.
 * Function issues all required commands and polls for completion.
 *
 * Valid <bpb_value> values are:  (please see the datasheet for each device)
 *  0x00 - 0x03 : for S25FL002D, S25FL001D...
 *  0x00 - 0x07 : for S25FL004D, S25FL032P...
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BlockProtectOp
(
        BYTE  device_num,            /* device number */
        BYTE        bpb_value,                 /* block protect bit value */
        DEVSTATUS  *dev_status_ptr             /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;
    BYTE        current_status_register;          /* current state of Status Register */
    BYTE        new_status_register;              /*new state of Status Register */

    status = slld_RDSRCmd(device_num, &current_status_register);
    if(status != SLLD_OK)
        return(status);

    new_status_register = ((current_status_register & (~BLOCK_PROTECT_BITS_MASK)) | (bpb_value << 2) );

// add by pzhu, write volitale register 
    status = slld_WRAR_Op(device_num, SR1V, &new_status_register, dev_status_ptr);

    return(status);
}

SLLD_STATUS slld_QuadWriteOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                 /* device address given by system */
        BYTE       *data_buf,                 /* variable containing data to program */
        BYTECOUNT   len_in_bytes,             /* number of bytes to program */
        DEVSTATUS  *dev_status_ptr            /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;
    BYTECOUNT length;

    while(len_in_bytes)
    {
        if(len_in_bytes < PAGE_SIZE)
            length = len_in_bytes;
        else
            length = PAGE_SIZE;

        status = slld_QPPOp(device_num, sys_addr, data_buf, length, dev_status_ptr);
        if(status != SLLD_OK)
            return(status);

        len_in_bytes -= length;
        sys_addr += length;
        data_buf += length;
    }

    return(status);
}

/******************************************************************************
 *
 * slld_RDSR2Cmd - Read from Status Register-2
 *
 * This function issues the RDSR2 command to SPI Flash and reads from status register.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RDSR2Cmd
(
        BYTE  device_num,            /* device number */
        BYTE    *target                        /* variable in which to store read data */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_RDSR2_CMD, (ADDRESS)ADDRESS_NOT_USED, target, 1);

    return(status);
}

/******************************************************************************
 *
 * slld_DLPRDCmd - Read Data Learning Pattern
 *
 * This function issues the DLPRD command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DLPRDCmd
(
        BYTE  device_num,            /* device number */
        BYTE      *data_buf,                  /* variable in which to store data */
        BYTECOUNT  len_in_bytes               /* number of bytes to operate */
)
{
    SLLD_STATUS         status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_DLPRD_CMD, ADDRESS_NOT_USED, data_buf, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_PNVDLRCmd - Program NV Data Learning Register
 *
 * This function issues the PNVDLR command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PNVDLRCmd
(
        BYTE  device_num,            /* device number */
        BYTE      *data_buf,                  /* variable containing data to program */
        BYTECOUNT  len_in_bytes               /* number of bytes to operate */
)
{
    SLLD_STATUS         status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);
    status = FLASH_WR(device_num,SPI_PNVDLR_CMD, ADDRESS_NOT_USED, data_buf, len_in_bytes);

    return(status);
}

/******************************************************************************
 *
 * slld_WVDLRCmd - Write Volatile Data Learning Register
 *
 * This function issues the WVDLR command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WVDLRCmd
(
        BYTE  device_num,            /* device number */
        BYTE      *data_buf,                  /* variable containing data to program */
        BYTECOUNT  len_in_bytes               /* number of bytes to operate */
)
{
    SLLD_STATUS         status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = FLASH_WR(device_num,SPI_WVDLR_CMD, ADDRESS_NOT_USED, data_buf, len_in_bytes);

    return(status);
}








/******************************************************************************
 *
 * slld_PASSRDCmd - Password Read
 *
 * This function issues the Password Read command to SPI Flash and read data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PASSRDCmd
(
        BYTE  device_num,            /* device number */
        BYTE     *data_buf                       /* variable in which to store read data */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_PASSRD_CMD, ADDRESS_NOT_USED, data_buf, 8);        // 64-bit password

    return(status);
}

/******************************************************************************
 *
 * slld_PASSPCmd - Password Program
 *
 * This function issues the Password Program command to SPI Flash and write data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PASSPCmd
(
        BYTE  device_num,            /* device number */
        BYTE     *data_buf                       /* variable containing data to program */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_PASSP_CMD, ADDRESS_NOT_USED, data_buf, 8);        // 64-bit password

    return(status);
}

/******************************************************************************
 *
 * slld_PASSUCmd - Password Unlock
 *
 * This function issues the Password Unlock command to SPI Flash and unlock data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PASSUCmd
(
        BYTE  device_num,            /* device number */
        BYTE     *data_buf                       /* variable containing data to program */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_PASSU_CMD, ADDRESS_NOT_USED, data_buf, 8);        // 64-bit password

    return(status);
}

/******************************************************************************
 *
 * slld_RDARCmd - Read any device register-non-volatile and volatile.
 *
 * This function issues the read any register command and reads the requested data in 1-bytes
 *
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RDARCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS             reg_addr,         /* register address given by device*/
        BYTE                *target           /*variable in which to store read data*/
)
{
    SLLD_STATUS status = SLLD_OK;
#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    status = FLASH_RD(device_num,SPI_RDAR_CMD, reg_addr, target, 1);

    return(status);
}

/******************************************************************************
 *
 * slld_WRARCmd - Write any device register-non-volatile and volatile.
 *
 * This function issues the write any register command and write 1-byte
 *
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRARCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS             reg_addr,         /*register address given by device*/
        BYTE                *data_buf         /*variable containing data to program*/
)
{
    SLLD_STATUS status = SLLD_OK;
#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    status = FLASH_WR(device_num,SPI_WRAR_CMD, reg_addr, data_buf, 1);

    return(status);
}

/******************************************************************************
 *
 * slld_WRAR_Op - Write any device register-non-volatile and volatile operation
 *
 * This function issues the write any register command and write 1-byte
 *
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRAR_Op
(
        BYTE  device_num,            /* device number */
        ADDRESS             reg_addr,         /*register address given by device*/
        BYTE                *data_buf,        /*variable containing data to program*/
        DEVSTATUS           *dev_status_ptr   /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRARCmd(device_num, reg_addr,data_buf);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);/* just in case PPOp is operated on protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_RSTENCmd -Reset Enable
 *
 * This function issues the Reset Enable command which required immediately before a
 * Reset command(RST).
 *
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RSTENCmd(BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;
#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    status = FLASH_WR(device_num,SPI_RSTEN_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_RSTCmd -Reset Reset
 *
 * This function issues the Software Reset command immediately following a RSTEN command,
 * initiates the software reset process.
 *
 *
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RSTCmd(BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;
#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    status = FLASH_WR(device_num,SPI_RESET_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_RDQIDCmd - Read access manufacturer id ,device id and CFI in Quad All Mode
 *
 * This function issues the RDQID command.
 *
 * RETURNS: SLLD_OK or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RDQIDCmd
(
        BYTE  device_num,            /* device number */
        BYTE      *target,              /* variable in which to store read data */
        BYTECOUNT  len_in_bytes         /* number of bytes to read */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_RDQID_CMD, (ADDRESS)ADDRESS_NOT_USED, target, len_in_bytes);

    return(status);
}







/******************************************************************************
 *
 * slld_EPR_Cmd - Sector Erase resume  ,Sector Erase/Program resume for FSS,only available when set CR3NV[2] to 1.
 * if CR3NV[2] is 0 ,can using resume command 0x70 or 0x8a slld_ERS_RESCmd() or slld_PGRSCmd().
 * This function issues the Sector Erase resume command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_EPR_Cmd (BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_EPR_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_EPS_Cmd - Sector Erase suspend ,Sector Erase/Program suspend suspend for FSS
 *
 * This function issues the Sector Erase suspend command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_EPS_Cmd (BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_EPS_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}


SLLD_STATUS slld_ReadOp
(
        BYTE  device_num,            /* device number */
        ADDRESS   sys_addr,          /* device address given by system */
        BYTE     *target,            /* variable in which to store read data */
        BYTECOUNT len_in_bytes       /* number of bytes to read */
)
{
    SLLD_STATUS         status = SLLD_OK;
    unsigned long        length;
#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    while (len_in_bytes)
    {
#ifdef CONTROLLER_BUFFER_SIZE
        if (len_in_bytes < CONTROLLER_BUFFER_SIZE)
            length = len_in_bytes;
        else
            length = CONTROLLER_BUFFER_SIZE;
#else
        length = len_in_bytes;
#endif

#ifdef USE_4B_ADDR_CMDS
#ifdef USE_FAST_READ
        status = FLASH_RD(device_num,SPI_FAST_READ_4B_CMD, sys_addr, target, length);
#elif defined USE_DUAL_READ
        status = FLASH_RD(device_num,SPI_DUALIO_RD_4B_CMD, sys_addr, target, length);
#elif defined USE_QUAD_READ
        status = FLASH_RD(device_num,SPI_QUADIO_RD_4B_CMD, sys_addr, target, length);
#else
        status = FLASH_RD(device_num,SPI_READ_4B_CMD, sys_addr, target, length);
#endif
#else //#ifdef USE_4B_ADDR_CMDS
#ifdef USE_FAST_READ
        status = FLASH_RD(device_num,SPI_FAST_READ_CMD, sys_addr, target, length);
#elif defined USE_DUAL_READ
        status = FLASH_RD(device_num,SPI_DUALIO_RD_CMD, sys_addr, target, length);
#elif defined USE_QUAD_READ
        status = FLASH_RD(device_num,SPI_QUADIO_RD_CMD, sys_addr, target, length);
#else
        status = FLASH_RD(device_num,SPI_READ_CMD, sys_addr, target, length);
#endif
#endif

        len_in_bytes -= length;
        sys_addr += length;
        target += length;
    }

    return(status);
}


SLLD_STATUS slld_WriteOp
(
        BYTE  device_num,            /* device number */
        ADDRESS     sys_addr,                 /* device address given by system */
        BYTE       *data_buf,                 /* variable containing data to program */
        BYTECOUNT   len_in_bytes,             /* number of bytes to program */
        DEVSTATUS  *dev_status_ptr            /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;
    unsigned long length;

    while(len_in_bytes)
    {
        if ((sys_addr % PAGE_SIZE) != 0) {
            length = sys_addr % PAGE_SIZE;
            if (length > len_in_bytes)
                length = len_in_bytes;
        } else {
            if(len_in_bytes < PAGE_SIZE)
                length = len_in_bytes;
            else
                length = PAGE_SIZE;
        }

        status = slld_WRENCmd(device_num);
        if(status != SLLD_OK)
            return(status);
#ifdef USE_4B_ADDR_CMDS
#ifdef USE_QUAD_WRITE
        status = slld_QPP_4BCmd(device_num, sys_addr, data_buf, length);
#else
        status = slld_PP_4BCmd(device_num, sys_addr, data_buf, length);
#endif
#else //#ifdef USE_4B_ADDR_CMDS
#ifdef USE_QUAD_WRITE
        status = slld_QPPCmd(device_num, sys_addr, data_buf, length);
#else
        status = slld_PPCmd(device_num, sys_addr, data_buf, length);
#endif
#endif

        if(status != SLLD_OK)
            return(status);

        status = slld_Poll(device_num, dev_status_ptr);
        if(status != SLLD_OK)
            return(status);

        status = slld_WRDICmd(device_num);
        if(status != SLLD_OK)
            return(status);

        len_in_bytes -= length;
        sys_addr += length;
        data_buf += length;
    }

    return(status);
}

/******************************************************************************
 * slld_ReadSFDPCmd - Read Serial Flash Discoverable Parameter
 * This function issues the Read SFDP command to SPI Flash
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_ReadSFDPCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS    sys_addr,        // device address given by system
        BYTE      *read_buf         // data buffer
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    // read 256-byte serial flash discoverable parameter (SFDP) register
    status = FLASH_RD(device_num,SPI_READ_SFDP_CMD, sys_addr, read_buf, 256);

    return(status);
}





/******************************************************************************
 *
 * slld_GetDevNumFromAddr -help user to get device number from address when target device with multiple chip select
 *
 * sys_addr       device address given by system, sys_addr may includes system base address + device offset address
 * device_num   device number based on sys_addr
 *     device_num = 0x00, drive CS[0]
 *     device_num = 0x01, drive CS[1]
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_GetDevNumFromAddr
(
        ADDRESS     sys_addr,                  /* device address given by system */
        BYTE  *device_num                        /* device number */
)
{
    SLLD_STATUS status = SLLD_OK;

    *device_num = (sys_addr & BASE_ADDR_MASK) >> BASE_ADDR_SHIFT_BIT;
    
    return status;
}

/******************************************************************************
 *
 * slld_RDCR2Cmd - Read from the configuration register -2
 *
 * This function issues the RDCR2 command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RDCR2Cmd
(
        BYTE  device_num,            /* device number */
        BYTE    *target                   /* variable in which to store read data */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_RDCR2_CMD, ADDRESS_NOT_USED, target, 1);

    return(status);
}

/******************************************************************************
 *
 * slld_RDCR3Cmd - Read from the configuration register -3
 *
 * This function issues the RDCR3 command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RDCR3Cmd
(
        BYTE  device_num,            /* device number */
        BYTE    *target                   /* variable in which to store read data */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_RDCR3_CMD, ADDRESS_NOT_USED, target, 1);

    return(status);
}

/******************************************************************************
 *
 * slld_RASPCmd - Read IRP Register
 *
 * This function issues the IRPRD command to SPI Flash and reads from IRP register.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_IRPRDCmd
(
        BYTE      device_num,                     //device number
        WORD    *target                        /* variable in which to store read data */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_IRPRD_CMD, (ADDRESS)ADDRESS_NOT_USED, (BYTE *)target, 2);

    return(status);
}

/******************************************************************************
 *
 * slld_IRPPCmd - Write to IRP Register
 *
 * This function issues the Write IRP register command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_IRPPCmd
(
        BYTE  device_num,            /* device number */
        WORD    *irp_val                 /* variable containing data to program to the IRP register */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_IRPP_CMD, ADDRESS_NOT_USED, (BYTE *)irp_val, 2);

    return(status);
}

/******************************************************************************
 *
 * slld_IRPPOP - Write to IRP Register operation
 *
 * This function issues the Write IRP register command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_IRPPOp
(
        BYTE  device_num,            /* device number */
        WORD       *irp_val,             /* variable containing data to program to the IRP register */
        DEVSTATUS  *dev_status_ptr       /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_IRPPCmd(device_num, irp_val);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);       /* just in case IRPPOp is operated on protected area */

    return(status);
}

/******************************************************************************
 *
 * slld_QPIENCmd - Enter QPI
 *
 * This function issues the QPIEN command to enter the QPI mode.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPIENCmd
(
        BYTE  device_num            /* device number */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_QPIEN_CMD, ADDRESS_NOT_USED, 0, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_QPIEXCmd - Exit QPI
 *
 * This function issues the QPIEX command to exit the QPI mode.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPIEXCmd
(
        BYTE  device_num            /* device number */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_QPIEX_CMD, ADDRESS_NOT_USED, 0, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_4BENCmd -enter 4-byte Address Mode command sets the volatile Address Length bit (CR2V[0]) to 1 to
 * change most 3-byte address commands to require 4 bytes of address.
 *
 * This function issues the enter 4-byte Address Mode command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4BENCmd (BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_4BEN_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_4BEXCmd -exit 4-byte Address Mode command sets the volatile Address Length bit (CR2V[0]) to 0.
 *
 * This function issues the exit 4-byte Address Mode command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4BEXCmd (BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_4BEX_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_SECRRCmd - Security Region read
 *
 * This function issues the Security Region read command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_SECRRCmd 
(
        BYTE      device_num,                     //device number
        ADDRESS   sys_addr,                      // device address given by system
        BYTE     *read_buf,                      // data buffer
        BYTECOUNT len_in_bytes                   // number of bytes
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_READ(device_num,SPI_SECRR_CMD, sys_addr, read_buf, len_in_bytes);
    return(status);
}

/******************************************************************************
 *
 * slld_SECRPCmd - Security Region program
 *
 * This function issues the Security Region program command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_SECRPCmd 
(
        BYTE      device_num,                     //device number
        ADDRESS   sys_addr,                 // device address given by system
        BYTE     *program_buf,              // data buffer
        BYTECOUNT len_in_bytes              // number of bytes
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_SECRP_CMD, sys_addr, program_buf, len_in_bytes);
    return(status);
}

/******************************************************************************
 *
 * slld_SECRPOp - Security Region program operation
 *
 * This function issues the SECRP command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_SECRPOp
(
        BYTE      device_num,                     //device number
        ADDRESS          sys_addr,                           // device address given by system
        BYTE             *program_buf,                       // data buffer
        BYTECOUNT        len_in_bytes,                       // number of bytes
        DEVSTATUS        *dev_status_ptr                     // variable to store device status
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_SECRPCmd(device_num, sys_addr, program_buf, len_in_bytes);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);        // just in case SEOp is operated on a protected area

    return(status);
}

/******************************************************************************
 *
 * slld_SECRECmd - Security Region erase
 *
 * This function issues the Security Region erase command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_SECRECmd
(
        BYTE      device_num,                     //device number
        ADDRESS   sys_addr                  // device address given by system
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_SECRE_CMD, sys_addr, BUFFER_NOT_USED, 0);
    return(status);
}

/******************************************************************************
 *
 * slld_SECREOp - Security Region erase operation
 *
 * This function issues the SECRE command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_SECREOp
(
        BYTE            device_num,                     //device number
        ADDRESS     sys_addr,                 // device address given by system
        DEVSTATUS  *dev_status_ptr            // variable to store device status
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_SECRECmd(device_num, sys_addr);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);        // just in case SEOp is operated on a protected area

    return(status);
}

/******************************************************************************
 *
 * slld_RUIDCmd - Read Unique ID Number
 *
 * This function issues the Read Unique ID command to SPI Flash and read 64-bit Unique Serial Number
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RUIDCmd
(
        BYTE      device_num,                     //device number
        BYTE      *read_buf        // data buffer
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    // read 64-bit number that is unique
    status = FLASH_RD(device_num,SPI_RUID_CMD, ADDRESS_NOT_USED, read_buf, 8);

    return(status);
}

/******************************************************************************
 *
 * slld_WRENVCmd - Write Enable for Volatile Status and Configure Register
 *
 * This function issues the WRENV command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRENVCmd (BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_WRENV_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_IBLRDCmd - IBL read.
 *
 * This function issues the reading the state of each IBL bit protection.
 *
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_IBLRDCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr,               /* device address given by system */
        BYTE                *target           /*variable in which to store read data*/
)
{
    SLLD_STATUS status = SLLD_OK;
#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    status = FLASH_RD(device_num,SPI_IBLRD_CMD, sys_addr, target, 1);

    return(status);
}

/******************************************************************************
 *
 * slld_4IBLRDCmd - IBL read - 4 Bytes address.
 *
 * This function issues the reading the state of each IBL bit protection.
 *
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4IBLRDCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr,               /* device address given by system */
        BYTE                *target           /*variable in which to store read data*/
)
{
    SLLD_STATUS status = SLLD_OK;
#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif
    status = FLASH_RD(device_num,SPI_4IBLRD_CMD, sys_addr, target, 1);

    return(status);
}

/******************************************************************************
 *
 * slld_IBLCmd - IBL lock
 *
 * This function issues the IBL lock command to sets the selected IBL bit to "0" protecting each related 
 * sector / block.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_IBLCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr               /* device address given by system */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_IBL_CMD, sys_addr, 0, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_4IBLCmd - IBL lock -4 Bytes address
 *
 * This function issues the IBL lock command to sets the selected IBL bit to "0" protecting each related 
 * sector / block.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4IBLCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr               /* device address given by system */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_4IBL_CMD, sys_addr, 0, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_IBLOP - IBL lock operation
 *
 * This function issues the IBL lock command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_IBLOp
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr,               /* device address given by system */
        DEVSTATUS  *dev_status_ptr       /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifndef USE_4B_ADDR_CMDS
    status = slld_IBLCmd(device_num, sys_addr);
#else
    status = slld_4IBLCmd(device_num, sys_addr);
#endif
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    return(status);
}

/******************************************************************************
 *
 * slld_IBULCmd - IBL unlock
 *
 * This function issues the IBL unlock command to sets the selected IBL bit to "1" unprotecting each related 
 * sector / block.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_IBULCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr               /* device address given by system */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_IBUL_CMD, sys_addr, 0, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_4IBLCmd - IBL unlock -4 Bytes address
 *
 * This function issues the IBL unlock command to sets the selected IBL bit to "1" unprotecting each related 
 * sector / block.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4IBULCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr               /* device address given by system */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_4IBUL_CMD, sys_addr, 0, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_IBULOP - IBL unlock operation
 *
 * This function issues the IBL unlock command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_IBULOp
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr,               /* device address given by system */
        DEVSTATUS  *dev_status_ptr       /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifndef USE_4B_ADDR_CMDS
    status = slld_IBULCmd(device_num, sys_addr);
#else
    status = slld_4IBULCmd(device_num, sys_addr);
#endif
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    return(status);
}

/******************************************************************************
 *
 * slld_GBLCmd - Global IBL lock
 *
 * This function issues the global IBL lock command to sets all IBL bit to "0" protecting all sectors / blocks.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_GBLCmd
(
        BYTE  device_num            /* device number */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_GBL_CMD, ADDRESS_NOT_USED, 0, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_GBLOP - Global IBL lock operation
 *
 * This function issues the global IBL lock command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_GBLOp
(
        BYTE  device_num,            /* device number */
        DEVSTATUS  *dev_status_ptr       /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_GBLCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    return(status);
}

/******************************************************************************
 *
 * slld_GBULCmd - Global IBL unlock
 *
 * This function issues the global IBL unlock command to sets all IBL bit to "1" unprotecting all sectors / blocks.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_GBULCmd
(
        BYTE  device_num            /* device number */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_GBUL_CMD, ADDRESS_NOT_USED, 0, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_GBULOP - Global IBL unlock operation
 *
 * This function issues the global IBL unlock command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_GBULOp
(
        BYTE  device_num,            /* device number */
        DEVSTATUS  *dev_status_ptr       /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_GBULCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    return(status);
}

/******************************************************************************
 *
 * slld_SPRPCmd - set pointer region protection
 *
 * This function issues the SPRP command to sets pointer region protection.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SPRPCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr               /* device address given by system */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_SPRP_CMD, sys_addr, 0, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_4SPRPCmd - set pointer region protection - 4 Bytes address
 *
 * This function issues the 4SPRP command to sets pointer region protection.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_4SPRPCmd
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr               /* device address given by system */
)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_4SPRP_CMD, sys_addr, 0, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_SPRPOp - set pointer region protection operation
 *
 * This function issues the SPRP command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SPRPOp
(
        BYTE  device_num,            /* device number */
        ADDRESS  sys_addr,               /* device address given by system */
        DEVSTATUS  *dev_status_ptr       /* variable to store device status */
)
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd(device_num);
    if(status != SLLD_OK)
        return(status);

    status = slld_4SPRPCmd(device_num, sys_addr);
    if(status != SLLD_OK)
        return(status);

    status = slld_Poll(device_num, dev_status_ptr);
    if(status != SLLD_OK)
        return(status);

    status = slld_WRDICmd(device_num);
    return(status);
}

/******************************************************************************
 *
 * slld_PRLCmd - Protection register lock (NVLOCK bit write)
 *
 * This function issues the PRL command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PRLCmd(BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_PRL_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return(status);
}

/******************************************************************************
 *
 * slld_PRRDCmd - Protection register Read
 *
 * This function issues the PRRD command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PRRDCmd
(
        BYTE  device_num,            /* device number */
        BYTE      *data_buf                  /* variable containing data to program */
)
{
    SLLD_STATUS         status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_RD(device_num,SPI_PRRD_CMD, ADDRESS_NOT_USED, data_buf, 1);

    return(status);
}

/******************************************************************************
 *
 * slld_BSLCmd - Set Burst Length
 *
 * This function issues the Set Burst Length command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SBLCmd
(
        BYTE  device_num,            /* device number */
        BYTE      *wrapbit_buf                     // variable in which to store wrapbit data
)
{
    SLLD_STATUS status = SLLD_OK;
    BYTE buf[4]={0xff,0xff,0xff,0xff};
    
    buf[3] = *wrapbit_buf;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_SETBURSTWRAP_CMD, ADDRESS_NOT_USED, buf, 4);

    return(status);
}

/******************************************************************************
 *
 * slld_MBRCmd - Mode Bit Reset
 *
 * This function issues MBR command toreturn the device from continuous high performance read mode 
 * back to normal standby awaiting any new command.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_MBRCmd (BYTE device_num)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if(sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return(status);
    }
#endif

    status = FLASH_WR(device_num,SPI_MBR_CMD, ADDRESS_NOT_USED, 0, 0);

    return(status);
}






