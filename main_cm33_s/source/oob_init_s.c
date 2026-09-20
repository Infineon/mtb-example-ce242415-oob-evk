/******************************************************************************
* File Name: oob_init_s.c
*
* Description: This file contains implementation of the initilization functions 
*              for the PSOC™ Control C3M/P8 Out of Box demo code Example
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
* Global Variables for interrupt
********************************************************************************/

/* Interrupt configuration structure of button connected to GPIO. */
cy_stc_sysint_t button_press_intr_config =
{
    .intrSrc = debounce_counter_IRQ,
    .intrPriority = 5UL,
};

/*******************************************************************************
* Global Variables
********************************************************************************/

/* Variable to hold the initial state of the state machine */
volatile uint32_t   state         = DEFAULT;

/* Shared variable to hold the initial state of the state machine */
volatile uint32_t *sm_state = (volatile uint32_t *)(PPCA_CPU0_M4_VAR_ADDRESS);

/* Shared variable for lock */
volatile uint32_t *sm_lock = (volatile uint32_t *)(PPCA_CPU0_M4_VAR_ADDRESS + 0x04);

/* Shared variable for the size of the sine-wave look-up table */
/* To change the output of DAC
 * for sine wave - '99'
 * for Rectified sine wave - '49' */
volatile uint32_t *sm_sine_counter = (volatile uint32_t *)(PPCA_CPU0_M4_VAR_ADDRESS + 0x08);

/* Variable for the current ADC data */
int32_t current_adc_data = 0;

/* Variable for the voltage of MCU in mV */
int32_t mcu_voltage      = 3300;

/* Variable for AFE gain */
float afe_gain_t         = 12.0f;

/* Variable for AFE scaling */
float afe_scale_t        = 1.2f;

/* Variable for the power of MCU in mW */
float mcu_power          = 0;

/* Variable for shunt resistor in ohms */
float shunt_rstr         = 0.1;

/* Variable for calculating the average of current ADC data */
float adc_flt_data       = 0;

/* Variable for the real current consumed by MCU in mA */
float adc_flt_current    = 0;

/* Variable for the MCU current */
float mcu_current        = 0;

/* Variable for potentiometer data */
int32_t pot_adc_data     = 0;

/* ADC0 configuration for Differential ADC */
cy_stc_ppca_adc_config_t test_adc =
{
    .calib_gain_mode = false,
    .aux_slot = false,
    .alt_aux_slot = false,
    .channels = 3,
    .channel_type = 3,
    .channel_data_type = 1,
    .aux_channel_data_type = 0,
    .alt_aux_channel_data_type = 0,
    .aux_slot_config = &ADC0_aux_config,
    .adc_conv_config =
    {
        .group_channel = false,
        .trigger_mode = CY_ADC_ARBITRARY_TRIGGER,
        .auto_trigger_sample_clck = 0,
        .num_of_slot = 1,
        .eos_src = false,
        .num_of_aux_slot = 0,
        .num_of_alt_aux_slot = 0,
        .aux_slot_scan_rate = 0,
        .ext_src = false,
        .sequence_of_channel = 0,
        .sequence_of_aux_channel = 0,
        .sequence_of_alt_aux_channel = 0,
    },
};

/* ADC0 configuration for HrPWM test */
cy_stc_ppca_adc_config_t HrPWM_config =
{
    .calib_gain_mode = false,
    .aux_slot = false,
    .alt_aux_slot = false,
    .channels = 3,
    .channel_type = 0,
    .channel_data_type = 0,
    .aux_channel_data_type = 0,
    .alt_aux_channel_data_type = 0,
    .aux_slot_config = &ADC0_aux_config,
    .adc_conv_config =
    {
        .group_channel = false,
        .trigger_mode = CY_ADC_ARBITRARY_TRIGGER,
        .auto_trigger_sample_clck = 0,
        .num_of_slot = 2,
        .eos_src = false,
        .num_of_aux_slot = 0,
        .num_of_alt_aux_slot = 0,
        .aux_slot_scan_rate = 0,
        .ext_src = false,
        .sequence_of_channel = 16,
        .sequence_of_aux_channel = 0,
        .sequence_of_alt_aux_channel = 0,
    },
};

/* Status variable for SPI */
DEVSTATUS  dev_status;

uint8_t pSendData[256];    /* Create buffer for transmit data */
uint8_t pReadData[260];   /* Create Buffers to read data */

/*******************************************************************************
* LED Logic Threshold Macros
********************************************************************************/

/* ADC threshold values for LED control in differential mode */
#define LED_THRESHOLD_MIN              0
#define LED_THRESHOLD_LOW              682
#define LED_THRESHOLD_MID_LOW          1365
#define LED_THRESHOLD_MID              2047
#define LED_THRESHOLD_MID_HIGH         6144
#define LED_THRESHOLD_HIGH             6827
#define LED_THRESHOLD_VERY_HIGH        7510

/*******************************************************************************
* State Machine Lookup Tables
********************************************************************************/

/* State transition table - maps current state to next state and configuration function */
const state_table_entry_t state_transition_table[] = {
    {INITIAL,                      Hello_world,                 NULL,                            "Hello world !!"},
    {Hello_world,                  Voltage_Current_Consumption, current_consumption_test,         NULL},
    {Voltage_Current_Consumption,  Differential_ADC,            differential_adc,                 NULL},
    {Differential_ADC,             PCCM_test,                   pccm_test,                        NULL},
    {PCCM_test,                    PFC_test,                    pfc_test,                         NULL},
    {PFC_test,                     PSFB_test,                   psfb_test,                        NULL},
    {PSFB_test,                    HR_PWM,                      hrpwm_test,                       NULL},
    {HR_PWM,                       Filtering,                   filter_test,                      NULL},
    {Filtering,                    Filtering_3P3Z,              hw_fltr_test,                     NULL},
    {Filtering_3P3Z,               CLB,                         CLB_test,                         NULL},
    {CLB,                          SPI_EXTERNAL_FLASH,          NULL,                             NULL},
    {SPI_EXTERNAL_FLASH,           Hello_world,                 NULL,                             NULL},
};

