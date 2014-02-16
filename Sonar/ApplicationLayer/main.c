#include "DRV_DrvList.h"
#include "OS_Types.h"
#include "CD_Delay.h"
#include "CD_AnalogIn.h"
#include "CD_AnalogOut.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static void leed(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	/* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
	
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_DOWN ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}


int main(void){
	
  /* Setup SysTick Timer for 1 µsec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000000)){
    while (1);
  }

//  CDOPAMP_Init();
  CD_AnalogOut_Init();

//	setup_adc();
	
	leed();
	
  while (1)
  {
		Delay_us(99999);
		GPIO_SetBits(GPIOB,GPIO_Pin_4);
		Delay_us(99999);
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
		
		/*DAC_SetChannel2Data(DAC_Align_12b_R , 2570);
		Delay_us(50);
		DAC_SetChannel2Data(DAC_Align_12b_R , 0);
		Delay_us(70);
		*/
  }
}



