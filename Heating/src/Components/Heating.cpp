#include "Heating.h"

Heating::Heating(const ComponentTimerConfig& config, Mode initialMode, float minTemp, float maxTemp, float increment) : Component(config, initialMode), MIN_TEMP(minTemp), MAX_TEMP(maxTemp), INCREMENT(increment) { }

Heating::Heating(const ComponentTimerConfig& config, float minTemp, float maxTemp, float increment) : Heating(config, Mode::ON, minTemp, maxTemp, increment) { }

Heating::Heating(const ComponentTimerConfig& config, float increment) : Heating(config,  10.0f, 27.0f, increment) { }

bool Heating::required(Clock& clock, float currentTemp) {
    return Component::required(clock) && currentTemp < requestedTemp;
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