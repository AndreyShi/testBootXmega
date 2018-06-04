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
	   void PullDown(int numPin);
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

void CPin::PullDown(int numPin)
{
    if(numPin == 5)
    portn->PIN5CTRL = PORT_OPC_PULLDOWN_gc;
	
}

bool CPin::OnPin()
{
     if(portn->IN&pin)
	 return true;
	 return false;
}

#endif /* CPIN_H_ */