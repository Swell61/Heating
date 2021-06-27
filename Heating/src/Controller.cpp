#include "Controller.h"

Controller::Controller(unsigned char boilerPin, unsigned char pumpPin, unsigned char oneWirePin) : config(Config("config.txt", 5)), websocketConnection(config.available()),
    componentController(ComponentControl(boilerPin, pumpPin)), localTempSensor(TempSensor(A4, 0x28, 0xFF, 0x60, 0x9A, 0xA1, 0x17, 0x5, 0x43)) {
        
}