#ifndef _COMPONENTTIMERCONFIG_h
#define _COMPONENTTIMERCONFIG_h

#include "Config.h"
#include "../Enums/SystemComponent.h"
#include "../Enums/TimerPeriod.h"
#include "../Enums/State.h"

//typedef unsigned short int (ComponentTimerConfig::*ConfigReadFunc)(TimerPeriod, State);

class ComponentTimerConfig : private Config {
    private:
        unsigned short int (ComponentTimerConfig::*configReadFunc)(TimerPeriod, State) = &defaultReadTimerConfig;
        bool (ComponentTimerConfig::*configWriteFunc)(TimerPeriod, State, unsigned short int) = &defaultWriteTimerConfig;

        unsigned short int readHeaterTimerConfig(TimerPeriod period, State state);
        unsigned short int readWaterTimerConfig(TimerPeriod period, State state);
        bool writeHeaterTimerConfig(TimerPeriod period, State state, unsigned short int timeInMinutes);
        bool writeWaterTimerConfig(TimerPeriod period, State state, unsigned short int timeInMinutes);
        unsigned short int defaultReadTimerConfig(TimerPeriod period, State state);
        bool defaultWriteTimerConfig(TimerPeriod period, State state, unsigned short int timeInMinutes);


    public:
        ComponentTimerConfig(const Config& config, SystemComponent component);

        unsigned short int readTimer(TimerPeriod timePeriod, State systemState);
        bool writeTimer(TimerPeriod timPeriod, State systemState, unsigned short int timerInMinutes);
        unsigned char readIncrement();
};

#endif