#include "Boost.h"

Boost::Boost(unsigned short int boostLengthMinutes) : BOOST_LENGTH_MINUTES(boostLengthMinutes) { }

void Boost::turnOn(Clock& clock) {
    boostStartTime = clock.getTimeInMinutes();
    boostState = State::ON;
}

void Boost::turnOff() {
    boostState = State::OFF;
}

State Boost::checkState(Clock& clock) const {
    bool withinBoostPeriod = clock.timeDifferenceFromNowInMinutes(boostStartTime) < BOOST_LENGTH_MINUTES;
    if (!withinBoostPeriod) {
        boostState = State::OFF;
    }
    return boostState;
}