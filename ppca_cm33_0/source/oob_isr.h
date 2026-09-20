/******************************************************************************
* File Name: oob_isr.h
*
* Description: This is a header file for ISR file for the PSOC™ Control C3
*              (P/M)(7/8) Out of Box demo code Example
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

#ifndef SOURCE_OOB_ISR_H_
#define SOURCE_OOB_ISR_H_

#include "cy_pdl.h"
#include "cycfg.h"
#include "stdio.h"

/*******************************************************************************
* Macros
********************************************************************************/

/* Minimum frequency for sine wave interrupt period */
#define SINE_INTR_PERIOD_MIN 21000

/* Shared memory addresses in M4 shared memory space (0x20040000-0x20043FFF) */
/* All cores can access M4 shared memory for inter-core communication */
#define PPCA_CPU0_M4_VAR_ADDRESS 0x20040400  /* Used by this core (CPU0) */

/* State in the state machine */
enum states {INITIAL,
             Hello_world,
             Voltage_Current_Consumption,
             Differential_ADC,
             PCCM_test,
             PFC_test,
             PSFB_test,
             HR_PWM,
             Filtering,
             Filtering_3P3Z,
             CLB,
             SPI_EXTERNAL_FLASH,
             OUT_INVALID,
             DEFAULT };

/*******************************************************************************
* Global Variables for interrupt
*********************************************************************************/         
   
/* Interrupt configuration structure of PWM for PCCM. */
extern cy_stc_sysint_t pwm_pccm_intr_config;

/* Interrupt configuration structure of PWM for PFC. */
extern cy_stc_sysint_t pwm_pfc_ac_intr_config;

/* Interrupt configuration structure of ADC filter ISR */
extern cy_stc_sysint_t adc2_filter_intr_config;

/* Interrupt configuration structure of PWM for PSFB. */
extern cy_stc_sysint_t pwm_psfb_intr_config;

/* Interrupt configuration structure of PWM for HRPWM. */
extern cy_stc_sysint_t hrpwm_intr_config;

/* Interrupt configuration structure of Hardware filter ISR */
extern cy_stc_sysint_t hwfilter_intr_config;

/* Interrupt configuration structure of frequency modulator ISR */
extern cy_stc_sysint_t freq_mod_intr_config;

/*******************************************************************************
* Global Variables
*********************************************************************************/

/* Shared variable to hold the state of the state machine */
extern volatile uint32_t *sm_state;

/* Shared variable for lock on switch case */
extern volatile uint32_t *sm_lock;

/* Shared variable for the size of the sine-wave look-up table */
/* To change the output of DAC
 * for sine wave - '99'
 * for Rectified sine wave - '49' */
extern volatile uint32_t *sm_sine_counter;

/* Variable for the size of sine array in PFC */
extern uint32_t   counter;

/* Variable to store the data sine table */
extern float sine_wave;

/* Lookup table for a sine wave in unsigned format. */
extern uint32_t sinewave_pattern[];

/* Variable for the output of the filter */
extern uint32_t filter_data;

/* Variable to create delay in phase shift full bridge test */
extern float delay;

/* Variable for calculating the average of delay in phase shift full bridge test */
extern float delay_avg;

/* Variable for the output of the 3P3Z HW filter */
extern uint32_t hw_filter_out;

/* Variable for the period of freq modulator */
extern uint32_t period;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
/* Switch case for Interrupt initialization. */
void isr_init(void);

/* Interrupt initialization for PCCM test case. */
void pccm_isr_int(void);

/* Interrupt initialization for PFC test case. */
void pfc_isr_int(void);

/* Interrupt initialization for PSFB test case. */
void psfb_isr_int(void);

/* Interrupt initialization for HRPWM test case. */
void hrpwm_isr_int(void);

/* Interrupt initialization for filter test case. */
void filter_isr_int(void);

/* Interrupt initialization for filter test case. */
void hw_isr_int(void);

/* Interrupt handler for PCCM test case.     */
void pwm_pccm_isr(void);

/* Interrupt handler for PFC test case.      */
void rectified_ac_generator_isr(void);

/* Interrupt handler for adc filter test case.*/
void adc2_fltr_isr(void);

/* Interrupt handler for psfb test case.     */
void psfb_isr(void);

/* Interrupt handler for hrpwm test case.    */
void hrpwm_isr(void);

/* Interrupt handler for frequency modulator.*/
void freq_mod_isr(void);

/* Interrupt handler for adc filter test case.*/
void hw_fltr_isr(void);

/* Function to disable the ISR */
void disable_isr(void);

#endif /* SOURCE_OOB_ISR_H_ */
