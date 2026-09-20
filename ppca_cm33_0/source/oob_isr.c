/******************************************************************************
* File Name: oob_isr.c
*
* Description: This file contains implementation of the interrupt service  
*              routine functions for the PSOC™ Control C3M/P8 Out of
*              Box demo code Example
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

#include "oob_isr.h"

/*******************************************************************************
* Global Variables for interrupt
*********************************************************************************/

/* Interrupt configuration structure of PWM for PCCM. */
cy_stc_sysint_t pwm_pccm_intr_config =
{
    .intrSrc = pwm_pccm_IRQ,
    .intrPriority = 2UL,
};

/* Interrupt configuration structure of PWM for PFC. */
cy_stc_sysint_t pwm_pfc_ac_intr_config =
{
    .intrSrc = pwm_pfc_ac_IRQ,
    .intrPriority = 1UL,
};

/* Interrupt configuration structure of ADC filter ISR */
cy_stc_sysint_t adc2_filter_intr_config =
{
    .intrSrc = ppca_0_epu_0_IRQ_EPU_0,
    .intrPriority = 1UL,
};

/* Interrupt configuration structure of PWM for PSFB. */
cy_stc_sysint_t pwm_psfb_intr_config =
{
    .intrSrc = pwm_psfb1_IRQ,
    .intrPriority = 3UL,
};

/* Interrupt configuration structure of PWM for HRPWM. */
cy_stc_sysint_t hrpwm_intr_config =
{
    .intrSrc = pwm_hrpwm_IRQ,
    .intrPriority = 3UL,
};

/* Interrupt configuration structure of Hardware filter ISR */
cy_stc_sysint_t hwfilter_intr_config =
{
    .intrSrc = ppca_0_epu_0_IRQ_EPU_1,
    .intrPriority = 1U,
};

/* Configure interrupt for generating sine wave */
cy_stc_sysint_t freq_mod_intr_config =
{
    .intrSrc = freq_mod_IRQ,
    .intrPriority = 1U,
};

/*******************************************************************************
* Global Variables
********************************************************************************/

/* Variable to hold the current state of the state machine */
volatile uint32_t *sm_state = (volatile uint32_t *)(PPCA_CPU0_M4_VAR_ADDRESS);

/* Shared variable for lock on switch case */
volatile uint32_t *sm_lock = (volatile uint32_t *)(PPCA_CPU0_M4_VAR_ADDRESS + 0x04);

/* Shared variable for the size of the sine-wave look-up table */
/* To change the output of DAC
 * for sine wave - '99'
 * for Rectified sine wave - '49' */
volatile uint32_t *sm_sine_counter = (volatile uint32_t *)(PPCA_CPU0_M4_VAR_ADDRESS + 0x08);

/* Variable for the size of sine array in PFC */
uint32_t   counter       = 0;

/* Variable to store the data sine table */
float sine_wave          = 0;

/* Lookup table for a sine wave in unsigned format. */
uint32_t sinewave_pattern[] = { 0x7FF, 0x880, 0x900, 0x97F, 0x9FC, 0xA78, 0xAF1,
           0xB67, 0xBD9, 0xC48, 0xCB2, 0xD18, 0xD79, 0xDD4, 0xE29, 0xE77, 0xEC0,
           0xF01, 0xF3C, 0xF6F, 0xF9A, 0xFBE, 0xFDA, 0xFEE, 0xFFA, 0xFFF, 0xFFA,
           0xFEE, 0xFDA, 0xFBE, 0xF9A, 0xF6F, 0xF3C, 0xF01, 0xEC0, 0xE77, 0xE29,
           0xDD4, 0xD79, 0xD18, 0xCB2, 0xC48, 0xBD9, 0xB67, 0xAF1, 0xA78, 0x9FC,
           0x97F, 0x900, 0x880, 0x7FF, 0x77E, 0x6FE, 0x67F, 0x602, 0x586, 0x50D,
           0x497, 0x425, 0x3B6, 0x34C, 0x2E6, 0x285, 0x22A, 0x1D5, 0x187, 0x13E,
           0x0FD, 0x0C2, 0x08F, 0x064, 0x040, 0x024, 0x010, 0x004, 0x000, 0x004,
           0x010, 0x024, 0x040, 0x064, 0x08F, 0x0C2, 0x0FD, 0x13E, 0x187, 0x1D5,
           0x22A, 0x285, 0x2E6, 0x34C, 0x3B6, 0x425, 0x497, 0x50D, 0x586, 0x602,
           0x67F, 0x6FE, 0x77E };
           
