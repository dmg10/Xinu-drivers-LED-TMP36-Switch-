#include <xinu.h>

sid32 readSem;
unsigned int PIN;

void configure(struct adc_csreg *pReg)
{
	configureAFEClock(pReg, 3000000);
	stepConfigWriteProtect(pReg, 1);
	adcStepConfig(pReg);
	workModeConfig(pReg,ADC_GENERAL_PURPOSE_MODE);
	clearInterruptStatus(pReg);
	setInterruptBit(pReg,ADC_EOS_INTR);
	enableModule(pReg);
}

void configureAFEClock(struct adc_csreg *pReg, unsigned int freq)
{
	unsigned int clkDiv = 24000000/freq - 1;
	pReg->reg_adc_ClkDiv &= ~(0xFFFF);
	pReg->reg_adc_ClkDiv |= clkDiv;
}

void stepConfigWriteProtect(struct adc_csreg *pReg, unsigned int isWritable)
{
	pReg->reg_ctrl &= ~ (0x01<<2);
	pReg->reg_ctrl |= (isWritable<<2);
}

void workModeConfig(struct adc_csreg *pReg,unsigned int workMode)
{
	pReg->reg_ctrl &= ~ ADC_WORK_MODE_CLR;
	pReg->reg_ctrl |= (workMode<<ADC_WORK_MODE_SHIFT);
}
void stepEnable(struct adc_csreg *pReg, unsigned int selectStep)   
{
	pReg->reg_stepEnable |= (0x01<<selectStep);
}

void stepDisable(struct adc_csreg *pReg, unsigned int selectStep)
{
	pReg->reg_stepEnable &= ~(0x01<<selectStep);
}

void clearInterruptStatus(struct adc_csreg *pReg)
{
	pReg->reg_irqStatus |= 0x7FFF;
}

void enableModule(struct adc_csreg *pReg)
{
	pReg->reg_ctrl |= 0x01;
}
void disableModule(struct adc_csreg *pReg)
{
	pReg->reg_ctrl &= ~(0x01);
}

void adcStepConfig(struct adc_csreg *pReg)
{

	stepOperationModeControl(pReg,PIN,ADC_SINGLE_ENDED_OPER_MODE);	
	stepConfig(pReg,PIN, AIN6);
	stepFIFOSelConfig(pReg,PIN, FIFODATA_0);
	stepModeConfig(pReg,PIN, ADC_MODE_SW_CONTINUOUS);
	stepAverge(pReg,PIN,16);

}
void stepAverge(struct adc_csreg *pReg, int stepSelect, int sampleNum)
{
	if(sampleNum == 2)
		pReg->step[stepSelect].reg_stepConfig |= 0x01<<2;
	else if(sampleNum == 4)
		pReg->step[stepSelect].reg_stepConfig |= 0x02<<2;
	else if(sampleNum == 8)
		pReg->step[stepSelect].reg_stepConfig |= 0x03<<2;	
	else if(sampleNum == 16)
		pReg->step[stepSelect].reg_stepConfig |= 0x04<<2;
	else
		return;
}

void stepOperationModeControl(struct adc_csreg *pReg, unsigned int stepSelect, unsigned int mode)
{
	if(mode)
	{
		pReg->step[stepSelect].reg_stepConfig |= ADC_STEPCONFIG_DIFF;
	}
	else
	{
		pReg->step[stepSelect].reg_stepConfig &= ~ADC_STEPCONFIG_DIFF;
	}
}

void stepConfig(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int positiveChannel)
{
	
	pReg->step[stepSelect].reg_stepConfig &= ~ SEL_RFM_SWC_CLR;               
	pReg->step[stepSelect].reg_stepConfig &= ~ SEL_INP_SWC_CLR;               
	pReg->step[stepSelect].reg_stepConfig |= positiveChannel<<SEL_INP_SWC_SHIFT; 
	pReg->step[stepSelect].reg_stepConfig &= ~ SEL_INM_SWC_CLR;               
	pReg->step[stepSelect].reg_stepConfig &= ~ SEL_RFP_SWC_CLR;  

	pReg->step[stepSelect].reg_stepConfig &= ~ ADC_STEPCONFIG_XPPSW_SWC_CLR;
	pReg->step[stepSelect].reg_stepConfig &= ~ ADC_STEPCONFIG_XNPSW_SWC_CLR;
	pReg->step[stepSelect].reg_stepConfig &= ~ ADC_STEPCONFIG_YPPSW_SWC_CLR;
	pReg->step[stepSelect].reg_stepConfig &= ~ ADC_STEPCONFIG_XNNSW_SWC_CLR;
	pReg->step[stepSelect].reg_stepConfig &= ~ ADC_STEPCONFIG_YPNSW_SWC_CLR;
	pReg->step[stepSelect].reg_stepConfig &= ~ ADC_STEPCONFIG_YNNSW_SWC_CLR;
	pReg->step[stepSelect].reg_stepConfig &= ~ ADC_STEPCONFIG_WPNSW_SWC_CLR;              
}

void stepFIFOSelConfig(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int fifoNO)
{
	pReg->step[stepSelect].reg_stepConfig &= ~ FIFODATA_SEL_CLR;
	pReg->step[stepSelect].reg_stepConfig |= fifoNO<<FIFODATA_SEL_SHIFT;
}

void stepModeConfig(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int mode)
{
	pReg->step[stepSelect].reg_stepConfig &= ~ ADC_MODE_CLR;
	pReg->step[stepSelect].reg_stepConfig |= mode<<ADC_MODE_SHIFT;
}

void setInterruptBit(struct adc_csreg *pReg, unsigned int irqEnable)
{
	pReg->reg_irqEnable_Set |=irqEnable;
}

devcall	adcinit (struct	dentry *devptr)
{
	struct adc_csreg* pReg = (struct adc_csreg*)devptr->dvcsr; 

	readSem = semcreate(0);										
	initADCClk();
	initADCIntr(devptr);
	configure(pReg);
	return 1;
}

