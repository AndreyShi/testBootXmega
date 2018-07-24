/*
 * CPin.h
 *
 * Created: 04.06.2018 9:01:01
 *  Author: Andrew
 */ 
 #define pin_out 1
 #define pin_in  0

#ifndef CPIN_H_
#define CPIN_H_

class CPin {
protected:
     PORT_t *portn;
     int pin;
  public: 
      

       CPin(PORT_t *port,int pinnum,int direct){
	   portn = port;
	   pin = pinnum;
	   if(direct)
	   port->DIRSET = pinnum;
	   }
       ~CPin(){}

	   void outset();
	   void outclr();
	   void shim10n100();
	   void PullDown();
	   bool OnPin();                       // 80 bytes PM
};

 void CPin::outset()
{
    portn->OUTSET = pin;
}


void CPin::outclr()
{
    portn->OUTCLR = pin; 
}



void CPin::shim10n100()
{
	outset();
	_delay_us(10);
	outclr();
	_delay_us(100);
}

void CPin::PullDown()
{
    if(pin == 0)portn->PIN0CTRL = PORT_OPC_PULLDOWN_gc;
	if(pin == 1)portn->PIN1CTRL = PORT_OPC_PULLDOWN_gc;
	if(pin == 2)portn->PIN2CTRL = PORT_OPC_PULLDOWN_gc;
	if(pin == 3)portn->PIN3CTRL = PORT_OPC_PULLDOWN_gc;
	if(pin == 4)portn->PIN4CTRL = PORT_OPC_PULLDOWN_gc;
	if(pin == 5)portn->PIN5CTRL = PORT_OPC_PULLDOWN_gc;	
}

bool CPin::OnPin()
{
     if(portn->IN&pin)
	 return true;
	 return false;
}

#endif /* CPIN_H_ */