/* Variable for the output of the filter */
uint32_t filter_data     = 0;

/* Variable to create delay in phase shift full bridge test */
float delay              = 0;

/* Variable for calculating the average of delay in phase shift full bridge test */
float delay_avg          = 0;

/* Variable for the output of the hardware filter */
uint32_t hw_filter_out   = 0;

/* Variable for the period of sine wave interrupt */
uint32_t period      = SINE_INTR_PERIOD_MIN;

/*******************************************************************************
* ISR Lookup Tables
********************************************************************************/

/* Function pointer type for ISR initialization */
typedef void (*isr_init_func_t)(void);

/* ISR initialization table entry */
typedef struct {
    enum states state;
    isr_init_func_t isr_init_handler;
} isr_table_entry_t;

/* ISR initialization lookup table */
const isr_table_entry_t isr_init_table[] = {
    {Hello_world,                 NULL},           /* No ISR needed */
    {Voltage_Current_Consumption, NULL},           /* No ISR needed */
    {Differential_ADC,            NULL},           /* No ISR needed */
    {PCCM_test,                   pccm_isr_int},
    {PFC_test,                    pfc_isr_int},
    {PSFB_test,                   psfb_isr_int},
    {HR_PWM,                      hrpwm_isr_int},
    {Filtering,                   filter_isr_int},
    {Filtering_3P3Z,              hw_isr_int},
    {CLB,                         NULL},           /* No ISR needed */
    {SPI_EXTERNAL_FLASH,          NULL},           /* No ISR needed */
};

#define ISR_TABLE_SIZE (sizeof(isr_init_table) / sizeof(isr_table_entry_t))

/*******************************************************************************
* Helper Functions
********************************************************************************/

/*******************************************************************************
* Function Name: get_isr_init_handler
********************************************************************************
* Summary:
*  Finds the ISR initialization handler for the given state.
*
* Parameters:
*  state: The state to search for
*
* Return:
*  Function pointer to ISR init handler, or NULL if not found/needed
*******************************************************************************/
static isr_init_func_t get_isr_init_handler(enum states state)
{
    for (uint32_t i = 0; i < ISR_TABLE_SIZE; i++)
    {
        if (isr_init_table[i].state == state)
        {
            return isr_init_table[i].isr_init_handler;
        }
    }
    return NULL;
}

/*******************************************************************************
* Function definition
********************************************************************************/

/*******************************************************************************
* Function Name: isr_init
********************************************************************************
* Summary:
*  Initializes interrupts based on current state using lookup table.
*  Disables all ISRs first, then enables only the required ones.
*
* Parameters:
*  void
*
* Return:
*  void
*******************************************************************************/
void isr_init(void)
{
    /* Get the ISR initialization handler for the current state */
    isr_init_func_t handler = get_isr_init_handler((enum states)*sm_state);
    
    /* If handler exists, disable all ISRs first, then enable the required one */
    if (handler != NULL)
    {
        /* Disable all ISRs before enabling the specific one */
        disable_isr();
        
        /* Call the state-specific ISR initialization */
        handler();
    }
    
    /* Release the lock to allow main core to continue */
    *sm_lock = 0;
}

/*******************************************************************************
* Function Name: pccm_isr_int
********************************************************************************
* Summary:
*
* Interrupt initialization for PCCM test.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void pccm_isr_int(void)
{
    cy_en_sysint_status_t status;

    /* Initializes the interrupt. */
    status = Cy_SysInt_Init(&pwm_pccm_intr_config, &pwm_pccm_isr);

    /* Initialization fail. */
    if (CY_SYSINT_SUCCESS != status)
    {
      CY_ASSERT(0);
    }

    /* Clearing any pending interrupt.*/
    NVIC_ClearPendingIRQ(pwm_pccm_intr_config.intrSrc);

    /* Enabling the interrupt */
    NVIC_EnableIRQ(pwm_pccm_intr_config.intrSrc);
}

/*******************************************************************************
* Function Name: pfc_isr_int
********************************************************************************
* Summary:
*
* Interrupt initialization for PFC test.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void pfc_isr_int(void)
{
    cy_en_sysint_status_t status;

    /*Initializing the interrupt */
    status = Cy_SysInt_Init(&pwm_pfc_ac_intr_config, &rectified_ac_generator_isr);

    /* Initialization fail. */
    if (CY_SYSINT_SUCCESS != status)
    {
      CY_ASSERT(0);
    }

    /* Clearing any pending interrupt */
    NVIC_ClearPendingIRQ(pwm_pfc_ac_intr_config.intrSrc);

    /* Enable the interrupt */
    NVIC_EnableIRQ(pwm_pfc_ac_intr_config.intrSrc);
}

