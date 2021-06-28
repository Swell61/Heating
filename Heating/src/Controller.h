#ifndef _CONTROLLER_h
#define _CONTROLLER_h

#include "WebInterface.h"
#include "Time/Timer.h"
#include <UIPEthernet.h>
#include "Components/ComponentControl.h"
#include "TempSensor.h"
#include "Request/Request.h"
#include "Display.h"

/**
 * Main controller class for the system
 **/
class Controller {
    private:
        WebInterface websocketConnection;
        Config config; // MUST BE INITIALISED BEFORE websocketConnection
        EthernetUDP udpInterface;
        Clock clock;
        ComponentControl componentController;
        TempSensor localTempSensor;
        Request request;
        //Display display;
        unsigned long lastCheck = 0;
        bool intervalMet();
        
        SystemFunction getRequest();

        static const unsigned short int ONE_SECOND_IN_MILLISECONDS = 1000;

    public:
        Controller(unsigned char boilerPin, unsigned char pumpPin, unsigned char oneWirePin);

        void loop();

        ComponentControl& getComponentControl();
        Clock& getClock();
        TempSensor& getTempSensor();
};

#endif