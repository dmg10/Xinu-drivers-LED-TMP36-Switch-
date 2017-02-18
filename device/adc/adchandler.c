#include <xinu.h>

extern sid32 readSem;
unsigned int PIN;

void adchandler(uint32 xnum)
{
	struct	dentry	*devptr;            	
	struct	adc_csreg *csrptr;          	
	volatile uint32	irqStatus = 0;     
	 
	devptr = (struct dentry *) &devtab[ADC];
	csrptr = (struct adc_csreg *) devptr->dvcsr;
	irqStatus = csrptr->reg_irqStatus;      
	csrptr->reg_irqStatus = irqStatus;     

	stepDisable(csrptr,PIN); // ADC step disable
	semcount(readSem);               
	signal(readSem);                    	
	return;
}

