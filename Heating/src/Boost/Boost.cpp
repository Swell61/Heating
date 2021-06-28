#include "Boost.h"

Boost::Boost(unsigned short int boostLengthMinutes) : BOOST_LENGTH_MINUTES(boostLengthMinutes) { }

void Boost::turnOn(Clock& clock) {
    Serial.println("Boost on");
    boostStartTime = clock.getTimeInMinutes();
    boostState = State::ON;
}

void Boost::turnOff() {
    boostState = State::OFF;
}

void Boost::toggle(Clock& clock) {
    switch (boostState) {
        case State::ON: {
            turnOff();
            break;
        }
        case State::OFF: {
            turnOn(clock);
            break;
        }
    }
}

State Boost::checkState(Clock& clock) const {
    bool withinBoostPeriod = clock.timeDifferenceFromNowInMinutes(boostStartTime) < BOOST_LENGTH_MINUTES;
    if (!withinBoostPeriod) {
        boostState = State::OFF;
    }
    return boostState;
}