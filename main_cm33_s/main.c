/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC™ Control C3M/P8 Out
*              of Box demo Example for ModusToolbox.
*
* Related Document: See README.md
*
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

/*******************************************************************************
* Header Files
********************************************************************************/
#include "oob_init_s.h"
/*******************************************************************************
* Macros
********************************************************************************/

/* These are the addresses where the core0 and core1 images are located. */
#define CORE0_IMAGE_ADDRESS    CYMEM_CM33_0_S_m33s_ppca0_nvm_C_S_START
#define CORE1_IMAGE_ADDRESS    CYMEM_CM33_0_S_m33s_ppca1_nvm_C_S_START
#define PPCA0_IMAGE_SIZE       CYMEM_CM33_0_S_ppca0_code_SIZE
#define PPCA1_IMAGE_SIZE       CYMEM_CM33_0_S_ppca1_code_SIZE

/*******************************************************************************
* Global Variables
********************************************************************************/

/* Debug UART variables */
static cy_stc_scb_uart_context_t    DEBUG_UART_context; /* DEBUG_UART context */
static mtb_hal_uart_t               DEBUG_UART_hal_obj; /* Debug DEBUG_UART HAL object */

/*******************************************************************************
* Function Prototypes
********************************************************************************/


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM33 CPU. The main function performs
* the following actions:
*
* 1. Initialize the UART component and button interrupt.
* 2. Show 11 demos navigation interfaces on the UART serial terminal.
* 3. "Hello world" demo (default demo) automatically.
* 4. Press the button SW4 to traverse in the state machine.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/

int main(void)
{
    cy_rslt_t   result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize and enable the UART */
    Cy_SCB_UART_Init(DEBUG_UART_HW, &DEBUG_UART_config, &DEBUG_UART_context);
    Cy_SCB_UART_Enable(DEBUG_UART_HW);

    /* Setup the HAL DEBUG_UART */
    result = mtb_hal_uart_setup(&DEBUG_UART_hal_obj, &DEBUG_UART_hal_config,
                                &DEBUG_UART_context, NULL);

    /* HAL DEBUG_UART init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize redirecting of low level IO */
    result = cy_retarget_io_init(&DEBUG_UART_hal_obj);

    /* retarget IO init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initializing the PPCA Configuration. */
    Cy_PPCA_CNFG_Init(PPCA_CNFG_HW, &PPCA_CNFG_config);

    /* Enabling the PPCA Configuration. */
    Cy_PPCA_Enable(PPCA_CNFG_HW);

   /* Enable exclusive access to the EPU resources based
    * on the provided resources allocation configuration. */
    Cy_PPCA_EPU_EnableExclusiveAccess(PPCA_EPU, true);

    /* Enable EPU */
    Cy_PPCA_EPU_Enable(PPCA_EPU);

    /* Initializing ATOP Analog reference */
    Cy_PPCA_AREF_Init(AREF_HW, &AREF_config);

    /* Enabling AREF */
    Cy_PPCA_AREF_Enable(AREF_HW);

    /* Initializing the debounce counter . */
    result = Cy_TCPWM_Counter_Init(debounce_counter_HW, debounce_counter_NUM,&debounce_counter_config);

    /* debounce counter init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable the debounce counter . */
    Cy_TCPWM_Counter_Enable(debounce_counter_HW, debounce_counter_NUM);

    /*Initializes the interrupt for button action. */
    result = Cy_SysInt_Init(&button_press_intr_config, button_press_intr_handler);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Clearing the pending button press interrupt before enabling . */
    NVIC_ClearPendingIRQ(button_press_intr_config.intrSrc);

    /* Enabling the button interrupt */
    NVIC_EnableIRQ(button_press_intr_config.intrSrc);

    /* Transmit header to the terminal */
    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");
    printf("************************************************************\r\n");
    printf("PSOC Control C3M/P8: Out-of-the-box (OOB) demo\r\n");
    printf("************************************************************\r\n\n");

   /* Print code example menu and read user selection. */
   printf("\r\n Please press the button 'SW4' to run the state machine.");
   printf("\r\n 1 : Hello world.");
   printf("\r\n 2 : Voltage_Current_Consumption.");
   printf("\r\n 3 : Differential ADC.");
   printf("\r\n 4 : Peak current control mode.");
   printf("\r\n 5 : PFC ZCD based PWM masking.");
   printf("\r\n 6 : Phase shift full bridge.");
   printf("\r\n 7 : HrPWM.");
   printf("\r\n 8 : ADC Filter.");
   printf("\r\n 9 : 3P3Z HW Filter.");
   printf("\r\n 10: CLB Custom logic block.");
   printf("\r\n 11: SPI based external flash test.\r\n");

   printf(" \r\n ");
   
    /* enable interrupts */
    __enable_irq();
    
    /* Initializing and starting PPCA CPU Core 0 and Core 1. */
    Cy_System_Init_CPU0((void*)CORE0_IMAGE_ADDRESS, PPCA0_IMAGE_SIZE);
    Cy_System_Init_CPU1((void*)CORE1_IMAGE_ADDRESS, PPCA1_IMAGE_SIZE);

    for (;;)
    {
        /* Printing the current state of the state machine */
        print_current_state();
        Cy_SysLib_Delay(10);
    }
}
/* The End */
