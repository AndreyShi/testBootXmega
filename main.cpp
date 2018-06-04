/*
 * testBoot.cpp
 *
 * Created: 31.05.2018 14:35:49
 * Author : Andrew
 */ 


#include <avr/io.h>
#include <avr/pgmspace.h>
#include "CSerial.h"



 int main(void) 
{
   PORTK.DIRSET = PIN0_bm ;
   CSerial uart(104);
   void (*exitboot)() = 0x0000; 
    /* Replace with your application code */

    while (1) 
    { 	  
	  EIND = 0x00; 
	  exitboot();
    }
}

