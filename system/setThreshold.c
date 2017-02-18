#include <xinu.h>
/*
 Set threshold for temperature.  

*/
unsigned int temp_threshold1;
void setThreshold(unsigned int th)
{
	temp_threshold1= th;  
}
