/******************************************************************************
* File Name: slld_hal.c
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
#include "cybsp.h"
#include "slld_fll.h"
#include "slld.h"
#include "slld_hal.h"
#include "cy_pdl.h"

#define mSPI_INTR_PRIORITY  (3U)

cy_stc_scb_spi_context_t mSPI_context;

volatile uint32_t spi_event = 0;

/*******************************************************************************
 * Function Name: mSPI_Interrupt
 *******************************************************************************
 *
 * Invokes the Cy_SCB_SPI_Interrupt() PDL driver function.
 *
 ******************************************************************************/
void mSPI_Interrupt(void)
{
    Cy_SCB_SPI_Interrupt(mSPI_HW, &mSPI_context);
}

static void callback(uint32_t event)
{
    spi_event = event;

}


//***************************************************************************
//  FLASH_READ - HAL read function
//
//  input : device_num            device number to which operation will be done
//          command               write a single command byte to flash
//          sys_addr              system address to be used
//          data_buffer           Pointer to the data buffer where to store the read data
//          Number_Of_Read_Bytes  number of bytes to be read
//
//  return value : status of the operation - FAIL or SUCCESS
//***************************************************************************
SLLD_STATUS FLASH_READ(BYTE  device_num,BYTE  command, ADDRESS sys_addr, BYTE  *data_buffer, int Number_Of_Read_Bytes )
{
    SLLD_STATUS status = SLLD_OK;
    //int Number_Of_Bytes = 0;
    uint8_t data[1024];
    uint16_t count=0;

    // Write the command - Insert code to write command to the device


    data[count++]=(uint8_t)command;

    // Write the address - Insert HAL specific code to write the address to the device
    // in the selected mode based on command type
    if (sys_addr != ADDRESS_NOT_USED)
    {
        switch (command)
        {
        case SPI_DUALIO_HPRD_CMD:
        {
            // Shift out the address two bits at a time in 3-bytes addressing scheme

            break;
        }
        case SPI_DUALIO_HPRD_4B_CMD:
        {
            // Shift out the address two bits at a time in 4-bytes addressing scheme

            break;
        }
        case SPI_QUADIO_HPRD_CMD:
        {
            // Shift out the address four bits at a time in 3-bytes addressing scheme

            break;
        }
        case SPI_QUADIO_HPRD_4B_CMD:
        {
            // Shift out the address four bits at a time in 4-bytes addressing scheme

            break;
        }
        case SPI_READ_4B_CMD:
        case SPI_FAST_READ_4B_CMD:
        case SPI_DUALIO_RD_4B_CMD:
        case SPI_QUADIO_RD_4B_CMD:
        {
            // Shift out the address one bit at a time in 4-bytes addressing scheme

            data[count++] = ((sys_addr & 0x00FF0000)>>24);
            data[count++] = ((sys_addr & 0x00FF0000)>>16);
            data[count++] = ((sys_addr & 0x0000FF00)>>8);
            data[count++]=((sys_addr & 0x000000FF)>>0);
            break;
        }
        default:
        {
            // Shift out the address one bit at a time in 3-bytes addressing scheme

            data[count++] = ((sys_addr & 0x00FF0000)>>16);
            data[count++] = ((sys_addr & 0x0000FF00)>>8);
            data[count++] = ((sys_addr & 0x000000FF)>>0);
            break;
        }
        }
    }

    // Write the bytes - Insert HAL specific code to write the bytes to the device
    switch (command)
    {
    case SPI_FAST_READ_CMD:
    case SPI_FAST_READ_4B_CMD:
    case SPI_DUALIO_RD_CMD:
    case SPI_DUALIO_RD_4B_CMD:
    case SPI_QUADIO_RD_CMD:
    case SPI_QUADIO_RD_4B_CMD:
    case SPI_OTPR_CMD:
    {
        // Number_Of_Bytes = 1;
        // Write a byte to the data bus

        data[count++]=0xFF;
        break;
    }
    case SPI_DUALIO_HPRD_CMD:
    case SPI_DUALIO_HPRD_4B_CMD:
    {
        // Number_Of_Bytes = 1;
        // Write a byte to the data bus - This is actually the mode bit

        data[count++]=0xFF;
        break;
    }
    case SPI_QUADIO_HPRD_CMD:
    case SPI_QUADIO_HPRD_4B_CMD:
    {
        // Number_Of_Bytes = 3;
        // Write 3 bytes to the data bus - The first byte is actually the mode bit

        data[count++]=0xFF;
        data[count++]=0xFF;
        data[count++]=0xFF;
        break;
    }
    case SPI_RES_CMD:
    {
        // Number_Of_Bytes = 3;
        // Write 3 bytes to the data bus

        data[count++]=0xFF;
        data[count++]=0xFF;
        data[count++]=0xFF;
        break;
    }
    default:
    {
        // Default to no  bytes written
        // Number_Of_Bytes = 0;
        break;
    }
    }

    // Read the data - Insert HAL specific code to read data from the device
    if (Number_Of_Read_Bytes != 0)
    {
        switch (command)
        {
        case SPI_DUALIO_RD_CMD:
        case SPI_DUALIO_RD_4B_CMD:
        case SPI_DUALIO_HPRD_CMD:
        case SPI_DUALIO_HPRD_4B_CMD:
        {
            // Read the data using dual mode

            break;
        }
        case SPI_QUADIO_RD_CMD:
        case SPI_QUADIO_RD_4B_CMD:
        case SPI_QUADIO_HPRD_CMD:
        case SPI_QUADIO_HPRD_4B_CMD:
        {
            // Read the data using quad mode

            break;
        }
        default:
        {
            // Read the data using the single mode


            spi_event = 0;
            status = (SLLD_STATUS)Cy_SCB_SPI_Transfer_Buffer(mSPI_HW, data, data_buffer, count,(Number_Of_Read_Bytes+count),0x00,&mSPI_context)  ;
            while(spi_event != CY_SCB_SPI_TRANSFER_CMPLT_EVENT);
            memcpy(data_buffer,data_buffer+count,Number_Of_Read_Bytes);
            break;
        }
        }
    }

    return(status);
}


