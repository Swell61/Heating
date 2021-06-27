#ifndef _WATER_h
#define _WATER_h

#include "../Enums/State.h"
#include "Component.h"

class Water : public Component {
    public:
        Water(Mode initialMode);
        Water();

        bool required(Clock& clock) const;
};

#endif