/* Size of the state transition table */
#define STATE_TABLE_SIZE (sizeof(state_transition_table) / sizeof(state_table_entry_t))

/* State print messages table */
const char* state_print_messages[] = {
    [INITIAL]                      = NULL,
    [Hello_world]                  = "\r Hello world !!                                                                                      ",
    [Voltage_Current_Consumption]  = NULL,
    [Differential_ADC]             = NULL,
    [PCCM_test]                    = "\r PCCM use case - Rotate the potentiometer - R212 (AIN0P) and Observe the PWM on P5_0.                ",
    [PFC_test]                     = "\r PFC use case - Connect AIN0 and A6P(AIN6P) and Observe the PWM on P6_5 & P6_3                       ",
    [PSFB_test]                    = "\r PSFB test Rotate the potentiometer - R262 (AIN0P) and Observe the PWM on P8_0, P8_1 & P8_4, P8_3    ",
    [HR_PWM]                       = "\r HRPWM test Rotate the potentiometer - R212(Period) & R221(Duty)and Observe the PWM on P7_1 & P9_2   ",
    [Filtering]                    = "\r Filter test Connect AIN0 and A6P(AIN6P) and Observe the result on A1N1                              ",
    [Filtering_3P3Z]               = "\r 3P3Z Filter test Connect AIN0 and AIN11P and Observe the result on A1N1                             ",
    [CLB]                          = "\r CLB test Observe the PWM on P4_4                                                                    ",
    [SPI_EXTERNAL_FLASH]           = "\r                                                                                                     ",
};

/*******************************************************************************
* Helper Functions
********************************************************************************/

/*******************************************************************************
* Function Name: init_pfc_ac_pwm_and_dacs
********************************************************************************
* Summary:
*  Common initialization for PWM counter and DAC resources used by PFC,
*  Filter, and 3P3Z Filter states. Uses OR conditions to avoid repetition.
*
* Parameters:
*  target_state: State that requires common initialization
*
* Return:
*  void
*******************************************************************************/
static void init_pfc_ac_pwm_and_dacs(enum states target_state)
{
    /* variable for the tcpwm status */
    cy_en_tcpwm_status_t status;

    if ((target_state == PFC_test) || (target_state == Filtering) || (target_state == Filtering_3P3Z))
    {
        /* Initializing TCPWM as counter */
        status = Cy_TCPWM_Counter_Init(pwm_pfc_ac_HW, pwm_pfc_ac_NUM, &pwm_pfc_ac_config);
        if (CY_TCPWM_SUCCESS != status)
        {
            CY_ASSERT(0);
        }

        /* Enabling counter */
        Cy_TCPWM_Counter_Enable(pwm_pfc_ac_HW, pwm_pfc_ac_NUM);

        /* Update period only for filter-related states */
        if ((target_state == Filtering) || (target_state == Filtering_3P3Z))
        {
            Cy_TCPWM_Counter_SetPeriod(pwm_pfc_ac_HW, pwm_pfc_ac_NUM, 20000);
        }

        /* Initializing and enabling the DAC */
        Cy_PPCA_DAC_Init(DAC_HW, &DAC_config);
        Cy_PPCA_DACBUF_Enable(DAC_HW);
        Cy_PPCA_DAC_Enable(DAC_HW);
    }

    /* Filter-related states use the filtered DAC output */
    if ((target_state == Filtering) || (target_state == Filtering_3P3Z))
    {
        Cy_PPCA_DAC_Init(DAC_FLT_HW, &DAC_FLT_config);
        Cy_PPCA_DACBUF_Enable(DAC_FLT_HW);
        Cy_PPCA_DAC_Enable(DAC_FLT_HW);
    }
}

/*******************************************************************************
* Function Name: get_state_table_entry
********************************************************************************
* Summary:
*  Finds the state table entry for the given current state.
*
* Parameters:
*  current: The current state to search for
*
* Return:
*  Pointer to state table entry, or NULL if not found
*******************************************************************************/
static const state_table_entry_t* get_state_table_entry(enum states current)
{
    for (uint32_t i = 0; i < STATE_TABLE_SIZE; i++)
    {
        if (state_transition_table[i].current_state == current)
        {
            return &state_transition_table[i];
        }
    }
    return NULL;
}

/*******************************************************************************
* Function definition
********************************************************************************/

/*******************************************************************************
* Function Name: button_press_intr_handler
********************************************************************************
* Summary:
*
* This is the interrupt handler for the button press event. At every button press
* the state machine goes to next state. Before going to next state the previous
* peripherals are turned off using Disable_peripherals() function.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/

void button_press_intr_handler(void)
{
    /* Clears the debounce counter interrupt. */
    uint32_t interrupts = Cy_TCPWM_GetInterruptStatusMasked(debounce_counter_HW, debounce_counter_NUM);
    Cy_TCPWM_ClearInterrupt(debounce_counter_HW, debounce_counter_NUM, interrupts);

    /* Disabling the peripherals to move to next state */
    Disable_peripherals();

    /* Look up the state transition entry */
    const state_table_entry_t* entry = get_state_table_entry((enum states)state);
    
    if (entry != NULL)
    {
        /* Call configuration function if present */
        if (entry->config_handler != NULL)
        {
            entry->config_handler();
        }
        
        /* Special case: SPI flash test requires two function calls */
        if (entry->current_state == CLB)
        {
            spi_flash_test();
            spi_flash_handler();
        }
        
        /* Transition to next state */
        state = entry->next_state;
        *sm_state = entry->next_state;
        *sm_lock = 1;
    }
    else
    {
        /* Invalid state - reset to hello world */
        state = Hello_world;
        *sm_state = Hello_world;
    }
}


