#ifndef _HEATING_h
#define _HEATING_h

#include "../Enums/State.h"
#include "Component.h"

class Heating : public Component {
    private:
        const float INCREMENT;
        const float MIN_TEMP;
        const float MAX_TEMP;
        float requestedTemp = 11.5f;

    public:
        Heating(Mode initialState, float minTemp, float maxTemp, float increment);
        Heating(float minTemp, float maxTemp, float increment);
        Heating(float increment);

        float getRequestedTemp() const;
        bool setRequestedTemp(float requestedTemp);
        bool adjustRequestedTemp(ValueAdjustment adjustment);

        bool required(Clock& clock, float currentTemp); 
};

#endif