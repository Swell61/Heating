#include "Heating.h"

Heating::Heating(Mode initialMode, float increment) : Component(initialMode), INCREMENT(increment) { }

Heating::Heating(float increment) : Heating(Mode::ON, increment) { }

bool Heating::required(Clock& clock, float currentTemp) const {
    return Component::required(clock) && currentTemp < requestedTemperature;
}

void Heating::adjustRequestedTemp(ValueAdjustment adjustment) {
    switch (adjustment) {
        case ValueAdjustment::UP: {
            requestedTemperature += INCREMENT;
            break;
        }
        case ValueAdjustment::DOWN: {
            requestedTemperature -= INCREMENT;
            break;
        }
    }
}

void Heating::setRequestedTemp(float requestedTemp) {
    requestedTemperature = requestedTemp;
}

float Heating::getRequestedTemp() const {
    return requestedTemperature;
}