/*******************************************************************************
* Function Name: print_current_state
********************************************************************************
* Summary:
*  Prints the current state of the state machine. Uses lookup table for
*  simple states and custom handlers for states requiring dynamic content.
*
* Parameters:
*  void
*
* Return:
*  void
*******************************************************************************/
void print_current_state(void)
{
    /* Handle states with dynamic/computed output */
    if (state == Voltage_Current_Consumption)
    {
        /* Current calculation using AFE */
        current_calculation();
        /* Printing the current and power used by the MCU */
        printf("\r The Voltage and current consumption: Current = %0.2f mA Power = %0.2f mW",
               (double)mcu_current, (double)mcu_power);
        return;
    }
    
    if (state == Differential_ADC)
    {
        /* LED turn on/off logic according to voltage level */
        led_logic();
        printf("\r Rotate R221(AIN0P) and R212(AIN0N) and observe the LEDs. Differential ADC0 output = %ld ",
               (long)pot_adc_data);
        return;
    }
    
    /* For simple states, use lookup table */
    if (state < (sizeof(state_print_messages) / sizeof(state_print_messages[0])))
    {
        const char* message = state_print_messages[state];
        if (message != NULL)
        {
            printf("%s", message);
        }
    }
}

/*******************************************************************************
* Function Name: current_consumption_test
********************************************************************************
* Summary:
* This is the current consumption by the MCU function. This is the configuration
* function, it configures processing unit, combiners, AFE, ADC and PWM.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void current_consumption_test(void)
{
    /* variable for the tcpwm status */
    cy_en_tcpwm_status_t status;

    /* Configuring EPU processing unit */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, pu_t1_voltage_current_trg_INDEX, &pu_t1_voltage_current_trg_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, pu_t1_voltage_current_trg_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU processing unit */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, pu_t1_adc1_eoc_INDEX, &pu_t1_adc1_eoc_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, pu_t1_adc1_eoc_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb72_voltage_current_trg_INDEX, &comb72_voltage_current_trg_combo_config);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb76_adc1_eoc_INDEX, &comb76_adc1_eoc_combo_config);

    /* Initializing the AFE */
    Cy_PPCA_AFE_Init(ADC1_AFE_HW, &ADC1_afe_config);

    /* Enabling the AFE */
    Cy_PPCA_AFE_Enable(ADC1_AFE_HW);

    /* Initializing ATOP ADC 1 */
    Cy_PPCA_ADC_Init(ADC1_HW, &ADC1_config);

    /* Enabling ADC 1 */
    Cy_PPCA_ADC_Enable(ADC1_HW);

    /* Initializing ADC filter */
    Cy_PPCA_ADC_Filter_Init(ADC_FLT_HW,&ADC_FLT_config);

    /* Initializing ADC average filter */
    Cy_PPCA_ADC_AVG_Filter_Trigger_Start(ADC_FLT_HW);

    /* Initializing PWM  */
    status = Cy_TCPWM_PWM_Init(voltage_current_trg_HW, voltage_current_trg_NUM, &voltage_current_trg_config);
    if(CY_TCPWM_SUCCESS != status)
    {
        CY_ASSERT(0);
    }

    /* Enabling PWM */
    Cy_TCPWM_PWM_Enable(voltage_current_trg_HW, voltage_current_trg_NUM);

    /* Starting PWM */
    Cy_TCPWM_TriggerStart_Single(voltage_current_trg_HW, voltage_current_trg_NUM);
}

/*******************************************************************************
* Function Name: current_calculation
********************************************************************************
* Summary:
* The function is used to calculate the current consumed by the MCU using AFE.
* To reject the noise moving average is applied on the ADC value.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void current_calculation(void)
{
    /* Reading the value of MCU current from the ADC */
    current_adc_data = Cy_PPCA_ADC_Read_ADC_Data(ADC1_HW, 0);

    /* Applying Moving average on the MCU current value */
    adc_flt_data = adc_flt_data + (((float32_t)current_adc_data - adc_flt_data) * 0.125f);

    /* converting ADC value to current in mA */
    mcu_current = (((adc_flt_data*afe_scale_t)/(afe_gain_t*HALF_ADC_RANGE*shunt_rstr))*1000.0f);

    /* calculating the power used by MCU */
    mcu_power = (mcu_current * mcu_voltage)/1000.0f;
}

/*******************************************************************************
* Function Name: differential_adc
********************************************************************************
* Summary:
*
* This is the differential ADC configuration function. It configures combiners,
* and ADC. The ADC's are configured in differential mode. This use case implements
* a differential measurement to provide a visual feedback on Serial Monitor to
* present the Full Range sensing (from -3.3V to 3.3V).
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void differential_adc(void)
{
    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb60_differential_adc_INDEX, &comb60_differential_adc_combo_config);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb61_differential_adc_INDEX, &comb61_differential_adc_combo_config);

    /* Initializing the ATOP ADC 0*/
    Cy_PPCA_ADC_Init(ADC0_HW, &test_adc);

    /* Enabling the ATOP ADC 0 */
    Cy_PPCA_ADC_Enable(ADC0_HW);
}

