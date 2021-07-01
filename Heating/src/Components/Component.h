#ifndef _COMPONENT_h
#define _COMPONENT_h

#include "../Boost/Boost.h"
#include "../Enums/Mode.h"
#include "../Time/ComponentTimer.h"
#include "../Enums/SystemComponent.h"
#include "../Config/ComponentTimerConfig.h"

class Component {
    private:
        Mode mode;

    protected:
        Boost boost;
        ComponentTimer timer;
        virtual bool required(Clock& clock);

    public:
        Component(const ComponentTimerConfig& config, Mode initialMode, SystemComponent component);
        Component(const ComponentTimerConfig& config, Mode initialMode);
        Component(const ComponentTimerConfig& config);

        Boost& getBoost();
        ComponentTimer& getTimer();
        Mode getMode() const;
        bool toggleMode();
};

#endif