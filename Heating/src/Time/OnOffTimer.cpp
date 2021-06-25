#include "OnOffTimer.h"

OnOffTimer::OnOffTimer(unsigned short int minTime, unsigned short int maxTime, unsigned short int onTime, unsigned short int offTime) : 
    OnOffTimer(minTime, maxTime, DEFAULT_INCREMENT, onTime, offTime) { }

OnOffTimer::OnOffTimer(unsigned short int minTime, unsigned short int maxTime, unsigned char increment, unsigned short int onTime, 
    unsigned short int offTime) : MIN_TIME(minTime), MAX_TIME(maxTime), INCREMENT(increment), on(onTime), off(offTime) { }

bool OnOffTimer::checkTimesValid(unsigned short int on, unsigned short int off) {
    return on < off && on >= MIN_TIME && off <= MAX_TIME;
}

bool OnOffTimer::adjustTimer(State state, ValueAdjustment adjustment) {
    switch (state) {
        case State::ON: {
            return adjustOn(adjustment);
        }
        case State::OFF: {
            return adjustOff(adjustment);
        }
    }
    return false;
}

bool OnOffTimer::adjustOn(ValueAdjustment adjustment) {
    switch (adjustment) {
        case ValueAdjustment::UP: {
            if (checkTimesValid(on + INCREMENT, off)) {
                on += INCREMENT;
                return true;
            }
            return false;
        }
        case ValueAdjustment::DOWN: {
            if (checkTimesValid(on - INCREMENT, off)) {
                on -= INCREMENT;
                return true;
            }
            return false;
        }
    }
    return false;
}

bool OnOffTimer::adjustOff(ValueAdjustment adjustment) {
    switch (adjustment) {
        case ValueAdjustment::UP: {
            if (checkTimesValid(on, off + INCREMENT)) {
                off += INCREMENT;
                return true;
            }
            return false;
        }
        case ValueAdjustment::DOWN: {
            if (checkTimesValid(on, off - INCREMENT)) {
                off -= INCREMENT;
                return true;
            }
            return false;
        }
    }
    return false;
}

unsigned short int OnOffTimer::getOnTime() const {
    return on;
}

unsigned short int OnOffTimer::getOffTime() const {
    return off;
}

bool OnOffTimer::timerStatus(unsigned short int time) const {
    return time > on && time < off;
}