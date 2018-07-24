/*
 * CInterrupt.h
 *
 * Created: 04.06.2018 13:41:22
 *  Author: Andrew
 */ 
 #define Boot 1
 #define App  0

#ifndef CINTERRUPT_H_
#define CINTERRUPT_H_


class CInterrupt
{
    public:
	CInterrupt(){}
	CInterrupt(int usesection){
	PMIC.CTRL |= PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm ;// активация контроллера прерываний 10 bytes PM
	if(usesection == Boot)
	{
	      CCP = CCP_IOREG_gc; //Разрешение записи в защищённый регистр
	      PMIC.CTRL |= PMIC_IVSEL_bm;  // активация таблицы прерываний загрузочного сектора
	}
	if(usesection == App)
	{
		CCP = CCP_IOREG_gc; //Разрешение записи в защищённый регистр
		PMIC.CTRL &= ~PMIC_IVSEL_bm;  // активация таблицы прерываний загрузочного сектора
	}
	}
	~CInterrupt(){}

	void enable();
	void disable();
};

void CInterrupt::enable()
{
     sei();
}



void CInterrupt::disable()
{
     cli();
}

#endif /* CINTERRUPT_H_ */