#ifndef _WATER_h
#define _WATER_h

#include "../Enums/State.h"
#include "Component.h"
#include "../Config/ComponentTimerConfig.h"

class Water : public Component {
    public:
        Water(const ComponentTimerConfig& config, Mode initialMode);
        Water(const ComponentTimerConfig& config);

        bool required(Clock& clock);
};

#endif