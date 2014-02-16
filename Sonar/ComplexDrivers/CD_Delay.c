#include "CD_Delay.h"

static volatile uint32 CDDELAY_ActVal = 0;

extern void Delay_us_DEC(void){
	
	if(CDDELAY_ActVal){
		CDDELAY_ActVal--;
	}else{
		;
	}
}


void Delay_us(uint32 pUsec){
	
	CDDELAY_ActVal = pUsec;
	while(CDDELAY_ActVal > 0);
	
	return;
}