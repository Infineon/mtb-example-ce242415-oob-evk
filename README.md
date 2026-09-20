# PSOC&trade; Control C3M/P8: Out-of-the-box (OOB) demo

This example is the out-of-the-box (OOB) demo project for the PSOC&trade; Control C3M/P8 MCU evaluation kit, which includes the following code examples:

- Hello world
- Current consumption
- Differential ADC
- Peak current control mode
- PFC ZCD based PWM masking
- Phase shift full bridge
- HrPWM
- ADC Filter
- 3P3Z HW Filter
- CLB Custom logic block
- SPI based external flash programming

[View this README on GitHub.](https://github.com/Infineon/mtb-example-ce242415-oob-evk)

[Provide feedback on this code example.](https://yourvoice.infineon.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyNDI0MTUiLCJTcGVjIE51bWJlciI6IjAwMi00MjQxNSIsIkRvYyBUaXRsZSI6IlBTT0MmdHJhZGU7IENvbnRyb2wgQzNNL1A4OiBPdXQtb2YtdGhlLWJveCAoT09CKSBkZW1vIiwicmlkIjoiY2hpcmFnc2FtcGF0LmpoYXdhckBpbmZpbmVvbi5jb20iLCJEb2MgdmVyc2lvbiI6IjEuMC4wIiwiRG9jIExhbmd1YWdlIjoiRW5nbGlzaCIsIkRvYyBEaXZpc2lvbiI6Ik1DRCIsIkRvYyBCVSI6IklDVyIsIkRvYyBGYW1pbHkiOiJQU09DIn0=)

## Requirements

- [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) v3.9.0 or later (tested with v3.9.0)
- Board support package (BSP) minimum required version for:
   - KIT_PSC3M8_EVK: v2.2.0
- Programming language: C
- Associated parts: All [PSOC&trade; Control C3M/P8 MCU](https://www.infineon.com/products/microcontroller/32-bit-psoc-arm-cortex/32-bit-psoc-control-arm-cortex-m33-mcu/psoc-control-c3-performance-line) parts



## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm&reg; Embedded Compiler v14.2.1 (`GCC_ARM`) – Default value of `TOOLCHAIN`
- Arm&reg; Compiler v6.22 (`ARM`)
- IAR C/C++ Compiler v9.70.4 (`IAR`)


## Supported kits (make variable 'TARGET')

- [PSOC&trade; Control C3M8 Evaluation Kit](https://www.infineon.com/KIT_PSC3M8_EVK) (`KIT_PSC3M8_EVK`) – Default value of `TARGET`


## Hardware setup

This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly.


## Software setup

See the [ModusToolbox&trade; tools package installation guide](https://www.infineon.com/ModusToolboxInstallguide) for information about installing and configuring the tools package.

Install a terminal emulator if you do not have one. Instructions in this document use [Tera Term](https://teratermproject.github.io/index-en.html).


## Using the code example


### Create the project

The ModusToolbox&trade; tools package provides the Project Creator as both a GUI tool and a command line tool.

<details><summary><b>Use Project Creator GUI</b></summary>

1. Open the Project Creator GUI tool

   There are several ways to do this, including launching it from the dashboard or from inside the Eclipse IDE. For more details, see the [Project Creator user guide](https://www.infineon.com/ModusToolboxProjectCreator) (locally available at *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/docs/project-creator.pdf*)

2. On the **Choose Board Support Package (BSP)** page, select a kit supported by this code example. See [Supported kits](#supported-kits-make-variable-target)

   > **Note:** To use this code example for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work

3. On the **Select Application** page:

   a. Select the **Application(s) Root Path** and the **Target IDE**

      > **Note:** Depending on how you open the Project Creator tool, these fields may be pre-selected for you

   b. Select this code example from the list by enabling its check box

      > **Note:** You can narrow the list of displayed examples by typing in the filter box

   c. (Optional) Change the suggested **New Application Name** and **New BSP Name**

   d. Click **Create** to complete the application creation process

</details>


<details><summary><b>Use Project Creator CLI</b></summary>

The 'project-creator-cli' tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the 'project-creator-cli' tool. On Windows, use the command-line 'modus-shell' program provided in the ModusToolbox&trade; installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; tools. You can access it by typing "modus-shell" in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

The following example clones the "[mtb-example-ce242415-oob-evk](https://github.com/Infineon/mtb-example-ce242415-oob-evk)" application with the desired name "OOB" configured for the *KIT_PSC3M8_EVK* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id KIT_PSC3M8_EVK --app-id mtb-example-ce242415-oob-evk --user-app-name OOB --target-dir "C:/mtb_projects"
   ```

The 'project-creator-cli' tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the <id> field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the <id> field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

<br>

> **Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at {ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf).

</details>


### Open the project

After the project has been created, you can open it in your preferred development environment.


<details><summary><b>Eclipse IDE</b></summary>

If you opened the Project Creator tool from the included Eclipse IDE, the project will open in Eclipse automatically.

For more details, see the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_ide_user_guide.pdf*).

</details>


<details><summary><b>Visual Studio (VS) Code</b></summary>

Launch VS Code manually, and then open the generated *{project-name}.code-workspace* file located in the project directory.

For more details, see the [Visual Studio Code for ModusToolbox&trade; user guide](https://www.infineon.com/MTBVSCodeUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_vscode_user_guide.pdf*).

</details>



<details><summary><b>IAR Embedded Workbench</b></summary>

Open IAR Embedded Workbench manually, and create a new project. Then select the generated *{project-name}.ipcf* file located in the project directory.

For more details, see the [IAR Embedded Workbench for ModusToolbox&trade; user guide](https://www.infineon.com/MTBIARUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_iar_user_guide.pdf*).

</details>


<details><summary><b>Command line</b></summary>

If you prefer to use the CLI, open the appropriate terminal, and navigate to the project directory. On Windows, use the command-line 'modus-shell' program; on Linux and macOS, you can use any terminal application. From there, you can run various `make` commands.

For more details, see the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation

1. Use the PSOC&trade; Control C3M8 Evaluation Kit (`KIT_PSC3M8_EVK`)

2. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector

3. Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud

4. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE</b></summary>

      1. Select the application project in the Project Explorer

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**
   </details>


   <details><summary><b>In other IDEs</b></summary>

   Follow the instructions in your preferred IDE
   </details>


   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain is specified in the application's Makefile but you can override this value manually:
      ```
      make program TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TOOLCHAIN=GCC_ARM
      ```
   </details>

5. After programming, the application starts automatically. Confirm that "PSOC Control C3M/P8: Out-of-the-box (OOB) demo" is displayed on the UART terminal


   **Figure 1. Terminal output on program startup**

   ![](images/menu_of_code_example.png)

6. Press the **SW4(P7.0)** button to start the state machine

## Debugging

You can debug the example to step through the code.


<details><summary><b>In Eclipse IDE</b></summary>

Use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide).


</details>


<details><summary><b>In other IDEs</b></summary>

Follow the instructions in your preferred IDE.

</details>


## Design and implementation

- **Hello world:** UART peripheral is used to print the message

- **Current consumption:** ADC1 channel 0(AIN4P) and channel 1(AIN4N) in differential mode with AFE gain of 12. To verify the MCU reading, connect the multimeter in millivolt (mV) reading mode at TP17 and TP18 and divide the reading by 0.1 (Shunt resistor value)

- **Differential ADC:** ADC0 channel 0(AIN0P) and 1(AIN0N) in differential mode along with LEDs(6) is used to demonstrate the full range sensing (-3.3 V to 3.3 V)

- **Peak current control mode:** Rotate the potentiometer R212 (AIN0P) and observe the PWM on P5.0. The potentiometer connected to ADC 0 sets an I_REF value. A DAC is configured in Down Slope Mode (DCSG 0, slope generation slice 0). A TCPWM (TCPWM0 group0 counter1) generates a PWM signal. Moving the potentiometer reduces the duty cycle, since the DCSG compares the potentiometer input on one side with the DAC output on the other

- **PFC ZCD based PWM masking:** Connect AIN0 and A6P (AIN6P), and observe the PWM on P6_5 & P6_3. A DAC (DCSG Group 0 R2R DAC) is configured to generate the rectified VAC signal, which is routed to an input ADC (AIN6P) pin. A comparator (DCMP2) is needed to compare a rectified input voltage signal (emulating the AC line voltage) against a threshold voltage. TCPWM 0 Group 2 Counter 0. When the ADC count falls below the threshold, the PWM is masked

- **Phase shift full bridge:** Rotate the potentiometer R262 and observe the PWM on P8.0, P8.1 and P8.4,P8.3 - 2 TCPWM (TCPWM0 group2 counter1 and TCPWM0 group2 counter0) are used to generate two complementary pairs. One potentiometer (ADC group channel 3) is used to shift the ON-time of the two timers in a range from 0° to 360°

- **HrPWM:** Rotate the potentiometer (R221 and R212) and observe the PWM P7.1 and P9.2. One potentiometer (ADC0 channel 0) is used to set the duty cycle and one potentiometer (ADC0 channel 1) is used to set the period of TCPWM0 group0 counter3

- **ADC Filter:** Connect AIN0 and A6P (AIN6P) and observe the result on AIN1. A DAC (DCSG Group 0 R2R DAC) is configured to generate the signal which is routed to an input ADC (AIN6P) pin. The result is filtered through the ADC filter(Configured as low pass filter - ADC2_FLT). Observe the output at AIN1 (DCSG group 1 R2R DAC)

- **3P3Z HW Filter:** Connect AIN0 and AIN11P, and observe the result on AIN1. A DAC (DCSG Group 0 R2R DAC) is configured to generate the signal which is routed to an input ADC (AIN6P) pin. The result is filtered through the HW 3P3Z filter (Configured as Bandpass filter - HW_FLT_LF). Observe the output at AIN1 (DCSG group 1 R2R DAC)

- **CLB Custom logic block:** PWM logic is fed to the LUT of CLB. Observe the signal on pin P4.4

- **SPI External flash:** External SPI flash memory is programmed and verified via SPI interface


### Resources and settings

**Table 1. Application resources**

 Resource  |  Alias/object     |    Purpose
 :-------- | :-------------    | :------------
 UART (PDL) | DEBUG_UART | UART PDL object used for the Debug UART port
 UART (HAL) | DEBUG_UART_hal_obj | UART HAL object used by Retarget-IO for the debug UART port
 ADC0 | ADC0_HW | Differential ADC and HrPWM potentiometer reading
 ADC1 | ADC1_HW | Current consumption measurement (AIN4P/AIN4N)
 ADC2 | ADC2_HW | PFC and ADC filter signal acquisition (AIN6P)
 ADC3 | ADC3_HW | PSFB potentiometer and HW filter signal reading
 ADC1 AFE | ADC1_AFE_HW | Analog front-end with gain of 12 for current sensing
 ADC1 Filter | ADC_FLT_HW | Averaging filter for ADC1 current measurement
 ADC2 Filter | ADC2_FLT_HW | ADC2 filter for PFC and ADC filter demo
 ADC3 Filter | ADC3_FLT_HW | ADC3 filter for HW 3P3Z filter demo
 TCPWM0 Group0 Counter 0 | voltage_current_trg | PWM trigger for voltage/current consumption
 TCPWM0 Group0 Counter 1 | pwm_pccm | PWM for peak current control mode (PCCM)
 TCPWM0 Group0 Counter 3 | pwm_hrpwm | High-resolution PWM with duty/period control
 TCPWM0 Group1 Counter 0 | pwm_pfc | PWM for PFC signal generation
 TCPWM0 Group2 Counter 0 | pwm_pfc_ac | PWM for PFC ZCD-based masking
 TCPWM0 Group2 Counter 1 | pwm_psfb1 | PWM for phase shift full bridge (pair 1)
 TCPWM0 Group2 Counter 2 | pwm_psfb2 | PWM for phase shift full bridge (pair 2)
 DCSG Group 0 (Slope Gen) | DCSG_HW / DCSG_SL_HW | Down-slope DAC and ramp generation for PCCM
 DCSG Group 0 R2R DAC | DAC_HW | Signal generation for PFC, PSFB, and filter demos
 DCSG Group 1 R2R DAC | DAC_FLT_HW | Output DAC for filter and 3P3Z demos
 Digital Comparator | DCMP2_HW | Zero-crossing detection for PFC PWM masking
 HW 3P3Z Filter | HW_FLT_LF_HW | Hardware 3-pole 3-zero band-pass filter
 CLB | CLB_HW | Custom logic block for PWM logic demo
 SCB SPI | mSPI_HW | SPI master for external flash programming
 EPU | PPCA_EPU_EPU | Event processing unit for interrupt routing
 GPIO (Button) | USER_BUTTON_IRQ | User button (SW4/P7.0) for state machine control


<br>


## Related resources

Resources  | Links
-----------|----------------------------------
Code examples  | [Using ModusToolbox&trade;](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub
Device documentation | [PSOC&trade; Control C3M/P8 MCU documents](https://www.infineon.com/products/microcontroller/32-bit-psoc-arm-cortex/32-bit-psoc-control-arm-cortex-m33-mcu/psoc-control-c3-performance-line?ftab=01#Documents)
Development kits | Select your kits from the [Evaluation board finder](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board)
Libraries on GitHub  | [mtb-dsl-psc3m8](https://github.com/Infineon/mtb-dsl-psc3m8) – Device Support Library (DSL) <br> [retarget-io](https://github.com/Infineon/retarget-io) – Utility library to retarget STDIO messages to a UART port
Tools  | [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use libraries and tools enabling rapid development with Infineon MCUs for applications ranging from wireless and cloud-connected systems, edge AI/ML, embedded sense and control, to wired USB connectivity using PSOC&trade; Industrial/IoT MCUs, AIROC&trade; Wi-Fi and Bluetooth&reg; connectivity devices, XMC&trade; Industrial MCUs, and EZ-USB&trade;/EZ-PD&trade; wired connectivity controllers. ModusToolbox&trade; incorporates a comprehensive set of BSPs, HAL, libraries, configuration tools, and provides support for industry-standard IDEs to fast-track your embedded application development

<br>


## Other resources

Infineon provides a wealth of data at [www.infineon.com](https://www.infineon.com) to help you select the right device, and quickly and effectively integrate it into your design.


## Document history

Document title: *CE242415* – *PSOC&trade; Control C3M/P8: Out-of-the-box (OOB) demo*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example

<br>


All referenced product or service names and trademarks are the property of their respective owners.

The Bluetooth&reg; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.

PSOC&trade;, formerly known as PSoC&trade;, is a trademark of Infineon Technologies. Any references to PSoC&trade; in this document or others shall be deemed to refer to PSOC&trade;.

---------------------------------------------------------

(c) 2024-2026, Infineon Technologies AG, or an affiliate of Infineon Technologies AG. All rights reserved.
This software, associated documentation and materials ("Software") is owned by Infineon Technologies AG or one of its affiliates ("Infineon") and is protected by and subject to worldwide patent protection, worldwide copyright laws, and international treaty provisions. Therefore, you may use this Software only as provided in the license agreement accompanying the software package from which you obtained this Software. If no license agreement applies, then any use, reproduction, modification, translation, or compilation of this Software is prohibited without the express written permission of Infineon.
<br>
Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A SPECIFIC USE/PURPOSE OR MERCHANTABILITY. Infineon reserves the right to make changes to the Software without notice. You are responsible for properly designing, programming, and testing the functionality and safety of your intended application of the Software, as well as complying with any legal requirements related to its use. Infineon does not guarantee that the Software will be free from intrusion, data theft or loss, or other breaches (“Security Breaches”), and Infineon shall have no liability arising out of any Security Breaches. Unless otherwise explicitly approved by Infineon, the Software may not be used in any application where a failure of the Product or any consequences of the use thereof can reasonably be expected to result in personal injury.