/*******************************************************************************
* Function Name: led_logic
********************************************************************************
* Summary:
*
* This is led logic function. According to the differential measurement result,
* an appropriate number of LED will turn on
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void led_logic(void)
{
    /* Reading the ADC value */
    pot_adc_data = Cy_PPCA_ADC_Read_ADC_Data(ADC0_HW, 0);

    /* Comparing the ADC value to determine the number of LEDs to turn on */
    if((pot_adc_data >= LED_THRESHOLD_MID_HIGH) && (pot_adc_data < LED_THRESHOLD_HIGH))
    {
        Cy_GPIO_Set(CYBSP_USER_LED2_PORT,CYBSP_USER_LED2_PIN);
        Cy_GPIO_Set(CYBSP_USER_LED3_PORT,CYBSP_USER_LED3_PIN);
        Cy_GPIO_Set(CYBSP_USER_LED4_PORT,CYBSP_USER_LED4_PIN);
        Cy_GPIO_Set(CYBSP_USER_LED5_PORT,CYBSP_USER_LED5_PIN);
        Cy_GPIO_Set(CYBSP_USER_LED6_PORT,CYBSP_USER_LED6_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED1_PORT,CYBSP_USER_LED1_PIN);
    }

    if((pot_adc_data >= LED_THRESHOLD_HIGH) && (pot_adc_data < LED_THRESHOLD_VERY_HIGH))
    {
        Cy_GPIO_Set(CYBSP_USER_LED3_PORT,CYBSP_USER_LED3_PIN);
        Cy_GPIO_Set(CYBSP_USER_LED4_PORT,CYBSP_USER_LED4_PIN);
        Cy_GPIO_Set(CYBSP_USER_LED5_PORT,CYBSP_USER_LED5_PIN);
        Cy_GPIO_Set(CYBSP_USER_LED6_PORT,CYBSP_USER_LED6_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED1_PORT,CYBSP_USER_LED1_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED2_PORT,CYBSP_USER_LED2_PIN);
    }

    if(pot_adc_data >= LED_THRESHOLD_VERY_HIGH)
    {
        Cy_GPIO_Set(CYBSP_USER_LED4_PORT,CYBSP_USER_LED4_PIN);
        Cy_GPIO_Set(CYBSP_USER_LED5_PORT,CYBSP_USER_LED5_PIN);
        Cy_GPIO_Set(CYBSP_USER_LED6_PORT,CYBSP_USER_LED6_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED1_PORT,CYBSP_USER_LED1_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED2_PORT,CYBSP_USER_LED2_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED3_PORT,CYBSP_USER_LED3_PIN);
    }

    if((pot_adc_data >= LED_THRESHOLD_MIN) && (pot_adc_data < LED_THRESHOLD_LOW))
    {
        Cy_GPIO_Set(CYBSP_USER_LED5_PORT,CYBSP_USER_LED5_PIN);
        Cy_GPIO_Set(CYBSP_USER_LED6_PORT,CYBSP_USER_LED6_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED1_PORT,CYBSP_USER_LED1_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED2_PORT,CYBSP_USER_LED2_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED3_PORT,CYBSP_USER_LED3_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED4_PORT,CYBSP_USER_LED4_PIN);
    }

    if((pot_adc_data >= LED_THRESHOLD_LOW) && (pot_adc_data < LED_THRESHOLD_MID_LOW))
    {
        Cy_GPIO_Set(CYBSP_USER_LED6_PORT,CYBSP_USER_LED6_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED1_PORT,CYBSP_USER_LED1_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED2_PORT,CYBSP_USER_LED2_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED3_PORT,CYBSP_USER_LED3_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED4_PORT,CYBSP_USER_LED4_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED5_PORT,CYBSP_USER_LED5_PIN);
    }

    if((pot_adc_data >= LED_THRESHOLD_MID_LOW) && (pot_adc_data < LED_THRESHOLD_MID))
    {
        Cy_GPIO_Clr(CYBSP_USER_LED1_PORT,CYBSP_USER_LED1_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED2_PORT,CYBSP_USER_LED2_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED3_PORT,CYBSP_USER_LED3_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED4_PORT,CYBSP_USER_LED4_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED5_PORT,CYBSP_USER_LED5_PIN);
        Cy_GPIO_Clr(CYBSP_USER_LED6_PORT,CYBSP_USER_LED6_PIN);
    }
}

