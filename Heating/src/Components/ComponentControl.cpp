#include "ComponentControl.h"

ComponentControl::ComponentControl(unsigned char boilerPin, unsigned char pumpPin) : boiler(boilerPin), pump(pumpPin), heating(Heating(0.5)) { }

bool ComponentControl::update(float currentTemp, Clock& clock) {
    if (heating.required(clock, currentTemp)) { // Enabling heating also enables hot water in our system
        return setHeatingAndWater();
    }
    if (water.required(clock)) {
        return setWaterWithoutHeating();
    }
    return setOff(); // Disabling heating also disables water in our system
}

bool ComponentControl::setHeatingAndWater() { 
    if (mode == SystemMode::HEATING_AND_WATER) {
        return false;
    }
	boiler.enable();
	pump.enable();
    mode = SystemMode::HEATING_AND_WATER;
    return true;;
};
bool ComponentControl::setOff() {
    if (mode == SystemMode::OFF) {
        return false;
    }
	boiler.disable();
	pump.disable();

    mode = SystemMode::OFF;
    return true;
};

bool ComponentControl::setWaterWithoutHeating() {
    if (mode == SystemMode::WATER) {
        return false;
    }
	boiler.enable();
	pump.disable();

    mode = SystemMode::WATER;
    return true;
}

Heating& ComponentControl::getHeating() {
    return heating;
}

Water& ComponentControl::getWater() {
    return water;
}