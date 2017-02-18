#include<xinu.h>
#include<gpio.h>
void newWrite(int device_index){
   switch(device_index){
    case 0:
 	  setDirection(GPIO1,6,OUTPUT);
	  write_LED(6,high);
 	  break;
     case 1:
 	  setDirection(GPIO1,2,OUTPUT);
	  write_LED(2,high);
 	  break;
 	default:
 	  break;
 }
}

int newRead(int device_index){
   switch(device_index){
    case 3:
 	  setDirection(GPIO1,7,INPUT);
	  read_switch(7);
 	   break;
     case 2:
 	  read_temp(0);
 	   break;
 	default:
 	   break;
 }
}
