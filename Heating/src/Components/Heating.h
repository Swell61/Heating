#ifndef _HEATING_h
#define _HEATING_h

#include "../Enums/State.h"
#include "Component.h"
#include "../Config/ComponentTimerConfig.h"
#include "../TempSensor.h"

class Heating : public Component {
    private:
        const float INCREMENT;
        const float MIN_TEMP;
        const float MAX_TEMP;
        const float MIN_TEMP_DEVIATION; // to stop heating flip flopping on and off
        float requestedTemp = 11.5f;
        bool heatingTempReached = false;

        bool tempDiffTooLarge(float currentTemp);

    public:
        Heating(const ComponentTimerConfig& config, Mode initialState, float minTemp, float maxTemp, float increment, float minTempDeviation);
        Heating(const ComponentTimerConfig& config, Mode initialState, float minTemp, float maxTemp, float increment);
        Heating(const ComponentTimerConfig& config, float minTemp, float maxTemp, float increment);
        Heating(const ComponentTimerConfig& config, float increment);

        float getRequestedTemp() const;
        bool setRequestedTemp(float requestedTemp);
        bool adjustRequestedTemp(ValueAdjustment adjustment);

        bool required(Clock& clock, TempSensor& tempSensor); 
};

#endif