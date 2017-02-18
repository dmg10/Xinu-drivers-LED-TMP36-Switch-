#include <xinu.h>


extern sid32 readSem;
unsigned int PIN_N;
unsigned int PIN;

devcall	adcread(struct dentry* devptr,char* buffer, int32 count)
{
	struct adc_csreg* pReg = (struct adc_csreg*)devptr->dvcsr;
	PIN_N=PIN+1;
	stepEnable(pReg,PIN_N); // ADC step enable

	wait(readSem);

	unsigned int data = 0;
	int sampleNum = pReg->fifoInfo[0].reg_fifoCount;
	int i;
	for(i = 0; i < sampleNum; i++)
	{
		data = pReg->reg_fifoData0 &(0xFFF) ;
	}
	buffer[3] = (data&(0xff000000))>>24;
	buffer[2] = (data&(0x00ff0000))>>16;	
	buffer[1] = (data&(0x0000ff00))>>8;
	buffer[0] = (data&(0x000000ff));
	return 1;
}

