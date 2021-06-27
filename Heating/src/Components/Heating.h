#ifndef _HEATING_h
#define _HEATING_h

#include "../Enums/State.h"
#include "Component.h"

class Heating : public Component {
    private:
        const float INCREMENT;
        float requestedTemperature;

    public:
        Heating(Mode initialState);
        Heating();

        float getRequestedTemp() const;
        void setRequestedTemp(float requestedTemp);
        void adjustRequestedTemp(ValueAdjustment);

        bool required(Clock& clock, float currentTemp) const; 
};

#endif