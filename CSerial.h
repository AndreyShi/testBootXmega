/*
 * CSerial.h
 *
 * Created: 01.06.2018 14:19:40
 *  Author: Andrew
 */ 


#ifndef CSERIAL_H_
#define CSERIAL_H_

class CSerial{
	public:
	CSerial(int rate)
	{
		USARTC0_BAUDCTRLB = 0;
		USARTC0_BAUDCTRLA = rate; //  0x68(104) 19200 bod  system clock 32Mhz   // 200Kbit - 9
		USARTC0_CTRLA = USART_RXCINTLVL_MED_gc; //����������� ����������
		USARTC0_CTRLC = USART_CHSIZE_8BIT_gc; //8 data bits, no parity and 1 stop bit
		USARTC0_CTRLB = USART_TXEN_bm | USART_RXEN_bm; //��������� ������ �����������
		PORTC.DIRSET = PIN3_bm;  // ��������� ���� 3 ����� � ����������� Xmega �� �����!��� �������� ������ � ���������� Uart
	}
	~CSerial()
	{
	     PORTK_OUTSET = PIN0_bm; //��������� ����� ���� �� ��������� � boot
	}
};



#endif /* CSERIAL_H_ */