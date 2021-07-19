#include "Heating.h"

Heating::Heating(const ComponentTimerConfig& config, Mode initialMode, float minTemp, float maxTemp, float increment, float minTempDeviation) : Component(config, initialMode), MIN_TEMP(minTemp), MAX_TEMP(maxTemp), INCREMENT(increment), MIN_TEMP_DEVIATION(minTempDeviation) { }

Heating::Heating(const ComponentTimerConfig& config, Mode initialMode, float minTemp, float maxTemp, float increment) : Heating(config, initialMode, minTemp, maxTemp, increment, 0.75) { }

Heating::Heating(const ComponentTimerConfig& config, float minTemp, float maxTemp, float increment) : Heating(config, Mode::ON, minTemp, maxTemp, increment) { }

Heating::Heating(const ComponentTimerConfig& config, float increment) : Heating(config,  10.0f, 27.0f, increment) { }

bool Heating::required(Clock& clock, TempSensor& tempSensor) {
    return Component::required(clock) && tempDiffTooLarge(tempSensor.getTemp());
}

bool Heating::tempDiffTooLarge(float currentTemp) {
    if (currentTemp >= requestedTemp) {
        heatingTempReached = true;
        return false;
    }    
    else if (currentTemp <= requestedTemp - MIN_TEMP_DEVIATION) { // This stops heating from flip flopping between on and off when at the requested temperature
        heatingTempReached = false;
        return true;
    }
    else {
        return !heatingTempReached;
    }
}

bool Heating::adjustRequestedTemp(ValueAdjustment adjustment) {
    switch (adjustment) {
        case ValueAdjustment::UP: {
            return setRequestedTemp(requestedTemp + INCREMENT);
        }
        case ValueAdjustment::DOWN: {
            return setRequestedTemp(requestedTemp - INCREMENT);
        }
    }
    return false;
}

bool Heating::setRequestedTemp(float requestedTemp) {
    if (requestedTemp <= MAX_TEMP && requestedTemp >= MIN_TEMP) {
        (*this).requestedTemp = requestedTemp;
        return true;
    }
    return false;
}

float Heating::getRequestedTemp() const {
    return requestedTemp;
}