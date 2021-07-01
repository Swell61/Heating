#ifndef _COMPONENTTIMERCONFIG_h
#define _COMPONENTTIMERCONFIG_h

#include "Config.h"
#include "../Enums/SystemComponent.h"
#include "../Enums/TimerPeriod.h"
#include "../Enums/State.h"

//typedef unsigned short int (ComponentTimerConfig::*ConfigReadFunc)(TimerPeriod, State);

class ComponentTimerConfig : private Config {
    private:
        unsigned short int (ComponentTimerConfig::*configReadFunc)(TimerPeriod, State) = &defaultTimerConfig;

        unsigned short int readHeaterTimerConfig(TimerPeriod period, State state);
        unsigned short int readWaterTimerConfig(TimerPeriod period, State state);
        unsigned short int defaultTimerConfig(TimerPeriod period, State state);

    public:
        ComponentTimerConfig(const Config& config, SystemComponent component);

        unsigned short int readTimer(TimerPeriod timePeriod, State systemState);
        unsigned char readIncrement();
};

#endif