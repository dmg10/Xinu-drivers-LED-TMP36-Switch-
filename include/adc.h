#ifndef ADC_H
#define ADC_H
extern unsigned int PIN;
extern unsigned int temp_threshold1;
struct adc_csreg
{
	volatile uint32 reg_Revison;
	volatile uint32 array1[3];
	volatile uint32 reg_sysconfig;
	volatile uint32 array2[4];
	volatile uint32 reg_irqStatus_Raw;
	volatile uint32 reg_irqStatus;
	volatile uint32 reg_irqEnable_Set;	
	volatile uint32 reg_irqEnable_Clr;
	volatile uint32 reg_irqWakeup;
	volatile uint32 reg_dmaEnable_Set;
	volatile uint32 reg_dmaEnable_Clr;
	volatile uint32 reg_ctrl;
	volatile uint32 reg_adcStat;
	volatile uint32 reg_adcRange;
	volatile uint32 reg_adc_ClkDiv;
	volatile uint32 reg_adc_Misc;
	volatile uint32 reg_stepEnable;
	volatile uint32 reg_idleConfig;
	volatile uint32 reg_ts_Charge_Stepconfig;
	volatile uint32 reg_ts_Charge_Delay;
	struct 
	{
		volatile uint32 reg_stepConfig;
		volatile uint32 reg_stepDelay;
	}step[16];
	struct 
	{
		volatile uint32 reg_fifoCount;
		volatile uint32 reg_fifoThreshold;
		volatile uint32 reg_dmaReq;
	}fifoInfo[2];
	volatile uint32 array3;
	volatile uint32 reg_fifoData0;
	volatile uint32 array4[63];
	volatile uint32 reg_fifoData1;
};

devcall	adcread(struct dentry* devptr, char* buff, int32 count);
devcall	adcinit (struct	dentry *devptr);
void adchandler(uint32 xnum);

#define TSC_ADC					(0x44E0D000)


#define ADC_STEPCONFIG_DIFF   (0x01<<25)

//differential mode or single ended mode
#define ADC_SINGLE_ENDED_OPER_MODE  (0)
#define ADC_DIFFERENTIAL_OPER_MODE	(1)

//ADC channel
#define AIN0            	(0)			//PIN AN0
#define AIN1           	        (1) 		//PIN AN1
#define AIN2            	(2)   		//PIN AN2
#define AIN3            	(3)   		//PIN AN3
#define AIN4            	(4)   		//PIN AN4
#define AIN5            	(5) 		//PIN AN5
#define AIN6            	(6) 		//PIN AN6
#define AIN7            	(7)			//PIN AN7



#define SEL_RFP_SWC_2_0				(0)
#define SEL_RFM_SWC_CLR			(0X03<<23)	
#define SEL_INP_SWC_CLR			 	(0XF<<19)	
#define SEL_INP_SWC_SHIFT			(19)		



#define SEL_INM_SWC_CLR			(0X0F<<15)	



#define SEL_RFP_SWC_CLR			(0x07<<12)	
#define ADC_STEPCONFIG_WPNSW_SWC_CLR   		(0x01<<11)

#define ADC_STEPCONFIG_XNNSW_SWC_CLR   		(0x01<<6)
#define ADC_STEPCONFIG_XNPSW_SWC_CLR   		(0x01<<9)
#define ADC_STEPCONFIG_XPPSW_SWC_CLR   		(0x01<<5)
#define ADC_STEPCONFIG_YNNSW_SWC_CLR   		(0x01<<8)
#define ADC_STEPCONFIG_YPNSW_SWC_CLR   		(0x01<<10)
#define ADC_STEPCONFIG_YPPSW_SWC_CLR   		(0x01<<7)

// FIFO field
#define FIFODATA_0                           	(0)
#define FIFODATA_1                           	(1)
#define FIFODATA_SEL_CLR			 	(0X01<<26)
#define FIFODATA_SEL_SHIFT			 	(26)

//MODE field
#define ADC_MODE_CLR						 	(0X03)
#define ADC_MODE_SW_ONE_SHOT					(0X00)
#define ADC_MODE_SW_CONTINUOUS					(0x01)
#define ADC_MODE_HW_ONE_SHOT					(0X10)
#define ADC_MODE_HW_CONTINUOUS					(0x11)
#define ADC_MODE_SHIFT							(0x00)

#define ADC_GENERAL_PURPOSE_MODE             	(0)		//for general purpose
#define ADC_WORK_MODE_CLR						(0x03<<5)
#define ADC_WORK_MODE_SHIFT						(5)		

//ADC interrupt bit set
#define ADC_EOS_INTR				(0x01<<1)	//end of sequence interrupt


#define CM_PER                                (0x44E00000)		
#define CM_PER_L3S_CLKSTCTRL                  (0x44E00004)
#define CM_PER_L3_CLKSTCTRL                   (0x44E0000C)
#define CM_PER_L3_INSTR_CLKCTRL               (0x44E000DC)
#define CM_PER_L3_CLKCTRL                     (0x44E000E0)
#define CM_PER_OCPWP_L3_CLKSTCTRL             (0x44E0012C)

#define CM_WKUP                               (0x44E00400)		
#define CM_WKUP_CLKSTCTRL                     (0x44E00400)
#define CM_WKUP_CONTROL_CLKCTRL               (0x44E00404)
#define CM_WKUP_L4WKUP_CLKCTRL                (0x44E0040C)
#define CM_L3_AON_CLKSTCTRL                   (0x44E00418)
#define CM_WKUP_ADC_TSC_CLKCTRL               (0x44E004BC)
#define CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL      (0x44E004CC)


#define INTR_CNTRL                  (0x48200000)
#define INTC_THRSHLD                        (0x48200068)
#define HWREG(n)	(*((volatile unsigned int *)(n)))


void stepModeConfig(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int mode);
void stepFIFOSelConfig(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int fifoNO);
void stepConfig(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int positiveChannel);
void stepOperationModeControl(struct adc_csreg *pReg, unsigned int stepSelect, unsigned int mode);
void adcStepConfig(struct adc_csreg *pReg);
void configure(struct adc_csreg *pReg);
void enableModule(struct adc_csreg *pReg);
void disableModule(struct adc_csreg *pReg);
void clearInterruptStatus(struct adc_csreg *pReg);
void clearInterruptStatus(struct adc_csreg *pReg);
void stepEnable(struct adc_csreg *pReg, unsigned int selectStep);
void workModeConfig(struct adc_csreg *pReg,unsigned int workMode);
void stepConfigWriteProtect(struct adc_csreg *pReg, unsigned int isWritable);

void configureAFEClock(struct adc_csreg *pReg, unsigned int freq);
void setInterruptBit(struct adc_csreg *pReg, unsigned int irqEnable);
void initADCClk();
void initADCIntr(struct	dentry *devptr);
void stepAverge(struct adc_csreg *pReg, int stepSelect, int sampleNum);
void setThreshold(unsigned int th);
#endif

