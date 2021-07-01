#ifndef _ONOFFTIMERCONFIG_h
#define _ONOFFTIMERCONFIG_h

#include "ComponentTimerConfig.h"
#include "../Enums/TimerPeriod.h"

class OnOffTimerConfig : public ComponentTimerConfig {
    
    private:
        unsigned short int (OnOffTimerConfig::*configReadFunc)(State) = &defaultReadTimerConfig;
        bool (OnOffTimerConfig::*configWriteFunc)(State, unsigned short int) = &defaultWriteTimerConfig;

        unsigned short int readMorningTimerConfig(State state);
        unsigned short int readAfternoonTimerConfig(State state);
        unsigned short int defaultReadTimerConfig(State state);
        bool writeMorningTimerConfig(State state, unsigned short int timeInMinutes);
        bool writeAfternoonTimerConfig(State state, unsigned short int timeInMinutes);
        bool defaultWriteTimerConfig(State state, unsigned short int timeInMinutes);

    public:
        OnOffTimerConfig(const ComponentTimerConfig& config, TimerPeriod period);

        unsigned short int readTimer(State state);
        bool writeTimer(State state, unsigned short int timeInMinutes);
};

#endif