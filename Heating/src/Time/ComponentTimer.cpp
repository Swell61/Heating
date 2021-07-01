#include "ComponentTimer.h"

// ComponentTimer::ComponentTimer() : morning(OnOffTimer(MIN_MORNING, MAX_AFTERNOON, DEFAULT_MORNING_ON, DEFAULT_MORNING_OFF)),
//     afternoon(OnOffTimer(MIN_AFTERNOON, MAX_AFTERNOON, DEFAULT_AFTERNOON_ON, DEFAULT_AFTERNOON_OFF)) { }

ComponentTimer::ComponentTimer(ComponentTimerConfig config) : config(config), morning(OnOffTimer(MIN_MORNING, MAX_MORNING, OnOffTimerConfig(config, TimerPeriod::MORNING))),
    afternoon(OnOffTimer(MIN_AFTERNOON, MAX_AFTERNOON, OnOffTimerConfig(config, TimerPeriod::AFTERNOON))) { }

bool ComponentTimer::adjustTimer(TimerPeriod period, State state, ValueAdjustment adjustment) {
    switch (period) {
        case TimerPeriod::MORNING: {
            return morning.adjustTimer(state, adjustment);
        }
        case TimerPeriod::AFTERNOON: {
            return afternoon.adjustTimer(state, adjustment);
        }
    }
    return false;
}

OnOffTimer& ComponentTimer::getMorningTimer() {
    return morning;
}

OnOffTimer& ComponentTimer::getAfternoonTimer() {
    return afternoon;
}

bool ComponentTimer::timerStatus(Clock& clock) {
    return morning.timerStatus(clock.getTimeInMinutes()) || afternoon.timerStatus(clock.getTimeInMinutes());
}