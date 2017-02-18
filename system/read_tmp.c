#include<xinu.h>
unsigned int PIN;
extern char data1[4];
void read_temp(unsigned int pin){
	PIN = pin;
	
	//kprintf("read temp entered\n");
	read(ADC,(char*)data1,1);	
}
