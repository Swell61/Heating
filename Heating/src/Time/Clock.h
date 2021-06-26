#ifndef _CLOCK_h
#define _CLOCK_h

#include <DS3232RTC.h> 
#include <UIPEthernet.h>
#include "../Enums/TimeComponent.h"
#include "../Enums/ValueAdjustment.h"

class Clock {
    private:
        int midnight = 0;
	    bool adjustTime(bool (Clock::*updateFunction)(unsigned char), int currentValue, ValueAdjustment adjustmentDirection, int adjustmentValue);
        bool setSystemHour(unsigned char newHour);
        bool setSystemMinute(unsigned char newMinute);
        bool setSystemTime(unsigned long time);
        unsigned short int dstOffset(unsigned long time);
        unsigned long inline ntpUnixTime(UIPUDP &udp);
    public:
        Clock();
        bool adjustTime(TimeComponent timeComponent, ValueAdjustment adjustment);
        unsigned short int getTimeInMinutes();
        bool synchroniseWithNtp(UIPUDP& udp);
};

#endif