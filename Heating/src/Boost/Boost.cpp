#include "Boost.h"

Boost::Boost(unsigned short int boostLengthMinutes) : BOOST_LENGTH_MINUTES(boostLengthMinutes) { }

bool Boost::turnOn(Clock& clock) {
    boostStartTime = clock.getTimeInMinutes();
    boostState = State::ON;
    return true;
}

bool Boost::turnOff() {
    boostState = State::OFF;
    return true;
}

bool Boost::toggle(Clock& clock) {
    switch (boostState) {
        case State::ON: {
            return turnOff();
        }
        case State::OFF: {
            return turnOn(clock);
        }
    }
    return false;
}

State Boost::checkState(Clock& clock) const {
    bool withinBoostPeriod = clock.timeDifferenceFromNowInMinutes(boostStartTime) < BOOST_LENGTH_MINUTES;
    if (!withinBoostPeriod) {
        boostState = State::OFF;
    }
    return boostState;
}