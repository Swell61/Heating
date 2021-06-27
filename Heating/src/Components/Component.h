#ifndef _COMPONENT_h
#define _COMPONENT_h

#include "../Boost/Boost.h"
#include "../Enums/Mode.h"
#include "../Time/ComponentTimer.h"

class Component {
    private:
        Mode mode;

    protected:
        Boost boost;
        ComponentTimer timer;
        virtual bool required(Clock& clock) const;

    public:
        Component(Mode initialMode);
        Component();

        Boost& getBoost();
        ComponentTimer& getTimer();
        Mode getMode(Clock& clock) const;
};

#endif