/*******************************************************************************
* Function Name: pccm_test
********************************************************************************
* Summary:
*
* This is peak current control mode (PCCM) configuration function. Such scenario mimics
* the behavior of a PCCM where the potentiometer is setting an I_REF value. A DAC
* is configured in Down Slope Mode. A TCPWM is configured to generate a PWM signal
* with a max duty cycle of 90%, moving the potentiometer will reduce the duty cycle
* since the DCSG will see from one side the potentiometer input and the other will
* be the DAC. Anytime comparator detect it, then PWM will be go low. PWM will come
* back high at next terminal count.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/

void pccm_test(void)
{
    /* variable for the tcpwm status */
    cy_en_tcpwm_status_t status;

    /* Disable the ADC */
    Cy_PPCA_ADC_Disable(ADC0_HW);

    /* De-init the ADC */
    Cy_PPCA_ADC_DeInit(ADC0_HW);

    /* Initializing ATOP ADC 0*/
    Cy_PPCA_ADC_Init(ADC0_HW, &ADC0_config);

    /* Enabling ADC 0*/
    Cy_PPCA_ADC_Enable(ADC0_HW);

    /* Initializing TCPWM as PWM */
    status = Cy_TCPWM_PWM_Init(pwm_pccm_HW, pwm_pccm_NUM, &pwm_pccm_config);

    /* Initialization failed */
    if(CY_TCPWM_SUCCESS != status)
    {
        CY_ASSERT(0);
    }

    /* Enabling PWM */
    Cy_TCPWM_PWM_Enable(pwm_pccm_HW, pwm_pccm_NUM);

    /* Initializing DCSG */
    Cy_PPCA_DCSG_Init(DCSG_HW, &DCSG_config);

    /* Enabling DCSG */
    Cy_PPCA_DCSG_Enable(DCSG_HW);

    /* Initializing DCSG slope generation. */
    Cy_PPCA_DCSG_SLGEN_Init(DCSG_SL_HW, &DCSG_SL_config);

    /* Enabling DCSG slope generation. */
    Cy_PPCA_DCSG_SLGEN_Enable(DCSG_SL_HW);

    /* Triggering the slope ramp generation. */
    Cy_PPCA_DCSG_SLGEN_Trigger_Ramp_Generation(DCSG_SL_HW);

    /* Configuring EPU processing unit */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, pu_t1_dcsg_trg_INDEX, &pu_t1_dcsg_trg_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, pu_t1_dcsg_trg_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb1_dcsg_trg_INDEX, &comb1_dcsg_trg_combo_config);

    /* Software start signal to start the PWM */
    Cy_TCPWM_TriggerStart_Single(pwm_pccm_HW, pwm_pccm_NUM);

}

/*******************************************************************************
* Function Name: pfc_test
********************************************************************************
* Summary:
*
* This power factor correction configuration function.
*
* Goal of this use case is to mimic the PWM interruption of a PFC application
* during AC voltage zero crossing. A TCPWM is programmed to output a PWM line,
* 50% duty cycle at 100 kHz frequency. A DAC is configured to generate the rectified
* VAC signal which is routed to an input ADC pin. A comparator is needed to compare
* a rectified input voltage signal (emulating the AC line voltage) against a threshold
* voltage.
*
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void pfc_test(void)
{
    /* variable for the tcpwm status */
    cy_en_tcpwm_status_t status;

    /* Updating the sine counter */
    *sm_sine_counter = 49;

    /* Initializing TCPWM as PWM */
    status = Cy_TCPWM_PWM_Init(pwm_pfc_HW, pwm_pfc_NUM, &pwm_pfc_config);

    /* Initialization failed */
    if(CY_TCPWM_SUCCESS != status)
    {
        CY_ASSERT(0);
    }

    /* Enabling PWM */
    Cy_TCPWM_PWM_Enable(pwm_pfc_HW, pwm_pfc_NUM);

    /* Software start signal to start the PWM */
    Cy_TCPWM_TriggerStart_Single(pwm_pfc_HW, pwm_pfc_NUM);

    /* Common PWM counter and DAC initialization for PFC-related states */
    init_pfc_ac_pwm_and_dacs(PFC_test);

    /* Configuring EPU processing unit */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, pu_t1_adc2_trg_INDEX, &pu_t1_adc2_trg_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, pu_t1_adc2_trg_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb78_adc2_soc_INDEX, &comb78_adc2_soc_combo_config);

    /* Configuring EPU processing unit */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, pu_t1_dcmp_out_INDEX, &pu_t1_dcmp_out_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, pu_t1_dcmp_out_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb8_pwm_pfc_msk_INDEX, &comb8_pwm_pfc_msk_combo_config);

    /* Initializing ATOP ADC 2*/
    Cy_PPCA_ADC_Init(ADC2_HW, &ADC2_config);
    Cy_PPCA_ADC_Enable(ADC2_HW);

    /* Initializing ATOP Digital Comparator */
    Cy_PPCA_DCMP_Init(DCMP2_HW, &DCMP2_config);
    Cy_PPCA_DCMP_Enable(DCMP2_HW);

    /* Software start signal to start the counter */
    Cy_TCPWM_TriggerStart_Single(pwm_pfc_ac_HW,pwm_pfc_ac_NUM);

}

/*******************************************************************************
* Function Name: filter_test
********************************************************************************
* Summary:
*
* This is the ADC filter_test configuration function.
*
* DAC is used to generate an Offset sine wave with frequency 100 Hz. The sine
* wave is then passed to ADC filter. The Output of the ADC filter is passed to
* Other DAC to visualize the filtered output.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void filter_test(void)
{
    /* Updating the sine counter */
    *sm_sine_counter = 99;

    /* Common PWM counter and DAC initialization for filter-related states */
    init_pfc_ac_pwm_and_dacs(Filtering);

    /* Configuring EPU processing unit for ADC2 trigger */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, pu_t1_adc2_trg_INDEX, &pu_t1_adc2_trg_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, pu_t1_adc2_trg_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner to route ADC2 start-of-conversion signal */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb78_adc2_soc_INDEX, &comb78_adc2_soc_combo_config);

    /* Configuring EPU processing unit for ADC2 end-of-conversion */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, put_t1_adc2_eoc_INDEX, &put_t1_adc2_eoc_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, put_t1_adc2_eoc_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner to route ADC interrupt signal */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb104_adc_intr_INDEX, &comb104_adc_intr_combo_config);

    /* Initializing ATOP ADC 2*/
    Cy_PPCA_ADC_Init(ADC2_HW, &ADC2_config);
    Cy_PPCA_ADC_Enable(ADC2_HW);

    /* Initializing ATOP ADC Filter */
    Cy_PPCA_ADC_Filter_Init(ADC2_FLT_HW, &ADC2_FLT_config);

    /* Configure the EPU interrupt to the PPCA CPU0 on conversion completion of the ADC */
    Cy_PPCA_EPU_InterruptSourceSelect(ppca_0_epu_0_EPU_IRQ0_HW, false, epuIrqSrc0);
    Cy_PPCA_EPU_SetInterruptMask(ppca_0_epu_0_EPU_IRQ0_HW);

    /* Software start to the counter */
    Cy_TCPWM_TriggerStart_Single(pwm_pfc_ac_HW,pwm_pfc_ac_NUM);
}

