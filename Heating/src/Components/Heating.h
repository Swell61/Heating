#ifndef _HEATING_h
#define _HEATING_h

#include "../Enums/State.h"
#include "Component.h"

class Heating : public Component {
    private:
        float requestedTemperature; 

    public:
        Heating(Mode initialState);
        Heating();

        float getRequestedTemp() const;
        void setRequestedTemp(float requestedTemp);

        bool required(Clock& clock, float currentTemp) const override; 
};

#endif