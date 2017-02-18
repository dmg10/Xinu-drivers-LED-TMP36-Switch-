#include <xinu.h>

pid32 receive_id;
pid32 send_id;
int32	slot1;
int32	slot2;
int32	slot3;
int32	retval;
uint32	remip;
uint16	remport;
char	buff[1500];
char edgeip[] = "192.168.0.100";	//assumed BBB got the ip and port of the edge 
uint32	ipaddr;
char arr[1500];
char str[15];
char* ar;
char* res;
int tempset = 0;
int templimit = 0;
unsigned int integer = 0;
unsigned int decimal = 0;
unsigned int PIN;
unsigned int temp_threshold1;
char data1[4];
unsigned int a=0;
float voltage;
int sw_status,id,rw;

process receiver(void)
{
	slot1 = udp_register(0, 0, 7);	//register a port for this process
	kprintf("entered");
    while(1)  {
    	memset(buff, 0, sizeof(buff));
        retval = udp_recvaddr(slot1, &remip, &remport, buff,
						sizeof(buff), 600000);	//receive the request from the edge
        if (retval == TIMEOUT) {
			continue;
		} else if (retval == SYSERR) {
			kprintf("error receiving UDP\n");
		}
		sw_status = newRead(3);		//read status of the switch
		//kprintf("Switch status is %d\n",sw_status);

		if(sw_status == 1){			//check if switch is on or off
			//kprintf("%c %c\n", buff[0], buff[1]);
			id = (int)buff[0] - '0';
			rw = (int)buff[1] - '0';
			//kprintf("%d %d \n",id, rw);
			switch (rw){
			case 0 :
				newWrite(id);			//write the led
				continue;
			case 1 :
				newRead(id);			//read the raw temperature data
				break;
			case 3 :
				res = buff + 2;
				templimit = (int)atoi(res);
				setThreshold(templimit);
				tempset = 1;
				//kprintf("tempset: %d templimit : %d\n",tempset, temp_threshold1);
				continue;
			case 4 :
				tempset = 0;
				//kprintf("tempset : %d\n",tempset);
				continue;
			default:
				continue;
			}
		if (id == 2){
			calculate();			//calculate the temperature value from raw
			strcpy(arr,"RESPONSE:2:");
			sprintf(str, "%d", (int)integer);
			strncat(arr, str, 20);
			//kprintf("arr : %s %d\n", arr, strlen(arr));
			ar = arr; 
			retval = udp_sendto(slot1, remip, remport, ar, strlen(ar));	//send the temperature value to the server through edge
			if (retval == SYSERR) {
				kprintf("udp_sendto failed\n");
				}
		}
	} else {
		char *arrs = "Switch is OFF!!";
		retval = udp_sendto(slot1, remip, remport, arrs, strlen(arrs));	//notufy the server that switch is OFF
			if (retval == SYSERR) {
				kprintf("udp_sendto failed\n");
    		}
		}
	}
	return OK;
}

void calculate(){				//calculate the value of the temp from the raw
			unsigned int vol = *(unsigned int*)data1;
			voltage = (vol*1.8/4095)*1000;
			integer = (unsigned int)voltage;
			decimal = (unsigned int)((voltage - integer)*10);
			a= (unsigned int) (500);
			integer = (unsigned int)((integer-a)/10);
			//kprintf("Temperature = %d.%d C\r\n",integer,decimal);
}

process senddata(void)
{ 
	slot2  = udp_register(0,0,6);	//register a port for this process
	//kprintf("ENtered");
    while(1)  {
    	kprintf("");
    	sw_status = newRead(3);
    	if(sw_status == 1){			//disable the functionality if swith is OFF
    	if(tempset==1){				//only check if user have set some temperature limit
	    	newRead(2);
	    	calculate();
	    	if (integer > temp_threshold1){		//constantly check the current temp with threshold
				
				newWrite(0);
				
				strcpy(arr,"OVERTEMP:");
				sprintf(str, "%d", integer);
				strncat(arr, str, 20);
				kprintf("arr : %s %d\n", arr, strlen(arr));
				char *arrsend = arr;
				int32 retvald2p = dot2ip(edgeip, &ipaddr); //convert the ip to decimal
				if ((int32)retvald2p == SYSERR) {
					kprintf("invalid IP address\n");
					return 1;
				}
	    		retval = udp_sendto(slot2, ipaddr, 1234, arrsend, strlen(arrsend)); //send information to server if temp is over the limit
				if (retval == SYSERR) {
					kprintf("udp_sendto failed\n");
    				}	
    			}	
    		}
    	}
	}
	return OK;
}

int32 bbbinit(){

	slot3  = udp_register(0,0,5);	//register a port 

	if (slot3 == SYSERR) {
		kprintf("could not reserve UDP port");
	}

	int32 retvald2p = dot2ip(edgeip, &ipaddr);
	
	if ((int32)retvald2p == SYSERR) {
		kprintf("invalid IP address\n");
		return 1;
	}
	//info = "Info;0:OUTPUT:LED1;1:OUTPUT:LED2;2:INPUT:TMP36;3:INPUT:SWITCH";
	int i = 0;
	while (1){
	//kprintf("%s\n",getstring());
	int32	retvalon = udp_sendto(slot3, ipaddr , 1234, getstring(), strlen(getstring()));	//get the mapping required and send it to the edge
	if (retvalon == SYSERR) {
			kprintf("udp_sendto failed\n");
		}
	//kprintf("trying to contact.... %d\n",i++);

	int32 ret = udp_recvaddr(slot3, &remip, &remport, buff, sizeof(buff), 6000);	// receive the ack from the edge
	if (ret == TIMEOUT) {
			continue;
		} else if (ret == SYSERR) {
			kprintf("error receiving UDP\n");
		}

	if(strcmp(buff,"Got it!!!") == 0 ){
			return 1;
		}
	}	
}


process	main(void)
{
	recvclr();
	receive_id = create(receiver, 4096, 50, "receiver", 0);		//create a process for receiving the request 
	send_id = create(senddata, 4096, 50, "senddata", 0);		//create a process for checking the temperature

	bbbinit();
	resched_cntl(DEFER_START);
	resume(receive_id);			//start the process
	resume(send_id);			//start the process
	resched_cntl(DEFER_STOP);

	return OK;
}