/*******************************************************************************
* Function Name: psfb_isr_int
********************************************************************************
* Summary:
*
* Interrupt initialization for PSFB test.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void psfb_isr_int(void)
{
    cy_en_sysint_status_t status;

    status = Cy_SysInt_Init(&pwm_psfb_intr_config, &psfb_isr);

    /* Initialization fail. */
    if (CY_SYSINT_SUCCESS != status)
    {
      CY_ASSERT(0);
    }

    /* Clearing any pending interrupt. */
    NVIC_ClearPendingIRQ(pwm_psfb_intr_config.intrSrc);

    /* Enabling the interrupt. */
    NVIC_EnableIRQ(pwm_psfb_intr_config.intrSrc);
}

/*******************************************************************************
* Function Name: hrpwm_isr_int
********************************************************************************
* Summary:
*
* Interrupt initialization for hrpwm test.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void hrpwm_isr_int(void)
{
    cy_en_sysint_status_t status;

    /* Initializes the interrupt */
    status = Cy_SysInt_Init(&hrpwm_intr_config, hrpwm_isr);

    /* Initialization fail. */
    if (CY_SYSINT_SUCCESS != status)        
    {
      CY_ASSERT(0);
    }

    /* Clear the interrupt */
    NVIC_ClearPendingIRQ(hrpwm_intr_config.intrSrc);

    /* Enabling the interrupt */
    NVIC_EnableIRQ(hrpwm_intr_config.intrSrc);
}

/*******************************************************************************
* Function Name: filter_isr_int
********************************************************************************
* Summary:
*
* Interrupt initialization for ADC filter test.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void filter_isr_int(void)
{
    cy_en_sysint_status_t status;
    
    status = Cy_SysInt_Init(&pwm_pfc_ac_intr_config, &rectified_ac_generator_isr);

    /* Initialization fail. */
    if (CY_SYSINT_SUCCESS != status)
    {
      CY_ASSERT(0);
    }

    /* Clearing any pending interrupt */
    NVIC_ClearPendingIRQ(pwm_pfc_ac_intr_config.intrSrc);

    /* Enable the interrupt */
    NVIC_EnableIRQ(pwm_pfc_ac_intr_config.intrSrc);
    
    /* Initializes the interrupt */
    status = Cy_SysInt_Init(&adc2_filter_intr_config, &adc2_fltr_isr);

    /* Initialization fail. */
    if (CY_SYSINT_SUCCESS != status)
    {
      CY_ASSERT(0);
    }

    /* Clear the interrupt */
    NVIC_ClearPendingIRQ(adc2_filter_intr_config.intrSrc);

    /* Enabling the interrupt */
    NVIC_EnableIRQ(adc2_filter_intr_config.intrSrc);
}

/*******************************************************************************
* Function Name: hw_isr_int
********************************************************************************
* Summary:
*
* Interrupt initialization for HW 3P3Z filter test.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void hw_isr_int(void)
{
    cy_en_sysint_status_t status;

    /* Rectified AC sine wave generation interrupt initialization */
    status = Cy_SysInt_Init(&pwm_pfc_ac_intr_config, &rectified_ac_generator_isr);
    
    /* Initialization fail. */
    if (CY_SYSINT_SUCCESS != status)
    {
      CY_ASSERT(0);
    }

    /* HW filter interrupt initialization */
    status = Cy_SysInt_Init(&hwfilter_intr_config, &hw_fltr_isr);

    /* Initialization fail. */
    if (CY_SYSINT_SUCCESS != status)
    {
      CY_ASSERT(0);
    }

    /* Frequency modulation interrupt initialization */
    status = Cy_SysInt_Init(&freq_mod_intr_config, &freq_mod_isr);

    /* Initialization fail. */
    if (CY_SYSINT_SUCCESS != status)
    {
      CY_ASSERT(0);
    }

    /* Clear the rectified AC sine wave generation interrupt */
    NVIC_ClearPendingIRQ(pwm_pfc_ac_intr_config.intrSrc);

    /* Clear the HW filter interrupt */
    NVIC_ClearPendingIRQ(hwfilter_intr_config.intrSrc);

    /* Clear the frequency modulation interrupt */
    NVIC_ClearPendingIRQ(freq_mod_intr_config.intrSrc);
    
    /* Enabling the rectified AC sine wave generation interrupt */
    NVIC_EnableIRQ(pwm_pfc_ac_intr_config.intrSrc);

    /* Enabling the HW filter interrupt */
    NVIC_EnableIRQ(hwfilter_intr_config.intrSrc);

    /* Enabling the frequency modulation interrupt */
    NVIC_EnableIRQ(freq_mod_intr_config.intrSrc);
}

