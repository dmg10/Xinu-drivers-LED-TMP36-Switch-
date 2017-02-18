#include<xinu.h>
#include<gpio.h>
const char * getstring()
{
const char *word="Info;0:OUTPUT:LED1;1:OUTPUT:LED2;2:INPUT:TMP36;3:INPUT:SWITCH";
return word;
}
