/******************************************************************************
* File Name: oob_init_s.h
*
* Description: This is a header file for initialization file for the PSOC™  
*              Control C3 (P/M)(7/8) Out of Box demo code Example
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

#ifndef SOURCE_OOB_INIT_S_H_
#define SOURCE_OOB_INIT_S_H_

#include "cy_pdl.h"
#include "cybsp.h"
#include "slld.h"
#include "slld_hal.h"
#include "cy_retarget_io.h"
#include "stdio.h"
#include "cycfg.h"
#include "stdint.h"
#include "string.h"

/*******************************************************************************
* Macros
********************************************************************************/

/* Half of the ADC range for midpoint calculations */
#define HALF_ADC_RANGE 2048

/* Shared memory addresses for inter-core communication */
/* PPCA cores write to these locations, main core reads from them */
/* Variables located in M4 shared memory space (16KB at 0x20040000-0x20043FFF from PPCA view) */
/* Main core accesses PPCA memory through PPCA peripheral base with memory windows: */
/* M1 (CPU0 data): 0x53020000, M3 (CPU1 data): 0x53040000, M4 (shared): 0x53050000 */
#define PPCA_CPU0_M4_VAR_ADDRESS   0x53050400  /* Written by PPCA Core 0 */
#define PPCA_CPU1_M4_VAR_ADDRESS   0x53050800  /* Written by PPCA Core 1 */

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

/* Interrupt configuration structure of button GPIO. */
extern cy_stc_sysint_t button_press_intr_config;

/*******************************************************************************
* Type Definitions
********************************************************************************/

/* Function pointer types for state handlers */
typedef void (*state_config_func_t)(void);
typedef void (*state_print_func_t)(void);

/* State table entry structure */
typedef struct {
    enum states current_state;
    enum states next_state;
    state_config_func_t config_handler;
    const char* print_message;
} state_table_entry_t;

/*******************************************************************************
* Function Prototypes
********************************************************************************/

/* Interrupt handler for button interrupt.*/
void button_press_intr_handler(void);

/* Function for calculation of MCU current.*/
void current_calculation(void);

/* Function for configuration of voltage and current test.*/
void current_consumption_test(void);

/* Function for configuration of differential ADC test.*/
void differential_adc(void);

/* Function for LED logic.*/
void led_logic(void);

/* Function for configuration of PCCM test.*/
void pccm_test(void);

/* Function for configuration of PFC test.*/
void pfc_test(void);

/* Function for configuration of PSFB test.*/
void psfb_test(void);

/* Function for configuration of HRPWM test.*/
void hrpwm_test(void);

/* Function for configuration of filter test.*/
void filter_test(void);

/* Function for hardware filter test.*/
void hw_fltr_test(void);

/* Function for configuration of CLB test.*/
void CLB_test(void);

/* Print the statement */
void print(uint8_t* data, uint16_t length);

/* Function for external SPI flash test.*/
void spi_flash_test(void);

/* Function for external SPI flash handler.*/
void spi_flash_handler(void);

/* Function to print the current state */
void print_current_state();

/* Function to disable the peripheral */
void Disable_peripherals(void);

/* Consolidated peripheral initialization function */
void init_peripherals(void);

/*******************************************************************************
* Global Variables
*********************************************************************************/

/* Variable to hold the current state of the state machine */
extern volatile uint32_t   state;

/* Pointer to shared state variable for inter-core communication */
extern volatile uint32_t *sm_state;

/* Shared variable for lock on switch case */
extern volatile uint32_t *sm_lock;

/* Shared variable for the size of the sine-wave look-up table */
/* To change the output of DAC
 * for sine wave - '99'
 * for Rectified sine wave - '49' */
extern volatile uint32_t *sm_sine_counter;

/* Variable for the current ADC data */
extern int32_t current_adc_data;

/* Variable for the voltage of MCU in mV */
extern int32_t mcu_voltage;

/* Variable for AFE gain */
extern float afe_gain_t;

/* Variable for AFE scaling */
extern float afe_scale_t;

/* Variable for the power of MCU in mW */
extern float mcu_power;

/* Variable for shunt resistor in ohms */
extern float shunt_rstr;

/* Variable for calculating the average of current ADC data */
extern float adc_flt_data;

/* Variable for the real current consumed by MCU in mA */
extern float adc_flt_current;

/* Variable for the MCU current */
extern float mcu_current;

/* Variable for potentiometer data */
extern int32_t pot_adc_data;

/* Config variable for the ADC1 to change the configuration run time */
extern cy_stc_ppca_adc_config_t test_adc;

extern cy_stc_ppca_adc_config_t HrPWM_config;

/* Variable for the status of the flash */
extern DEVSTATUS  dev_status;

/* Create buffer for transmit data */
extern uint8_t pSendData[256];

/* Create Buffers to read data */
extern uint8_t pReadData[260];

/* Context variable for the SPI */
extern cy_stc_scb_spi_context_t mSPI_context;

#endif /* SOURCE_OOB_INIT_S_H_ */
