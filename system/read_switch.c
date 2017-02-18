#include<xinu.h>
int32 d;
int32 *ptr;
int read_switch(int pin){
	
	ptr=GPIO1_BASE;
	d = ptr[GPIO_DATAIN/4];
	if(d & (1<<pin))
      	{	        	 
		return 1;
      	}
      	else
      	{
        	 return 0;
      	}
}

