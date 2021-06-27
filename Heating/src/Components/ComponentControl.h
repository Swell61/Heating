#ifndef _COMPONENTCONTROL_h
#define _COMPONENTCONTROL_h

#include "Pump.h"
#include "Boiler.h"
#include "Heating.h"
#include "Water.h"
#include "../Request/Request.h"
#include "../Time/Timer.h"
#include "../Time/ComponentTimer.h"
#include "../Enums/State.h"
#include "../TempSensor.h"
#include "../Enums/SystemMode.h"

class ComponentControl {
    private:
        Pump pump;
        Boiler boiler;

        Heating heating;
        Water water;

        SystemMode mode = SystemMode::OFF;

        bool setOff(); // Function for setting state of hot water components to off
        bool setHeatingAndWater(); // Function for setting the state of heating components to on
        bool setWaterWithoutHeating();

    public:
        ComponentControl(unsigned char boilerPin, unsigned char pumpPin);

        bool update(float currentTemp, Clock& timer);

        Heating& getHeating();
        Water& getWater();
};

#endif