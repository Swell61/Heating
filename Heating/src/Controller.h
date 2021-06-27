#ifndef _CONTROLLER_h
#define _CONTROLLER_h

#include "Display.h"
#include "WebInterface.h"
#include "Time/Timer.h"
#include <UIPEthernet.h>
#include "Components/ComponentControl.h"
#include "TempSensor.h"

/**
 * Main controller class for the system
 **/

class Controller {
    private:
        Config config; // MUST BE INITIALISED BEFORE websocketConnection
        EthernetUDP udpInterface;
        WebInterface websocketConnection;
        Clock clock;
        ComponentControl componentController;
        TempSensor localTempSensor;

    public:
        Controller(unsigned char boilerPin, unsigned char pumpPin, unsigned char oneWirePin);

        ComponentControl& getComponentControl();
        Clock& getClock();
};

#endif