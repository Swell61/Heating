#ifndef _HEATING_h
#define _HEATING_h

#include "../Enums/State.h"
#include "Component.h"

class Heating : public Component {
    private:
        const float INCREMENT;
        float requestedTemperature = 11.5f;

    public:
        Heating(Mode initialState, float increment);
        Heating(float increment);

        float getRequestedTemp() const;
        void setRequestedTemp(float requestedTemp);
        void adjustRequestedTemp(ValueAdjustment adjustment);

        bool required(Clock& clock, float currentTemp) const; 
};

#endif