/*******************************************************************************
* Function Name: pwm_pccm_isr
********************************************************************************
* Summary:
*
* Interrupt service routine for PCCM test.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void pwm_pccm_isr(void)
{
    /* Clearing the PWM capture interrupt. */
    uint32_t interrupts = Cy_TCPWM_GetInterruptStatusMasked(pwm_pccm_HW, pwm_pccm_NUM);
    Cy_TCPWM_ClearInterrupt(pwm_pccm_HW, pwm_pccm_NUM, interrupts);

    /* Triggering the slope generation at every terminal count. */
    Cy_PPCA_DCSG_SLGEN_Trigger_Ramp_Generation(DCSG_SL_HW);
}

/*******************************************************************************
* Function Name: rectified_ac_generator_isr
********************************************************************************
* Summary:
*
* Interrupt service routine to generate sine wave for PFC, ADC filter and 3p3z
* Hw filter test.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void rectified_ac_generator_isr(void)
{
    /* Clearing the PWM capture interrupt. */
    uint32_t interrupts = Cy_TCPWM_GetInterruptStatusMasked(pwm_pfc_ac_HW,pwm_pfc_ac_NUM);
    Cy_TCPWM_ClearInterrupt(pwm_pfc_ac_HW,pwm_pfc_ac_NUM, interrupts);
    
    /* Counter to iterate through the lookup table */
    if(counter >= *sm_sine_counter)
    {
        counter = 0;
    }
    else
    {
        counter+=1;
    }

    /* Rectified sine for PFC and offset sine for ADC filter test */
    if(*sm_state == PFC_test)
    {
        sine_wave = (sinewave_pattern[counter]);
    }
    else
    {
        if(*sm_state == Filtering)
        {
           sine_wave = (sinewave_pattern[counter])*0.4 + 0x7FF;
        }
    }

    /* Sine wave for 3P3Z filter */
    if(*sm_state == Filtering_3P3Z)
    {
       sine_wave = (sinewave_pattern[counter]);
    }

    /* Updating the DAC output */
    Cy_PPCA_DAC_Set_DACOut(DAC_HW, (uint16_t)sine_wave);
}

/*******************************************************************************
* Function Name: psfb_isr
********************************************************************************
* Summary:
*
* This the interrupt handler for phase shift full bridge use case. In this phase
* delay is calculated.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void psfb_isr(void)
{
    /* Clearing the interrupt. */
    uint32_t interrupts = Cy_TCPWM_GetInterruptStatusMasked(pwm_psfb1_HW, pwm_psfb1_NUM);
    Cy_TCPWM_ClearInterrupt(pwm_psfb1_HW, pwm_psfb1_NUM, interrupts);

    /* Reading the potentiometer using the ADC value. */
    uint32_t ADC =  Cy_PPCA_ADC_Read_ADC_Data(ADC3_HW, 4);
    uint32_t period = Cy_TCPWM_PWM_GetPeriod0(pwm_psfb2_HW, pwm_psfb2_NUM);

    /* Phase delay calculation */
    delay = (period - (ADC*period/4095));
    delay_avg = ((delay_avg - ((delay_avg - delay) / 8)));

    /* Setting the counter value */
    Cy_TCPWM_Counter_SetCounter(pwm_psfb2_HW, pwm_psfb2_NUM,(uint32_t)delay_avg);
}

