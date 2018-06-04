/*
 * CTwi.h
 *
 * Created: 04.06.2018 15:12:30
 *  Author: Andrew
 */ 
 #define Twi100Khz  155

#define Writemode        0 
#define Readmode         1
#define NoMasterAckStop  0
#define YesMasterAck     1

#define bmp180           0x77

#ifndef CTWI_H_
#define CTWI_H_

class CTwi{

public:
    CTwi(){}
	CTwi(int speed){
	//----------------InitTWI
	TWIC.CTRL = 0;
	TWIC_MASTER_BAUD=speed;// Ïנט 32 Ìדצ, םא 100ךÃצ = 155(0x9B)
	TWIC_MASTER_CTRLA=TWI_MASTER_ENABLE_bm; //enable master
	TWIC_MASTER_CTRLB=0x00; //disable Inactive Bus Timeout, disable QCEN and SMEN
	TWIC_MASTER_CTRLC=0x00; //send ACK and NOACT initially
	TWIC_MASTER_STATUS=TWI_MASTER_RIF_bm|TWI_MASTER_WIF_bm|TWI_MASTER_ARBLOST_bm|TWI_MASTER_BUSERR_bm|TWI_MASTER_BUSSTATE0_bm; //initially clear all flags and select bus state as IDLE
	}
	~CTwi(){}

	void sendaddres(uint8_t addr,int rw);
	void senddata(uint8_t data);
	uint8_t readdata(int MasterAck);
};

void CTwi::sendaddres(uint8_t addr,int rw)
{
     if(rw)
     {
	     addr = addr<<1|0x01;
	     TWIC_MASTER_ADDR = addr;
     }
     else
     {

	     addr = addr<<1;
	     TWIC_MASTER_ADDR = addr;
     }
}


void CTwi::senddata(uint8_t data)
{
    while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm));
    TWIC_MASTER_DATA = data;
}

uint8_t CTwi::readdata(int MasterAck)
{
    uint8_t byte;
    while(!(TWIC_MASTER_STATUS & TWI_MASTER_RIF_bm));
    byte = TWIC_MASTER_DATA;
    if(MasterAck)
    {
	    TWIC_MASTER_CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
    }
    else
    {
	    TWIC_MASTER_CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;
    }
    return byte;
}

#endif /* CTWI_H_ */