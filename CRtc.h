/*
 * CRtc.h
 *
 * Created: 25.07.2018 12:53:10
 *  Author: Andrew
 */ 


#ifndef CRTC_H_
#define CRTC_H_

class CRtc {
	public:
	CRtc(int device)
	{
		if(device == 793)
			CLK.RTCCTRL=CLK_RTCEN_bm+CLK_RTCSRC_TOSC32_gc;
		if(device == 540)
			//CLK.RTCCTRL=CLK_RTCEN_bm+CLK_RTCSRC_RCOSC32_gc;	  //äëÿ XMEGA256A3U (ËÈÐ)

		RTC.CTRL=0x01;
		unsigned int i = 324;
		while(!(RTC.STATUS==0));
		RTC.PER=i;
		RTC.COMP=0x00;
		RTC.CNT=0x00;
		RTC.INTCTRL=0x02;
	}
	~CRtc(){};
};



#endif /* CRTC_H_ */