/*
 * testBoot.cpp
 *
 * Created: 31.05.2018 14:35:49
 * Author : Andrew
 */ 
 
#define F_CPU 32000000UL
#define APP_END                   (APP_SECTION_START + APP_SECTION_SIZE)

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>        // 50 bytes PM
#include <util/delay.h>
#include "CSerial.h"
#include "CPin.h"
#include "CInterrupt.h"
#include "CCpu.h"
#include "CTwi.h"  
#include "enum.h"
#include "sp_driver.h"
    

extern "C"{extern void CCP_RST( void );}



bool end_of_flashing;
void exitboot();

void exitboot(){
	void (*gotozero)() = 0x0000;
	cli();
	SP_WaitForSPM();
	SP_LockSPM();
	USARTD1_CTRLA = USART_RXCINTLVL_OFF_gc;
	EIND = 0x00;
	gotozero();
}

 int main(void) 
{   
   
   CCpu          cpu32Mhz;
   CInterrupt    inter(Boot);
   CPin          pin0portK(&PORTK,PIN0_bm,pin_out);
   CPin          pin2portK(&PORTK,PIN2_bm,pin_out);
   CPin          pin5portK(&PORTK,PIN5_bm,pin_in);
   CSerial       InitUart(bdrate200000,microe793);
   //CSerial       InitUart(bdrate200000,lir540);
   CTwi          InitTwi(Twi100Khz);
 
   
   pin5portK.PullDown(5);

   bool Onboot  = false;   
             
   inter.enable();
   
    while (1) 
    { 	 
	   pin0portK.shim10n100();
	   //pin2portK.shim10n100();

	    if(pin5portK.OnPin()) // || Onboot
	    {
		    Onboot = true;					
	    }
	    if(!pin5portK.OnPin() || end_of_flashing)
	    {
		    pin0portK.outclr();
		    exitboot();
	    }
    }
}

ISR(USARTC0_RXC_vect)
{

	CSerial       uart(&USARTC0);
	CTwi          twi;
	
	unsigned char block[700] = { 0 };
	unsigned char answerFlash[] = { 0xff,0x02,0x04,0x0A };

	uart.Recievedblock(block,sizeof(block));
	
	if ( block[3] == WRITE_ || block[3] == 0x83)
	{
		unsigned long addres_page;

		if(block[3] == 0x83)
			addres_page = 255 + (int)block[6];
		else
			addres_page = (int)block[6] - 1;

		addres_page = addres_page * 512;

		if(addres_page >= APP_END)
			return;
		
		for(int x=0;x<APP_SECTION_PAGE_SIZE;x+=2)
		{
			uint16_t word;
			word = (uint8_t) block[x + 8];          //low
			word |= (uint8_t) block[x + 9]<<8;      //hi
			SP_WaitForSPM();
			SP_LoadFlashWord(x, word);
		}
		SP_WaitForSPM();
		SP_EraseWriteApplicationPage(addres_page);

		uart.TransmitBlock(answerFlash,sizeof(answerFlash));
	}


	if ( block[3] == TEST_ && block[4] == TEST_ ){ uart.TransmitBlock(block,8); }

	if ( block[3] == SET_TIME && block[4] == SET_TIME) { uart.TransmitBlock(block,1); }

	if ( block[3] == WRITE_CHECK_ONES){ end_of_flashing = true;}
	
	return;	
}


