/*! \mainpage Sonar Application
 *
 * \section intro_sec Introduction
 *
 * Application software of Sonar project V1.0.
 *
 * @attention Teszt figyelmeztetes
 * Ha akarmi van akkor meg mi van?!
 */

/**
  ******************************************************************************
  * @file    main.c
  * @author  S&I Application Team
  * @version V1.0.0
  * @date    15-February-2014
  * @brief   This file provides firmware functions to manage the functionalities
  *  of the Ultrasonic level measurement system.
  *           + Initialization and Configuration
  *           + Measurement

  @verbatim
  ==============================================================================
                             ##### How to use this application #####
  ==============================================================================
    [..]
    Lorem ipsum dolor sit amet, sea et nonumy dissentiunt, no mel paulo doctus.
    His eloquentiam consectetuer ut. Exerci discere ne nec. Usu at dolore scriptorem,
    illum dolores oportere pri in, cum impetus diceret et. Putant delicata philosophia
    mea ex, quem dolorem qui ei, ut dico postea cum.

	Nec sale dolore aperiam eu, et summo discere nam, dico corpora eum ne.
	Utinam feugiat sea ex, ferri simul facilisi pro an, ferri option complectitur et eos.
	Percipitur interesset cu vix, te pri alienum deserunt, eos diam primis temporibus at.
	Vix ea rebum altera efficiantur, assum ponderum ex eum. Id elitr invidunt mea.



  @endverbatim
  *
  */


#include "DRV_DrvList.h"
#include "OS_Types.h"
#include "CD_Delay.h"
#include "CD_AnalogIn.h"
#include "CD_AnalogOut.h"
#include "ECUA_HMI.h"
#include "ECUA_SerialComm.h"

/** @addtogroup Sonar_Application
  * @brief Main application container
  * @{
  */

/** @addtogroup Main_Entry_Point
  * @brief This module contains the main entry point of the firmware.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void){

	
  /* Setup SysTick Timer for 1 µsec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000000)){
    while (1);
  }

  /* Complex Driver Level Init */
  CD_AnalogOut_Init();

  /* ECU Level Init */
  ECUA_HMI_Init();
  ECUA_Serial_Init();

  ECUA_Serial_Write("\r\nBOOTED!");


  //	setup_adc();
  //  CDOPAMP_Init();
	/*DAC_SetChannel2Data(DAC_Align_12b_R , 2570);
	Delay_us(50);
	DAC_SetChannel2Data(DAC_Align_12b_R , 0);
	Delay_us(70);
	*/

  while (1)
  {
		Delay_us(99999);
		ECUA_HMI_LedSwitch(LED_On);
		Delay_us(99999);
		ECUA_HMI_LedSwitch(LED_Off);
		

  }

}

/**
  * @}
  */

/**
  * @}
  */

//[EOF]

