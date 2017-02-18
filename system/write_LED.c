#include<gpio.h>
#include<xinu.h>
int32 *ptr;
int i;
void write_LED(int pin,int data){
	ptr=GPIO1_BASE;
	for(i=0;i<2;i++){
		//kprintf("Led for loop %d\n",i);
		setPin(pin);
		sleep(2);
		clrPin(pin);
		sleep(2);

	}
}

void setPin(int pin){
	ptr[GPIO_DATAOUT/4] |= (1<<pin);
	//kprintf("Pin %d set\n",pin);	
}

void clrPin(int pin){
	ptr[GPIO_DATAOUT/4] ^= (1<<pin);
	//kprintf("Pin %d clr\n",pin);	
}
