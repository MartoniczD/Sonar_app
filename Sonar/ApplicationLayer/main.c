//.\Application;.\InterruptDispatcher;.\MicrocontrollerAbsLayer;.\MicrocontrollerAbsLayer\Startup;.\MicrocontrollerAbsLayer\CMSIS\Core;.\ComplexDrivers

#include "DRV_DrvList.h"
#include "OS_Types.h"
#include "sinehalf.h"
#include "CD_Delay.h"
#include "CD_AnalogIn.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DAC_DHR12RD_Address      0x40007408

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void TIM_Config(void);
static void DAC_Config(void);
static void DMA_Config(void);

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
  DMA_Config();
  DAC_Config();
	/* TIM2 */
  TIM_Config();

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



static void TIM_Config(void)
{
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

  /* TIM2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 54;         
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 TRGO selection: update event is selected as trigger for DAC */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}

static void DAC_Config(void)
{
  DAC_InitTypeDef   DAC_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  
  /* Fill DAC InitStructure */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits2_0;  
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  
  /* DAC channel1 Configuration */
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
  
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
  
	/* DAC channel2 Configuration */
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);
  
  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
  automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  
  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is 
  automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_2, ENABLE);
  
  /* Enable DMA for DAC Channel1 */
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}


static void DMA_Config(void)
{
  DMA_InitTypeDef   DMA_InitStructure;

  /* Enable DMA2 clock -------------------------------------------------------*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

  DMA_DeInit(DMA2_Channel3);
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32)&Sine12bit[0];
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 32;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel3, &DMA_InitStructure);

  /* Enable DMA2 Channel3 */
  DMA_Cmd(DMA2_Channel3, ENABLE);
	
	DMA_ITConfig(DMA2_Channel3, DMA_IT_HT, ENABLE);

	/* enable DMA IRQ */
	NVIC_EnableIRQ(DMA2_Channel3_IRQn);
}
