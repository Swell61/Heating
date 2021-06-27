#ifndef _COMPONENTTIMER_h
#define _COMPONENTTIMER_h

#include <Arduino.h>
#include "OnOffTimer.h"
#include "Clock.h"
#include "../Enums/TimerPeriod.h"
#include "../Enums/ValueAdjustment.h"

// All values in minutes from midnight
#define DEFAULT_MORNING_ON 420
#define DEFAULT_MORNING_OFF 640
#define DEFAULT_AFTERNOON_ON 1200
#define DEFAULT_AFTERNOON_OFF 1320

#define MIN_MORNING 0 // Midnight
#define MAX_MORNING MINUTES_IN_A_DAY/2 // Midday
#define MIN_AFTERNOON MINUTES_IN_A_DAY/2 // Midday
#define MAX_AFTERNOON MINUTES_IN_A_DAY // Midnight

class ComponentTimer {
    private:
        OnOffTimer morning;
        OnOffTimer afternoon;

    public:
        ComponentTimer();

        bool adjustTimer(TimerPeriod period, State state, ValueAdjustment adjustment);

        const OnOffTimer& getMorningTimer() const;
        const OnOffTimer& getAfternoonTimer() const;
        bool timerStatus(unsigned short int time) const;
};

#endif