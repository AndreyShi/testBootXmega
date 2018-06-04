/*
 * CSerial.h
 *
 * Created: 01.06.2018 14:19:40
 *  Author: Andrew
 */ 
#define bdrate19200    104
#define bdrate200000   9

#define microe793      793
#define lir540         540

#ifndef CSERIAL_H_
#define CSERIAL_H_

class CSerial{
	public:
	CSerial(){}
	CSerial(int rate,int device)
	{
	   if(device == microe793)
	   {
		   USARTC0_BAUDCTRLB = 0;
		   USARTC0_BAUDCTRLA = rate; //  0x68(104) 19200 bod  system clock 32Mhz   // 200Kbit - 9
		   USARTC0_CTRLA = USART_RXCINTLVL_MED_gc; //подключение прерываний
		   USARTC0_CTRLC = USART_CHSIZE_8BIT_gc; //8 data bits, no parity and 1 stop bit
		   USARTC0_CTRLB = USART_TXEN_bm | USART_RXEN_bm; //Включение приема передатчика
		   PORTC.DIRSET = PIN3_bm;  // настройка ноги 3 порта С контроллера Xmega на выход!для передачи данных в микросхему Uart
	   }
	   if(device == lir540)
	   {
	       PORTD.OUTSET =0x80;
	       PORTD.DIRSET =0x80;
	       USARTD1.CTRLC=0x03;
	       USARTD1.BAUDCTRLA=rate;
	       USARTD1.BAUDCTRLB=0;
	       USARTD1.CTRLB=0x18;
	   }
	}
	~CSerial()
	{
	}
	char Recievedchar();
	void Sendchar(unsigned char out);
	void Sendconststring(const char *string);
	void Sendflashstring(const char *string);
};

char CSerial::Recievedchar()
{
    while(!(USARTC0_STATUS & USART_RXCIF_bm));
    return USARTC0_DATA;
}

void CSerial::Sendchar(unsigned char out)
{
    while(!(USARTC0_STATUS & USART_DREIF_bm) );//оператор ! делает результат условия противоположным
    USARTC0_DATA = out;
}


void CSerial::Sendconststring(const char *string)
{
    int index = 0;
    while(string[index] !='\0')
    {
	    while(!(USARTC0_STATUS & USART_DREIF_bm) );//оператор ! делает результат условия противоположным
	    USARTC0_DATA = string[index];
	    index++;
    }
}

void CSerial::Sendflashstring(const char *string)
{
    while(pgm_read_byte(string)!='\0')
    {
	    Sendchar(pgm_read_byte(string));
	    string++;
    }
}

#endif /* CSERIAL_H_ */