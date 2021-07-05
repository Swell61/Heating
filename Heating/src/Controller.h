#ifndef _CONTROLLER_h
#define _CONTROLLER_h

#include "WebInterface.h"
#include <UIPEthernet.h>
#include "Components/ComponentControl.h"
#include "TempSensor.h"
#include "Request/Request.h"
#include "Display.h"
#include "Config/Config.h"

/**
 * Main controller class for the system
 **/
class Controller {
    private:
        IPAddress ip{192, 168, 1, 201}; // IP address of Arduino
	    byte mac[6] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; // MAC address of network interface

        WebInterface websocketConnection;
        Config config; // MUST BE INITIALISED BEFORE websocketConnection
        EthernetUDP udpInterface;
        Clock clock;
        ComponentControl componentController;
        TempSensor localTempSensor;
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

        ComponentControl& getComponentControl();
        Clock& getClock();
        TempSensor& getTempSensor();
};

#endif