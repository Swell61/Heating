#include "Heating.h"

Heating::Heating(Mode initialMode) : Component(initialMode) { }

Heating::Heating() : Component() { }

bool Heating::required(Clock& clock, float currentTemp) const {
    return Component::required(clock) && currentTemp < requestedTemperature;
}