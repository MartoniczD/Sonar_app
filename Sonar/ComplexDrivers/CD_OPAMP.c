#include "CD_OPAMP.h"

void CDOPAMP_Init(void){

	/* Locals */
	OPAMP_InitTypeDef       OPAMP_InitStructure;
	GPIO_InitTypeDef        GPIO_InitStructure;

	/* OPAMP Peripheral clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA2 analog mode: It is used as OPAMP1 output */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* OPAMP1 config */
	OPAMP_InitStructure.OPAMP_NonInvertingInput = OPAMP_NonInvertingInput_IO3;
	OPAMP_InitStructure.OPAMP_InvertingInput =  OPAMP_InvertingInput_PGA;
	OPAMP_Init(OPAMP_Selection_OPAMP1, &OPAMP_InitStructure);

	/* Configure OPAMP1 in PGA mode with gain set to 2 */
	OPAMP_PGAConfig(OPAMP_Selection_OPAMP1, OPAMP_OPAMP_PGAGain_2, OPAMP_PGAConnect_IO1);

	/* Enable OPAMP1 */
	OPAMP_Cmd(OPAMP_Selection_OPAMP1, ENABLE);

	return;
}
