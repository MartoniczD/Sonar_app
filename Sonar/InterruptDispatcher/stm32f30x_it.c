/**
  ******************************************************************************
  * @file   ./stm32f30x_it.c
  * @author  S&I Application Team
  * @version V1.0.0
  * @date    15-February-2014  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  */

/* Includes ------------------------------------------------------------------*/
#include "DRV_DrvList.h"
#include "OS_Types.h"
#include "CD_Delay.h"

#include "ECUA_CurrentTr.h"
#include "ECUA_HMI.h"

/** \defgroup Interrupt_Dispatcher
 *  \brief Brief description of Interrupt Dispatcher
 *  \details  Long description of Interrupt Dispatcher
 */

/** @addtogroup Sonar_Application
  * @{
  */

/** @addtogroup Interrupt_Dispatcher
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	Delay_us_DEC();
}

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                            */
/******************************************************************************/
/**
  * @brief  This function handles External line 4 interrupt request.
  * @note	Configured by ECUA - Current Transmitter as ERROR pin.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{    
	EXTI_ClearITPendingBit(CT_ERR_LINE);
}

static volatile uint32 alma = 0;
volatile uint8 flag = 0;


void DMA2_Channel3_IRQHandler(void){
	/*
	if(flag == 0){
		
		alma++;
	}
	
	if(alma >= 8 && flag == 0){
		((GPIO_TypeDef*)LED_GPIO)->ODR |= LED_PIN;
		//DAC_Cmd(DAC_Channel_1, DISABLE);
		DMA_Cmd(DMA2_Channel3, DISABLE);
		flag = 1;
		alma = 0;
	}*/
	DMA2->IFCR = DMA_IFCR_CGIF3;
}

void USART1_IRQHandler(void){

	sint8 ml_Indata_s8 = 0;
	ml_Indata_s8 = (sint8)USART1->RDR;

}

void TIM1_CC_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
  {
		if(flag == 0){
			alma++;
	  }
		
	if(alma >= 8 && flag == 0){
		((GPIO_TypeDef*)LED_GPIO)->ODR |= LED_PIN;
		TIM_Cmd(TIM1, DISABLE);

		flag = 1;
		alma = 0;
	}
		
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    
	} 
}

/**
  * @}
  */ 

/**
  * @}
  */ 


