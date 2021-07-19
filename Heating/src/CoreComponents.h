#ifndef _CORECOMPONENTS_h
#define _CORECOMPONENTS_h

#include "Components/ComponentControl.h"
#include "TempSensor.h"
#include "Time/Clock.h"

class CoreComponents {
    private:
        Clock clock;
        ComponentControl componentController;
        TempSensor localTempSensor;
    public:
        CoreComponents(unsigned char boilerPin, unsigned char pumpPin, unsigned char oneWirePin, Config& config);

        Clock& getClock();
        ComponentControl& getComponentControl();
        TempSensor& getTempSensor();

        bool update();
};

#endif