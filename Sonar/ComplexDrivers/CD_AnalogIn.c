#include "CD_AnalogIn.h"


enum { ADC_HALF_BUFFER = 180 };
volatile uint16 ADCbuf[ADC_HALF_BUFFER*2];
volatile uint32 samples = 0;

static void setup_adc_gpio(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;
	/* Enable GPIOA Periph clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure ADC1/2 channels as analog input */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void setup_adc_dma(void)
{
	DMA_InitTypeDef        DMA_InitStructure;
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel1);	
	
	/* DMA1 Channel1 Init */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1_2->CDR;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32)ADCbuf;
	DMA_InitStructure.DMA_BufferSize = sizeof(ADCbuf)/sizeof(ADCbuf[0]);
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* DMA interrupts */
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC | DMA_IT_HT, ENABLE);

	/* enable DMA IRQ */
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
} 

static void calibrate_adc(void)
{
	__IO uint16_t calibration_value_1 = 0;	///< __IO = volatile	

  /* ADC Calibration procedure */
  ADC_VoltageRegulatorCmd(ADC1, ENABLE);
 
  /* Insert delay equal to 10 µs */
  Delay_us(10);
  
  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);

  while(ADC_GetCalibrationStatus(ADC1) != RESET );
  calibration_value_1 = ADC_GetCalibrationValue(ADC1);

}

void setup_adc(void)
{
	ADC_CommonInitTypeDef	 ADC_CommonInitStructure;
	ADC_InitTypeDef        ADC_InitStructure;
		
	
	ADC_DeInit(ADC1);
		
	/* Configure the ADC clock */
	//RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div6);	// 12 MHz  
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div128);	//562,5khz
		
	/* Enable ADC1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
		
	setup_adc_gpio();
	setup_adc_dma();
	
  
	calibrate_adc();

	ADC_CommonStructInit(&ADC_CommonInitStructure);
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_RegSimul; 
	ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode; //ADC_Clock_SynClkModeDiv1
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1 ; // ADC_DMAAccessMode_2
	ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_Circular; 
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0; 
	ADC_CommonInit(ADC1, &ADC_CommonInitStructure);

	/* ADC setup */  
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;         
	ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;   
	ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;  
	ADC_InitStructure.ADC_NbrOfRegChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);


  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_2Cycles5);
	
	/* Configures the ADC DMA */
	ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
		
	/* Enable the ADC DMA */
	ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 and ADC2 */
  ADC_Cmd(ADC1, ENABLE);

	/* wait for ADC1 ADRDY */
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
	
	/* Enable the DMA channel */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* Start ADC1 Software Conversion */ 
	ADC_StartConversion(ADC1);
}

volatile int elso = 0;
volatile int masodik = 0;

/* Handles DMA1_Channel1 (ADC Buffer) interrupt request */
// void DMA1_Channel1_IRQHandler(void) __attribute__ ((section (".ccmram")));
 void DMA1_Channel1_IRQHandler(void)
 {
 	volatile uint16 *ptr;

	if(DMA1->ISR & DMA1_IT_TC1){
		//samples +=180;
		elso = TIM3->CNT;
		ptr = &ADCbuf[ADC_HALF_BUFFER];
		DMA1->IFCR = DMA_IFCR_CTCIF1;
		

	} else if(DMA1->ISR & DMA1_IT_HT1) {
		
 		/* Half transfer - 1st half buffer has data */
		//samples +=180;
		TIM3->CNT = 0;
		ptr = &ADCbuf[0];
		DMA1->IFCR = DMA_IFCR_CHTIF1;
 	}
 	else
 	{
 		/* Transfer complete - 2nd half buffer has data */
 		DMA1->IFCR = DMA_IFCR_CGIF1;
 	}


 }

 void CD_AnalogIn_Init(void){
	 setup_adc();
 }
