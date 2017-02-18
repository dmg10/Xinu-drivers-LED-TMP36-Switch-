#include<xinu.h>
#include<gpio.h>
int32 *ptr;
int32 *p;
void setDirection(int gpio,int pin,int dir){
	//kprintf("Set Direction entered\n");
	if(gpio==0){
		ptr=GPIO0_BASE;
	}
	else if(gpio==1){
		ptr=GPIO1_BASE;
		//kprintf("GPIO %d set\n",gpio);	
	}
	else if(gpio==2){
		ptr=GPIO2_BASE;
	}
	else{
		ptr=GPIO3_BASE;
	}
	ptr[GPIO_OE/4] &= (0xFFFFFFFF ^(dir<<pin));
	//return;
}
