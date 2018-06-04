/*
 * CCpu.h
 *
 * Created: 04.06.2018 13:59:31
 *  Author: Andrew
 */ 


#ifndef CCPU_H_
#define CCPU_H_


class CCpu
{
     public:
     CCpu(){
	 //----------------InitCPU
	 OSC_CTRL = (1<<OSC_RC32MEN_bp);//разрешаем работу внутреннего тактового генератора 32MHz
	 while(!(OSC_STATUS & (1<<OSC_RC32MRDY_bp))); // ќжидание готовности внутреннего тактового генератора 32MHz;
	 CCP = 0xD8; //–азрешение записи в защищЄнный регистр
	 CLK_CTRL = 1;
     };
     ~CCpu(){};

};


#endif /* CCPU_H_ */