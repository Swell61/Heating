#include "ComponentTimer.h"

ComponentTimer::ComponentTimer() : morning(OnOffTimer(MIN_MORNING, MAX_MORNING, DEFAULT_MORNING_ON, DEFAULT_MORNING_OFF)),
    afternoon(OnOffTimer(MIN_AFTERNOON, MAX_AFTERNOON, DEFAULT_AFTERNOON_ON, DEFAULT_AFTERNOON_OFF)) { }

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

const OnOffTimer& ComponentTimer::getMorningTimer() const {
    return morning;
}

const OnOffTimer& ComponentTimer::getAfternoonTimer() const {
    return afternoon;
}

bool ComponentTimer::timerStatus(unsigned short int time) const {
    return morning.timerStatus(time) || afternoon.timerStatus(time);
}