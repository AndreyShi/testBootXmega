/*
 * testBoot.cpp
 *
 * Created: 31.05.2018 14:35:49
 * Author : Andrew
 */ 


#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>        // 50 bytes PM
#include "CSerial.h"
#include "CPin.h"
#include "CInterrupt.h"
#include "CCpu.h"
#include "CTwi.h"       


 int main(void) 
{   
   
   CCpu          cpu32Mhz;
   CInterrupt    inter(Boot);
   CPin          pin0portK(&PORTK,PIN0_bm,pin_out);
   CPin          pin5portK(&PORTK,PIN5_bm,pin_in);
   CSerial       InitUart(bdrate19200,microe793);
   //CSerial       InitUart(bdrate200000,lir540);
   CTwi          InitTwi(Twi100Khz);

   void (*exitboot)() = 0x0000; 
  
   pin0portK.outset();
   pin5portK.PullDown(5);

   bool Onboot  = false;   
             
   inter.enable();
   
    while (1) 
    { 	 
	    if(pin5portK.OnPin() || Onboot)
	    {
		    Onboot = true;					
	    }
	    else
	    {
		    pin0portK.outclr();
		    EIND = 0x00;
		    exitboot();
	    }
    }
}

ISR(USARTC0_RXC_vect)
{
	CSerial     uart;
	CTwi        twi;
	char        buf;

	buf = uart.Recievedchar();
	twi.sendaddres(bmp180,Writemode);
	twi.senddata(0xD0);
	twi.sendaddres(bmp180,Readmode);
	buf = twi.readdata(NoMasterAckStop); 
	uart.Sendchar(buf);
}