/*******************************************************************************
* Function Name: psfb_test
********************************************************************************
* Summary:
*
* This phase shift full bridge configuration function.
*
* Goal of this use case is to mimic a phase shift full bridge application (PSFB).
* 2 TCPWM are used to generate 2 complementary pairs. 1 potentiometer is used to
* shift the ON-time of the two timers in a range from 0° to 360°. Such range shall
* correspond to perfect synchronous ON Time (0°), and 1 period delay (360°)
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void psfb_test(void)
{
    /* variable for the tcpwm status */
    cy_en_tcpwm_status_t status;

    /* Initializing TCPWM as PWM */
    status = Cy_TCPWM_PWM_Init(pwm_psfb1_HW, pwm_psfb1_NUM, &pwm_psfb1_config);

    /* Initialization failed */
    if(CY_TCPWM_SUCCESS != status)
    {
        CY_ASSERT(0);
    }

    /* Enabling the TCPWM as PWM */
    Cy_TCPWM_PWM_Enable(pwm_psfb1_HW, pwm_psfb1_NUM);

    /* Initializing TCPWM as PWM */
    status = Cy_TCPWM_PWM_Init(pwm_psfb2_HW, pwm_psfb2_NUM, &pwm_psfb2_config);

    /* Initialization failed */
    if(CY_TCPWM_SUCCESS != status)
    {
        CY_ASSERT(0);
    }

    /* Enabling the TCPWM as PWM */
    Cy_TCPWM_PWM_Enable(pwm_psfb2_HW, pwm_psfb2_NUM);

    /* Configuring EPU processing unit */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, put_t1_psfb_trig_INDEX, &put_t1_psfb_trig_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, put_t1_psfb_trig_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb12_psfb_trig_INDEX, &comb12_psfb_trig_combo_config);

    /* Configuring EPU processing unit */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, put_t1_psfb1_sync_INDEX, &put_t1_psfb1_sync_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, put_t1_psfb1_sync_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb9_psfb1_sync_INDEX, &comb9_psfb1_sync_combo_config);

    /* Configuring EPU processing unit */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, put_t1_psfb2_sync_INDEX, &put_t1_psfb2_sync_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, put_t1_psfb2_sync_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb10_psfb2_sync_INDEX, &comb10_psfb2_sync_combo_config);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb88_adc3_soc_INDEX, &comb88_adc3_soc_combo_config);

    /* Initializing the timer */
    status = Cy_TCPWM_Counter_Init(psfb_start_trig_HW,psfb_start_trig_NUM,&psfb_start_trig_config);

    /* Initialization failed */
    if(CY_TCPWM_SUCCESS != status)
    {
        CY_ASSERT(0);
    }

    /* Enabling the timer */
    Cy_TCPWM_Counter_Enable(psfb_start_trig_HW,psfb_start_trig_NUM);

    /* Initializing ATOP ADC 3 */
    Cy_PPCA_ADC_Init(ADC3_HW, &ADC3_config);

    /* Enabling ADC 3 */
    Cy_PPCA_ADC_Enable(ADC3_HW);

    /* Software start to the sync timer */
    Cy_TCPWM_TriggerStart_Single(psfb_start_trig_HW,psfb_start_trig_NUM);

}

/*******************************************************************************
* Function Name: hrpwm_test
********************************************************************************
* Summary:
*
* This is the high resolution PWM configuration function.
*
* One potentiometer is used to set the duty cycle. One potentiometer is used to
* set the period.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void hrpwm_test(void)
{
    /* variable for the tcpwm status */
    cy_en_tcpwm_status_t status;

    /* Initializing TCPWM as PWM */
    status = Cy_TCPWM_PWM_Init(pwm_hrpwm_HW, pwm_hrpwm_NUM, &pwm_hrpwm_config);

    /* Initialization failed */
    if(CY_TCPWM_SUCCESS != status)
    {
        CY_ASSERT(0);
    }

    /* Enabling the TCPWM as PWM */
    Cy_TCPWM_PWM_Enable(pwm_hrpwm_HW, pwm_hrpwm_NUM);

    /* Initializing ATOP ADC 0*/
    Cy_PPCA_ADC_Init(ADC0_HW, &HrPWM_config);

    /* Enabling the ADC 0 */
    Cy_PPCA_ADC_Enable(ADC0_HW);

    /* Configuring EPU processing unit */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, put_t1_hrpwm_trg_INDEX, &put_t1_hrpwm_trg_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, put_t1_hrpwm_trg_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb3_hrpwm_trg_INDEX, &comb3_hrpwm_trg_combo_config);

    /* Software start to the PWM */
    Cy_TCPWM_TriggerStart_Single(pwm_hrpwm_HW, pwm_hrpwm_NUM);

}

