#ifndef _ONOFFTIMER_h
#define _ONOFFTIMER_h

#include "../Enums/State.h"
#include "../Enums/ValueAdjustment.h"
#include "../Config/OnOffTimerConfig.h"

#define DEFAULT_INCREMENT 5 // 5 minutes

class OnOffTimer {
    private:
        unsigned short int on;
        unsigned short int off;
        const unsigned short int MIN_TIME;
        const unsigned short int MAX_TIME;
        const unsigned char INCREMENT;

        OnOffTimerConfig config;

        bool checkTimesValid(unsigned short int on, unsigned short int off);
        bool adjustOn(ValueAdjustment adjustment);
        bool adjustOff(ValueAdjustment adjustment);

    public:
        // OnOffTimer(unsigned short int minTime, unsigned short int maxTime,
        //     unsigned short int onTime, unsigned short int offTime);
        // OnOffTimer(unsigned short int minTime, unsigned short int maxTime,
        //     unsigned char increment, unsigned short int onTime, unsigned short int offTime);
        OnOffTimer(unsigned short int minTime, unsigned short int maxTime, OnOffTimerConfig config); 
        
        bool adjustTimer(State state, ValueAdjustment adjustment);

        unsigned short int getOnTime() const;
        unsigned short int getOffTime() const;
        bool timerStatus(unsigned short int time) const;
};

#endif