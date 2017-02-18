#include<xinu.h>
unsigned int temp_threshold1;
void check_temp(unsigned int temp){
	if(temp >=temp_threshold1){
		kprintf("Temperature above %d\n", temp_threshold1); 		
		newWrite(0);
	}
}
