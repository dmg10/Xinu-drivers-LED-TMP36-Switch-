#include <xinu.h>
unsigned int PIN;


void initADCClk()
{
	HWREG(CM_WKUP_CONTROL_CLKCTRL) |= 0x02;                            
	while(( HWREG(CM_WKUP_CONTROL_CLKCTRL) & 0x03 ) != 0x02 );          

	HWREG(CM_WKUP_CLKSTCTRL) |= 0x02;                                   
	while(( HWREG(CM_WKUP_CLKSTCTRL) & 0x03 ) != 0x02 );                

	HWREG(CM_WKUP_CLKSTCTRL) |= 0x02;                                  
	while(( HWREG(CM_WKUP_CLKSTCTRL) & 0x03 ) != 0x02 );              

	HWREG(CM_L3_AON_CLKSTCTRL) |= 0x02;                                 
	while(( HWREG(CM_L3_AON_CLKSTCTRL) & 0x03 ) != 0x02 );             
	HWREG(CM_WKUP_ADC_TSC_CLKCTRL) |= 0x02;                             
	while(( HWREG(CM_WKUP_ADC_TSC_CLKCTRL) & 0x03 ) != 0x02 );          

	while(( HWREG(CM_WKUP_CONTROL_CLKCTRL) & (0x03<<16) ) != 0x00 );            
	while(( HWREG(CM_L3_AON_CLKSTCTRL) & (0x01<<3) ) == 0x00 );                 
	while(( HWREG(CM_WKUP_L4WKUP_CLKCTRL) & (0x03<<16) ) != 0x00 );             
	while(( HWREG(CM_WKUP_CLKSTCTRL) & (0x01<<2) ) == 0x00 );                   
	while(( HWREG(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) & (0x01<<2) ) == 0x00 );    
	while(( HWREG(CM_WKUP_CLKSTCTRL) & (0x01<<14) ) == 0x00 );                  
	while(( HWREG(CM_WKUP_ADC_TSC_CLKCTRL) & (0x03<<16) ) != 0x00 );            
}

void initADCIntr(struct	dentry *devptr)
{
	struct	intc_csreg *csrptr = (struct intc_csreg *)0x48200000;   
	csrptr->threshold = 0X0FF;                                      
	set_evec(devptr->dvirq, (uint32)devptr->dvintr);                
	csrptr->ilr[devptr->dvirq] &= ~(0x01);                          
	csrptr->ilr[devptr->dvirq] |= (0x0A<<2);                        

}
