#ifndef _CLOCK_h
#define _CLOCK_h

#include <DS3232RTC.h> 
#include <EthernetENC.h>
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
        unsigned long inline ntpUnixTime(EthernetUDP& udp);
    public:
        const static unsigned short MINUTES_IN_A_DAY = 1440;

        Clock();
        bool adjustTime(TimeComponent timeComponent, ValueAdjustment adjustment);
        unsigned short int getTimeInMinutes();
        bool synchroniseWithNtp(EthernetUDP& udp);

        unsigned short int timeDifferenceFromNowInMinutes(unsigned short int startMinutes); // Should be const but Time library is not const correct
        static unsigned short int absoluteTimeDifferenceInMinutes(unsigned short int startMinutes, unsigned short int endMinutes);
};

#endif