/*******************************************************************************
* Function Name: adc2_fltr_isr
********************************************************************************
* Summary:
*
* This is the interrupt handler for the ADC filter use case. In this function
* the output of the filter is fed to the DAC to visualize the output.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void adc2_fltr_isr(void)
{
    /* Clearing the interrupt. */
    Cy_PPCA_EPU_ClearInterrupt(ppca_0_epu_0_EPU_IRQ0_HW);

    if(*sm_state == Filtering)
    {
        /* Reading the filter output data */
       filter_data = (Cy_PPCA_ADC_Filter_LPF_Output(ADC2_FLT_HW) >> 4)- 2047;

       /* Write the filter output to the DAC buffer */
       Cy_PPCA_DAC_Set_DACOut(DAC_FLT_HW, filter_data);
    }

    if(*sm_state == Filtering_3P3Z)
    {
        /* Reading the ADC data */
       filter_data = (Cy_PPCA_ADC_Read_ADC_Data(ADC2_HW, 0) << 4);

       /* Writing the ADC data to the hardware filter */
       Cy_PPCA_HWFILT3P3Z_Write_DATA_IN1(HW_FLT_LF_HW, filter_data);
       Cy_PPCA_HWFILT3P3Z_Write_DATA_IN0(HW_FLT_LF_HW, 0);
    }
}

/*******************************************************************************
* Function Name: hrpwm_isr
********************************************************************************
* Summary:
*
*  This is the interrupt handler for the HrPWM use case. One potentiometer is
*  used to set the duty cycle. One potentiometer is used to set the period.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void hrpwm_isr(void)
{
    /* Clearing the interrupt. */
    uint32_t interrupts = Cy_TCPWM_GetInterruptStatusMasked(pwm_hrpwm_HW, pwm_hrpwm_NUM);
    Cy_TCPWM_ClearInterrupt(pwm_hrpwm_HW, pwm_hrpwm_NUM, interrupts);

    /* Calculate the new duty and period value */
    uint32_t period = 256000 - 56*(Cy_PPCA_ADC_Read_ADC_Data(ADC0_HW, 0));
    uint32_t duty = (uint32_t)((period/4095)*Cy_PPCA_ADC_Read_ADC_Data(ADC0_HW, 1));

    /* Updating the buffer period and compare register with new values */
    Cy_TCPWM_PWM_SetPeriod1(pwm_hrpwm_HW, pwm_hrpwm_NUM,period);
    Cy_TCPWM_PWM_SetCompare0BufVal(pwm_hrpwm_HW, pwm_hrpwm_NUM,duty);
}

/*******************************************************************************
* Function Name: hw_fltr_isr
********************************************************************************
* Summary:
*
* This is the interrupt handler for the HW 3P3Z filter use case. In this function
* the output of the filter is fed to the DAC to visualize the output.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void hw_fltr_isr(void)
{
    /* Clearing the interrupt. */
    Cy_PPCA_EPU_ClearInterrupt(PPCA_EPU_EPU_IRQ1);

    /* Read HW filter output */
    hw_filter_out = Cy_PPCA_HWFILT3P3Z_ReadFilterDataOutput(HW_FLT_LF_HW) >> 8;

    /* Writing the HW filter output to the DAC input */
    Cy_PPCA_DAC_Set_DACOut(DAC_FLT_HW, hw_filter_out);
}

/*******************************************************************************
* Function Name: freq_mod_isr
********************************************************************************
* Summary:
*
* This is the frequency modulation interrupt handler. This is used to change the
* frequency of the sine wave.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void freq_mod_isr(void)
{
    /* Clearing the interrupt. */
    Cy_TCPWM_ClearInterrupt(freq_mod_HW, freq_mod_NUM, CY_TCPWM_INT_ON_TC);

    /* Updating the period value to change the frequency */
    if(period <= 600) 
    { 
        period = SINE_INTR_PERIOD_MIN; 
    }
    else if(period <= 10500)
    {
        /* Fine step near target frequency */
        period = period - 100;
    }
    else 
    { 
        /* Larger step for higher frequencies */
        period = period - 300; 
    }

    /* Updating the period register */
    Cy_TCPWM_PWM_SetPeriod0(pwm_pfc_ac_HW,pwm_pfc_ac_NUM, period);
}

/*******************************************************************************
* Function Name: disable_isr
********************************************************************************
* Summary:
*
* This is the function to disable the ISR. 
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void disable_isr(void)
{
    /* Disabling all the ISR */
    NVIC_DisableIRQ(pwm_pccm_intr_config.intrSrc);
    NVIC_DisableIRQ(pwm_pfc_ac_intr_config.intrSrc);
    NVIC_DisableIRQ(pwm_psfb_intr_config.intrSrc);
    NVIC_DisableIRQ(hrpwm_intr_config.intrSrc);
    NVIC_DisableIRQ(adc2_filter_intr_config.intrSrc);
    NVIC_DisableIRQ(hwfilter_intr_config.intrSrc);
    NVIC_DisableIRQ(freq_mod_intr_config.intrSrc);
}