/*******************************************************************************
* Function Name: hw_fltr_test
********************************************************************************
* Summary:
*
* This is the HW 3p3Z filter_test configuration function.
*
* DAC is used to generate a sine wave with varying frequency. The sine wave is
* then passed to ADC filter and then fed to the 3P3Z HW filter. The filter is
* configured as band pass filter. The Output of the ADC filter is passed to other
* DAC to visualize the filtered output.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void hw_fltr_test(void)
{
    /* variable for the tcpwm status */
    cy_en_tcpwm_status_t status;

    /* Updating the sine counter */
    *sm_sine_counter = 99;

    /* Common PWM counter and DAC initialization for filter-related states */
    init_pfc_ac_pwm_and_dacs(Filtering_3P3Z);

    /* Initializing ATOP ADC 3*/
    Cy_PPCA_ADC_Init(ADC3_HW, &ADC3_config);
    Cy_PPCA_ADC_Enable(ADC3_HW);

    /* Initializing ATOP ADC Filter */
    Cy_PPCA_ADC_Filter_Init(ADC3_FLT_HW, &ADC3_FLT_config);

    /* Enabling Hardware filter block */
    Cy_PPCA_HWFILT3P3Z_SS_PeripheralEnable(HW_FLT_LF_SS_HW);

    /* Enabling Hardware filter */
    Cy_PPCA_HWFILT3P3Z_FilterEnable(HW_FLT_LF_HW, true);

    /* Initializing Hardware filter */
    Cy_PPCA_HWFILT3P3Z_InitFilterConfig(HW_FLT_LF_HW, &HW_FLT_LF_Set1_config);

    /* Setting Data input 0 for the filter */
    Cy_PPCA_HWFILT3P3Z_Write_DATA_IN0(HW_FLT_LF_HW, 0);

    /* Initializing the counter */
    status = Cy_TCPWM_Counter_Init(freq_mod_HW, freq_mod_NUM, &freq_mod_config);

    /* Initialization failed */
    if(CY_TCPWM_SUCCESS != status)
    {
        CY_ASSERT(0);
    }

    /* Enabling the counter */
    Cy_TCPWM_Counter_Enable(freq_mod_HW, freq_mod_NUM);

    /* Configuring EPU processing unit to ADC3 SOC from PWM. */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, pu_t1_voltage_current_trg_INDEX, &pu_t1_voltage_current_trg_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, pu_t1_voltage_current_trg_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner to route ADC3 start-of-conversion signal */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb90_adc3_soc_INDEX, &comb90_adc3_soc_combo_config);

    /* Configuring EPU processing unit to receive HWfilter EOP signal */
    Cy_PPCA_EPU_PU_T1_Configure(PPCA_EPU_EPU, put_t1_hwflt_eop_INDEX, &put_t1_hwflt_eop_put1_config);
    Cy_PPCA_EPU_PU_T1_Enable(PPCA_EPU_EPU, put_t1_hwflt_eop_INDEX,CY_ENABLE_ASYNC_BYPASS);

    /* Configuring EPU combiner to route the 3P3Z HW filter interrupt signal */
    Cy_PPCA_EPU_Combo_Configure(PPCA_EPU_EPU, comb105_hwflt_3p3z_INDEX, &comb105_hwflt_3p3z_combo_config);

    /* Configure the EPU interrupt to the PPCA CPU0 on conversion completion of the ADC */
    Cy_PPCA_EPU_InterruptSourceSelect(ppca_0_epu_0_EPU_IRQ1_HW, false, epuIrqSrc1);
    Cy_PPCA_EPU_SetInterruptMask(ppca_0_epu_0_EPU_IRQ1_HW);

    /* Software start to the timer */
    Cy_TCPWM_TriggerStart_Single(freq_mod_HW, freq_mod_NUM);

    /* Software start to the counter */
    Cy_TCPWM_TriggerStart_Single(pwm_pfc_ac_HW,pwm_pfc_ac_NUM);
}

/*******************************************************************************
* Function Name: CLB_test
********************************************************************************
* Summary:
*
* This is the CLB test configuration function.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void CLB_test(void)
{
    /* Select CLB output as PPCA output */
    Cy_PPCA_CLB_Select_Output(CLB_HW, CY_CLB_MACO_SEL);

    /* Enable CLB */
    Cy_PPCA_CLB_Enable(CLB_HW);

    /* Initialize and configure CLB MACO */
    Cy_PPCA_CLB_MACO_InitConfig(CLB_MACO_HW, &CLB_macoConfig);

    /* Enable CLB MACO */
    Cy_PPCA_CLB_MACO_Enable(CLB_MACO_HW);

}

/*******************************************************************************
* Function Name: spi_flash_test
********************************************************************************
* Summary:
*
* This is the SPI based external flash test configuration function.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void spi_flash_test(void)
{
    /* variable for the SPI status */
    cy_rslt_t  result;

    /* Initialize SPI */
    result = init_spi();
    
    /* Initialization failed. Stop program execution */
    if(result != SLLD_OK)
    {
        CY_ASSERT(0);
    }

}

