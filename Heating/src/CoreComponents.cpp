#include "CoreComponents.h"

CoreComponents::CoreComponents(unsigned char boilerPin, unsigned char pumpPin, unsigned char oneWirePin, Config& config) : componentController(ComponentControl(boilerPin, pumpPin, config)), 
    localTempSensor(TempSensor(oneWirePin, 0x28, 0xFF, 0x60, 0x9A, 0xA1, 0x17, 0x5, 0x43)) { }

Clock& CoreComponents::getClock() {
    return clock;
}

ComponentControl& CoreComponents::getComponentControl() {
    return componentController;
}

TempSensor& CoreComponents::getTempSensor() {
    return localTempSensor;
}

bool CoreComponents::update() {
    return componentController.update(localTempSensor, clock);
}