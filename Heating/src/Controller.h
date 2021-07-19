#ifndef _CONTROLLER_h
#define _CONTROLLER_h

#include "WebInterface.h"
#include <EthernetENC.h>
#include <EthernetUdp.h>
#include "Request/Request.h"
#include "Config/Config.h"
#include "Touchscreen/Display.h"
#include "Touchscreen/LoadingDisplay.h"
#include "CoreComponents.h"

/**
 * Main controller class for the system
 **/
class Controller {
    private:
        IPAddress ip{192, 168, 1, 201}; // IP address of Arduino
	    byte mac[6] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; // MAC address of network interface

        Config config; // MUST BE INITIALISED BEFORE websocketConnection
        EthernetUDP udpInterface;
        WebInterface websocketConnection;
        
        CoreComponents coreComponents;

        Request request;
        Display display;
        unsigned long lastSystemUpdateCheck = 0;
        unsigned long lastNetworkStatusCheck = 0;
        bool systemCheckIntervalMet();
        bool networkStatusIntervalMet();
        bool intervalMet(unsigned long& lastCheck, unsigned short int intervalLength);
        bool periodicUpdate();
        void setupWatchdog();

        SystemFunction getRequest();

        static const unsigned short int ONE_SECOND_IN_MILLISECONDS = 1000;
        static const unsigned short int TEN_SECONDS_IN_MILLISECONDS = 10000;

    public:
        Controller(unsigned char boilerPin, unsigned char pumpPin, unsigned char oneWirePin);

        void loop();

        CoreComponents& getCoreComponents();
        ComponentControl& getComponentControl();
        Clock& getClock();
        TempSensor& getTempSensor();
};

#endif