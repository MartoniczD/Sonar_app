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


/* General Includes */
#include "DRV_DrvList.h"
#include "OS_Types.h"

/* CD Level Includes */
#include "CD_Delay.h"
#include "CD_AnalogIn.h"
#include "CD_AnalogOut.h"
#include "CD_UniqueID.h"
#include "CD_NVM.h"
#include "CD_TimeBase.h"

/* ECU Level Includes */
#include "ECUA_HMI.h"
#include "ECUA_SerialComm.h"
#include "ECUA_CurrentTr.h"
#include "ECUA_VGA.h"

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
CPUID cpuinfo __attribute__((at(UIDADDRBASE)));
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern volatile uint8 flag;

int main(void){
	
  /* Setup SysTick Timer for 1 µsec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000000)){
    while (1);
  }

  /* Complex Driver Level Init */
  //CD_AnalogOut_Init();
	CD_AnalogIn_Init();
  CD_TimeBase_Init();
	
  /* ECU Level Init */
  ECUA_HMI_Init();
  ECUA_Serial_Init();
  ECUA_VGainAmp_Init();
  //ECUA_CurrentTR_Init();
  //ECUA_CurrentTR_Switch(OUT_On);

  ECUA_Serial_Write("\r\nBOOTED!");


  while (1)
  {
	  	ECUA_HMI_LedSwitch(LED_On);
	  	Delay_ms(500);
//		ECUA_VGainAmp_Send(0x55);
		//if(flag > 0){

	//		DAC_Cmd(DAC_Channel_1, ENABLE);
	//		DMA_Cmd(DMA2_Channel3, ENABLE);
	//		flag = 0;
	//	}

		ECUA_HMI_LedSwitch(LED_Off);
		Delay_ms(500);

  }

}


/**
  * @}
  */

/**
  * @}
  */

//[EOF]

//CDNVM_WriteData32(0x00000000,(uint32*)&CDNVM_Memory.tempflag);
//uid1 = CDNVM_Memory.tempflag;

//	setup_adc();
//  CDOPAMP_Init();
	/*DAC_SetChannel2Data(DAC_Align_12b_R , 2570);
	Delay_us(50);
	DAC_SetChannel2Data(DAC_Align_12b_R , 0);
	Delay_us(70);
	*/
