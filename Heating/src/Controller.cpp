#include "Controller.h"

Controller::Controller(unsigned char boilerPin, unsigned char pumpPin, unsigned char oneWirePin) : config(Config("config.txt", 5)), 
    componentController(ComponentControl(boilerPin, pumpPin)), localTempSensor(TempSensor(A4, 0x28, 0xFF, 0x60, 0x9A, 0xA1, 0x17, 0x5, 0x43))
    , websocketConnection(WebInterface(config.available())) 
   {
        IPAddress ip{192, 168, 1, 201}; // IP address of Arduino
	    byte mac[6] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; // MAC address of network interface
        Ethernet.begin(mac, ip);
	    Serial.println("Ethernet");
	    ip = Ethernet.localIP();
	    // start listening for clients
	    
        
        for (unsigned char ntpTryCount = 0; ntpTryCount < 3; ++ntpTryCount) {
            Serial.println("NTP");
            if (clock.synchroniseWithNtp(udpInterface)) {
                break;
            }
        }

        websocketConnection.begin();
}

void Controller::loop() {
    SystemFunction currentRequest = getRequest();
    bool updateOccurred = false;
    if (currentRequest != SystemFunction::NONE) {
        updateOccurred |= request.execute(currentRequest, *this);
    }

    if (intervalMet()) {
        updateOccurred |= componentController.update(localTempSensor.getTemp(), clock);
    }

    if (updateOccurred) {
        websocketConnection.processRemoteOutput(*this);
    }

}

ComponentControl& Controller::getComponentControl() {
    return componentController;
}

Clock& Controller::getClock() {
    return clock;
}

TempSensor& Controller::getTempSensor() {
    return localTempSensor;
}

bool Controller::intervalMet() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastCheck > ONE_SECOND_IN_MILLISECONDS) {
        lastCheck = currentMillis;
        return true;
    }
    return false;
}

SystemFunction Controller::getRequest() {
    unsigned char command = websocketConnection.processRemoteInput();
    return Request::v1CommandToFunction(command);
}