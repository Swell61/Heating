#ifndef _BOOST_h
#define _BOOST_h

#include "../Enums/State.h"
#include "../Time/Clock.h"

class Boost {
    private:
        const unsigned short int BOOST_LENGTH_MINUTES;
        unsigned short int boostStartTime = 0;
        mutable State boostState = State::OFF;

    public:
        Boost(unsigned short int boostLength);
        bool turnOn(Clock& clock);
        bool turnOff();
        bool toggle(Clock& clock);
        State checkState(Clock& clock) const; // Should be const but Time library is not const correct
};

#endif