//***************************************************************************
//  FLASH_WRITE - HAL write function
//
//  input : device_num               device number to which operation will be done
//          command                  write a single command byte to flash
//          sys_addr                 system address to be used
//          data_buffer              Pointer to the data buffer where to store the written data
//          Number_Of_Written_Bytes  number of bytes to be written
//
//  return value : status of the operation - FAIL or SUCCESS
//***************************************************************************
SLLD_STATUS FLASH_WRITE(BYTE device_num,BYTE  command,ADDRESS sys_addr,BYTE   *data_buffer,int Number_Of_Written_Bytes )
{
    SLLD_STATUS status = SLLD_OK;
    uint8_t data[1024];
    uint16_t count=0;

    // Write the command - Insert HAL specific code to write command to the device

    data[count++]=(uint8_t)command;

    // Write the address - Insert HAL specific code to write the address to the device
    // in the selected mode based on command type
    if (sys_addr != ADDRESS_NOT_USED)
    {
        switch (command)
        {
        case SPI_PP_4B_CMD:
        case SPI_QPP_4B_CMD:
        case SPI_P8E_4B_CMD:
        case SPI_SE_4B_CMD:
        {
            // Shift out the address one bit at a time in 4-bytes addressing scheme

            data[count++] = ((sys_addr & 0x00FF0000)>>24);
            data[count++] = ((sys_addr & 0x00FF0000)>>16);
            data[count++] = ((sys_addr & 0x0000FF00)>>8);
            data[count++]=((sys_addr & 0x000000FF)>>0);
            break;
        }
        default:
        {
            // Shift out the address one bit at a time in 3-bytes addressing scheme

            data[count++] = ((sys_addr & 0x00FF0000)>>16);
            data[count++] = ((sys_addr & 0x0000FF00)>>8);
            data[count++] = ((sys_addr & 0x000000FF)>>0);
            break;
        }
        }
    }

    // Write the data - Insert HAL specific code to write data to the device
    if (Number_Of_Written_Bytes != 0)
    {
        switch (command)
        {
        case SPI_QPP_CMD:
        case SPI_QPP_4B_CMD:
        {
            // Write the data using quad mode

            break;
        }
        default:
        {
            // Write the data using single mode

            memcpy(data+count,data_buffer,Number_Of_Written_Bytes);
            spi_event = 0;
            status = (SLLD_STATUS)Cy_SCB_SPI_Transfer(mSPI_HW, data, NULL,(Number_Of_Written_Bytes+count), &mSPI_context);
            while(spi_event != CY_SCB_SPI_TRANSFER_CMPLT_EVENT);
            break;
        }
        }
    }
    else
    {
        spi_event = 0;
        status = (SLLD_STATUS)Cy_SCB_SPI_Transfer(mSPI_HW, data, NULL,(Number_Of_Written_Bytes+count), &mSPI_context);
        while(spi_event != CY_SCB_SPI_TRANSFER_CMPLT_EVENT);

    }





    return(status);
}

SLLD_STATUS init_spi(void)
{
    cy_en_scb_spi_status_t result;

    cy_en_sysint_status_t sysSpistatus;

    /* Configure the SPI block */

    result = Cy_SCB_SPI_Init(mSPI_HW, &mSPI_config, &mSPI_context);

    if( result != CY_SCB_SPI_SUCCESS)
    {
        return(SLLD_E_HAL_ERROR);
    }

    /* Set active slave select to line 0 */
    Cy_SCB_SPI_SetActiveSlaveSelect(mSPI_HW, CY_SCB_SPI_SLAVE_SELECT0);

    /* Populate configuration structure */

    Cy_SCB_SPI_RegisterCallback    (mSPI_HW,callback,&mSPI_context );

    const cy_stc_sysint_t mSPI_SCB_IRQ_cfg =
    {
            .intrSrc      = mSPI_IRQ,
            .intrPriority = 1//mSPI_INTR_PRIORITY
    };

    /* Hook interrupt service routine and enable interrupt */
    sysSpistatus = Cy_SysInt_Init(&mSPI_SCB_IRQ_cfg, &mSPI_Interrupt);

    if(sysSpistatus != CY_SYSINT_SUCCESS)
    {
        return(SLLD_E_HAL_ERROR);
    }
    /* Enable interrupt in NVIC */
    NVIC_EnableIRQ(mSPI_IRQ);

    /* Enable the SPI Master block */
    Cy_SCB_SPI_Enable(mSPI_HW);

    /* Initialization completed */
    return(SLLD_OK);
}


/*****************************************************************************/