/*******************************************************************************
* Function Name: spi_flash_handler
********************************************************************************
* Summary:
*
* This function is erasing and programming the external flash.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void spi_flash_handler(void)
{
    /* variable for the SPI status */
    cy_rslt_t  result;
    
    printf("\r SPI based external flash test                               \n");
    printf("\r\n Issuing erase command ");

    /* Issue erase command */
    result = slld_SEOp(0, 0x00000000, &dev_status);

    if(result != SLLD_OK)
    {
        /* Operation failed during erase sector: ProjectStatus = 1 */
        printf("\r\n Operation failed during erase sector ");
        CY_ASSERT(0); // Error
    }
    else
    {
        printf("\r\n Operation success during erase sector ");
        CyDelayCycles(8000000);
    }

    /* Reading the data from external memory */
    result = slld_ReadOp(0, 0x00000000, &pReadData[0], 256);

    printf("\r\n Data read from external memory: \n");
    print(pReadData,256);

    printf("\r\n\n Data to be programmed into external memory: \n");

    for(uint16_t tmp=0;tmp<256;tmp++)
    {
        pSendData[tmp]=tmp;
    }

    /* Printing the data to be programmed into external memory */
    print(pSendData,256);

    printf("\r\n\n Issuing program command.");

    /* Issue program command */
    result = slld_WriteOp(0, 0x00000000, &pSendData[0], 256,&dev_status);
    if(result != SLLD_OK)
    {
        printf("\n Operation failed during program page ");
        CY_ASSERT(0); // Error
    }
    else
    {
        printf("\r\n Operation success during program page ");
    }

    /* Read the data from SPI flash chip */
    result = slld_ReadOp(0, 0x00000000, &pReadData[0], 256);

    if(result != SLLD_OK)
    {
        printf("\r\n Operation failed during read page ");
        CY_ASSERT(0); // Error
    }
    else
    {
        printf("\r\n Data read from external memory: \n");
    }

    /* Printing the data read from external memory */
    print(pReadData,256);

    /* Check all the data received */
    for (uint16_t tmp=0; tmp<256; tmp++)
    {
        if (pSendData[tmp]!=pReadData[tmp])
        {
            printf("\r\n Operation failed during data verify ");
            CY_ASSERT(0); // Error
        }
    }

    printf("\r\n Operation successful where data written is matching with data read\n");
}

/*******************************************************************************
* Function Name: print
********************************************************************************
* Summary:
*
* This function is used to print the data in the flash.
*
* Parameters:
* data: pointer to data buffer
* length: length of data buffer
*
* Return:
*  void
*
*******************************************************************************/
void print(uint8_t* data,uint16_t length)
{
    uint16_t tmp=0;

    for (tmp = 0; tmp < length; tmp++)
    {
        if (tmp % 16 == 0)
        {
            printf("\n\r");
        }

        printf("0x%X ", data[tmp]);

        CyDelayCycles(20000);
    }
}

/*******************************************************************************
* Function Name: Disable_peripherals
********************************************************************************
* Summary:
*
* This function is used to disable the peripherals before moving to next state.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Disable_peripherals(void)
{
    /* State 2 */
    /* Disable the ADC 1 */
    Cy_PPCA_ADC_Disable(ADC1_HW);

    /* De-init the ADC 1 */
    Cy_PPCA_ADC_DeInit(ADC1_HW);

    /* State 3 */
    /* Disable the ADC 0 */
    Cy_PPCA_ADC_Disable(ADC0_HW);

    /* De-init the ADC 0 */
    Cy_PPCA_ADC_DeInit(ADC0_HW);

    /* State 4 */
    /* Disable the PWM */
     Cy_TCPWM_PWM_Disable(pwm_pccm_HW, pwm_pccm_NUM);

     /* Disable the DCSG */
     Cy_PPCA_DCSG_Disable(DCSG_HW);

     /* Disable the DCSG slope generator */
     Cy_PPCA_DCSG_SLGEN_Disable(DCSG_SL_HW);

     /* State 5 */
     /* Disable the PWM */
     Cy_TCPWM_PWM_Disable(pwm_pfc_HW, pwm_pfc_NUM);

     /* Disable the counter */
     Cy_TCPWM_Counter_Disable(pwm_pfc_ac_HW, pwm_pfc_ac_NUM);

     /* Disable the DAC */
     Cy_PPCA_DAC_Disable(DAC_HW);

     /* Disable the ATOP ADC 2 */
     Cy_PPCA_ADC_Disable(ADC2_HW);

     /* De-init the ATOP ADC 2 */
     Cy_PPCA_ADC_DeInit(ADC2_HW);

     /* Disable the comparator */
     Cy_PPCA_DCMP_Disable(DCMP2_HW);

     /* State 6 */
     /* Disable the PWM */
     Cy_TCPWM_PWM_Disable(pwm_psfb1_HW, pwm_psfb1_NUM);

     /* Disable the PWM */
     Cy_TCPWM_PWM_Disable(pwm_psfb2_HW, pwm_psfb2_NUM);

     /* Disable the counter */
     Cy_TCPWM_Counter_Disable(psfb_start_trig_HW,psfb_start_trig_NUM);

     /* Disable the ADC3 */
     Cy_PPCA_ADC_Disable(ADC3_HW);

     /* De-init the ADC3 */
     Cy_PPCA_ADC_DeInit(ADC3_HW);

     /* State 7 */
     /* Disable the PWM */
     Cy_TCPWM_PWM_Disable(pwm_hrpwm_HW, pwm_hrpwm_NUM);

     /* State 8 */
     /* Disable the DAC */
     Cy_PPCA_DAC_Disable(DAC_FLT_HW);

     /* State 9 */
     /* Disable the HW filter */
     Cy_PPCA_HWFILT3P3Z_SS_PeripheralDisable(HW_FLT_LF_SS_HW);

     /* Disable the counter */
     Cy_TCPWM_Counter_Disable(freq_mod_HW, freq_mod_NUM);

     /* State 10 */
     /* Disable the CLB MACO */
     Cy_PPCA_CLB_MACO_Disable(CLB_MACO_HW);

     /* Disable the CLB */
     Cy_PPCA_CLB_Disable(CLB_HW);

     /* State 11 */
     /* Disable the SPI */
     Cy_SCB_SPI_Disable(mSPI_HW, &mSPI_context);

     /* Disable the interrupt */
     NVIC_DisableIRQ(mSPI_IRQ);

}
