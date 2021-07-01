#ifndef _ONOFFTIMERCONFIG_h
#define _ONOFFTIMERCONFIG_h

#include "ComponentTimerConfig.h"
#include "../Enums/TimerPeriod.h"

class OnOffTimerConfig : public ComponentTimerConfig {
    
    private:
        unsigned short int (OnOffTimerConfig::*configReadFunc)(State) = &defaultTimerConfig;

        unsigned short int readMorningTimerConfig(State state);
        unsigned short int readAfternoonTimerConfig(State state);
        unsigned short int defaultTimerConfig(State state);

    public:
        OnOffTimerConfig(const ComponentTimerConfig& config, TimerPeriod period);

        unsigned short int readTimer(State state);
};

#endif