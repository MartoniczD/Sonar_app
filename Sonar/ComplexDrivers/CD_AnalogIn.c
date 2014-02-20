/* @file    CD_AnalogIn.c
 * @author  S&I Application Team
 * @version V1.0.0
 * @date    16-February-2014
 * @brief   This file provides firmware functions to manage the Analog Input part of the ECU.
 *           + Initialization and Configuration
 *           + Measurement
 */


#include "CD_AnalogIn.h"


enum { 
	
	ADC_BUFFER_SIZE		 		= 36,
	ADC_HALF_BUFFER_SIZE	= 18

};
volatile uint16 ADCbuf[ADC_BUFFER_SIZE];

/**
 * @section Local functions
 */
static void setup_adc_gpio(void);
static void setup_adc_dma(void);
static void calibrate_adc(void);
static void setup_adc(void);
/**
 *
 *
 */



/** @addtogroup Sonar_Application
  * @brief Main application container
  * @{
  */


/** @addtogroup ComplexDrivers
  * @brief Drivers of mcu c. peripherals.
  * @{
  */

/** @addtogroup Analog_Input
  * @brief Drivers of the ADC interface.
  * @details Initializes the ADC/DMA interface.
  * @{
  */


/**
 * @name CD_AnalogIn_Init(void)
 * @brief Initializes the analog output.
 * @param None
 * @return None
 * @callergraph
 */
void CD_AnalogIn_Init(void){

	setup_adc();

	return;
}


/*
 * @}
 */

/*
 * @}
 */

/*
 * @}
 */

static void setup_adc_gpio(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;
	/* Enable GPIOA Periph clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure ADC1/2 channels as analog input */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void setup_adc_dma(void)
{
	DMA_InitTypeDef        DMA_InitStructure;
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Channel1);
	
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

	DMA_Init(DMA2_Channel1, &DMA_InitStructure);

	/* DMA interrupts */
	DMA_ITConfig(DMA2_Channel1, DMA_IT_TC | DMA_IT_HT, ENABLE);

	/* enable DMA IRQ */
	NVIC_EnableIRQ(DMA2_Channel1_IRQn);
} 

static void calibrate_adc(void)
{
	__IO uint16_t calibration_value_1 = 0;	///< __IO = volatile	
//	__IO uint16_t calibration_value_2 = 0;	

  /* ADC Calibration procedure */
  ADC_VoltageRegulatorCmd(ADC2, ENABLE);
 // ADC_VoltageRegulatorCmd(ADC2, ENABLE);
  
  /* Insert delay equal to 10 µs */
  Delay_us(10);
  
  ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC2);

//  ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);
//  ADC_StartCalibration(ADC2);
  
  while(ADC_GetCalibrationStatus(ADC2) != RESET );
  calibration_value_1 = ADC_GetCalibrationValue(ADC2);

//  while(ADC_GetCalibrationStatus(ADC2) != RESET );
//  calibration_value_2 = ADC_GetCalibrationValue(ADC2);	
}

static void setup_adc(void)
{
	ADC_CommonInitTypeDef	 ADC_CommonInitStructure;
	ADC_InitTypeDef        ADC_InitStructure;
		
	
	//ADC_DeInit(ADC1);
	ADC_DeInit(ADC2);
	
	/* Configure the ADC clock */
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div6);	// 12 MHz  
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
	ADC_CommonInit(ADC2, &ADC_CommonInitStructure);

	/* ADC setup */  
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b; 
	ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;         
	ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;   
	ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;  
	ADC_InitStructure.ADC_NbrOfRegChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);
	//ADC_Init(ADC2, &ADC_InitStructure);

	// PA0 = ADC1 IN1 (channel 1)
  ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_19Cycles5);
	// PA4 = ADC2 IN1 (channel 1)
  //ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_19Cycles5);	
	
	/* Configures the ADC DMA */
	ADC_DMAConfig(ADC2, ADC_DMAMode_Circular);
		
	/* Enable the ADC DMA */
	ADC_DMACmd(ADC2, ENABLE);

  /* Enable ADC1 and ADC2 */
  ADC_Cmd(ADC2, ENABLE);
  //ADC_Cmd(ADC2, ENABLE);

	/* wait for ADC1 ADRDY */
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY));
	
  /* wait for ADC2 ADRDY */
  //while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY));	

	/* Enable the DMA channel */
	DMA_Cmd(DMA2_Channel1, ENABLE);

	/* Start ADC1 Software Conversion */ 
	ADC_StartConversion(ADC2);
}

/* Handles DMA1_Channel1 (ADC Buffer) interrupt request */
// void DMA1_Channel1_IRQHandler(void) __attribute__ ((section (".ccmram")));
 void DMA2_Channel1_IRQHandler(void)
 {
 	volatile uint16 *ptr;

 	if(DMA2->ISR & DMA2_IT_HT1)
 	{
 		/* Half transfer - 1st half buffer has data */
 		ptr = &ADCbuf[0];
 	}
 	else
 	{
 		/* Transfer complete - 2nd half buffer has data */
 		ptr = &ADCbuf[ADC_HALF_BUFFER_SIZE];
 	}

 	/* process the buffer */
// 	adc_data_callback(ptr, ADC_HALF_BUFFER);

 	/* Clear DMA1_Channel1 interrupt */
 	DMA2->IFCR = DMA_IFCR_CGIF1;
 }

