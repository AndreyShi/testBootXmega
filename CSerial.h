/*
 * CSerial.h
 *
 * Created: 01.06.2018 14:19:40
 *  Author: Andrew
 */ 
 //#define F_CPU 32000000UL
// #include <util/delay.h>
#define bdrate19200    104
#define bdrate200000   9
#define bdrate115200   131

#define microe793      793
#define lir540         540

#ifndef CSERIAL_H_
#define CSERIAL_H_

class CSerial{
	protected:
	USART_t * port;
	public:
	CSerial(){}
	CSerial(USART_t * port_t)
	{
		port = port_t;
	}
	CSerial(int rate,int device)
	{
		if(device == microe793)
		{
			port = &USARTC0;//port C0 is used
			if(rate == bdrate19200 || rate == bdrate200000){USARTC0_BAUDCTRLB = 0;USARTC0_BAUDCTRLA = rate;}
			if(rate == bdrate115200){USARTC0_BAUDCTRLB = 208; USARTC0_BAUDCTRLA = rate;}
			
			USARTC0_CTRLA = USART_RXCINTLVL_MED_gc; //подключение прерываний
			USARTC0_CTRLC = USART_CHSIZE_8BIT_gc; //8 data bits, no parity and 1 stop bit
			USARTC0_CTRLB = USART_TXEN_bm | USART_RXEN_bm; //Включение приема передатчика
			PORTC.DIRSET = PIN3_bm;  // настройка ноги 3 порта С контроллера Xmega на выход!для передачи данных в микросхему Uart
		}
		if(device == lir540)
		{
			port = &USARTD1;//port D1 is used 
			USARTD1.BAUDCTRLB=0;
			USARTD1.BAUDCTRLA=rate;
			USARTD1_CTRLA = USART_RXCINTLVL_MED_gc;
			USARTD1.CTRLC=0x03;
			USARTD1.CTRLB=USART_TXEN_bm | USART_RXEN_bm;
			PORTD.OUTSET =0x80;
			PORTD.DIRSET =0x80;
		}
	}
	~CSerial(){}

	int  Recievedblock(unsigned char * buf,int max_block_size);
	void TransmitBlock(unsigned char * buf,int max_block_size);
	int Recievedchar(unsigned char & byte);
	void Sendchar(unsigned char out);
	void Sendconststring(const char *string);
	void Sendflashstring(const char *string);
};

int CSerial::Recievedblock(unsigned char * buf,int max_block_size)
{
	for(int x = 0;x<max_block_size;x++) 
	{ 
		if(!Recievedchar(buf[x]))return 0;
	}
	return 1;
}

void CSerial::TransmitBlock(unsigned char * buf,int max_block_size)
{
	for(int y = 0;y<max_block_size;y++)
		Sendchar(buf[y]);
}

#pragma GCC push_options
#pragma GCC optimize ("O0")
int CSerial::Recievedchar(unsigned char & byte)
{
   int waiting = 0;
	while(!(port->STATUS & USART_RXCIF_bm))
	{		
		if(waiting == 2100) return 0;
		waiting++;
	}
	byte = port->DATA;
	return 1;
}
#pragma GCC pop_options

void CSerial::Sendchar(unsigned char out)
{
	while(!(port->STATUS & USART_DREIF_bm) );//оператор ! делает результат условия противоположным
	port->DATA = out;
}


void CSerial::Sendconststring(const char *string)
{
	int index = 0;
	while(string[index] !='\0')
	{
		while(!(port->STATUS & USART_DREIF_bm) );//оператор ! делает результат условия противоположным
		port->DATA = string[index];
		index++;
	}
}

void CSerial::Sendflashstring(const char *string)
{
//	while(pgm_read_byte(string)!='\0')
//	{
//		Sendchar(pgm_read_byte(string));
//		string++;
//	}
}

#endif /* CSERIAL_H_ */