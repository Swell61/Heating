#include "Controller.h"

Controller::Controller(unsigned char boilerPin, unsigned char pumpPin, unsigned char oneWirePin) : config(Config("config.txt", 5)), 
    componentController(ComponentControl(boilerPin, pumpPin, config)), localTempSensor(TempSensor(A4, 0x28, 0xFF, 0x60, 0x9A, 0xA1, 0x17, 0x5, 0x43))
    , websocketConnection(WebInterface(config.available())) 
   {
        Ethernet.begin(mac, ip);
	    Serial.println("Ethernet");

        for (unsigned char ntpTryCount = 0; ntpTryCount < 3; ++ntpTryCount) {
            Serial.println("NTP");
            if (clock.synchroniseWithNtp(udpInterface)) {
                break;
            }
        }

        websocketConnection.begin();
}

bool Controller::periodicUpdate() {
    bool updateOccurred = false;
    
    if (systemCheckIntervalMet()) {
        updateOccurred |= componentController.update(localTempSensor.getTemp(), clock);
    }

    if (networkStatusIntervalMet()) {
        if (Ethernet.linkStatus() == LinkOFF || Ethernet.linkStatus() == Unknown) { // If ethernet becomes disconnected
			Ethernet.begin(mac, ip);
		}
        else {
            Ethernet.maintain();
        }
    }
}

void Controller::loop() {
    SystemFunction currentRequest = getRequest();
    bool updateOccurred = false;
    if (currentRequest != SystemFunction::NONE) {
        updateOccurred |= request.execute(currentRequest, *this);
    }

    if (systemCheckIntervalMet()) {
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

bool Controller::systemCheckIntervalMet() {
    return intervalMet(lastSystemUpdateCheck, ONE_SECOND_IN_MILLISECONDS);
}

bool Controller::networkStatusIntervalMet() {
    return intervalMet(lastNetworkStatusCheck, TEN_SECONDS_IN_MILLISECONDS);
}

bool Controller::intervalMet(unsigned long& lastCheck, unsigned short int intervalLength) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastCheck > intervalLength) {
        lastCheck = currentMillis;
        return true;
    }
    return false;
}

SystemFunction Controller::getRequest() {
    unsigned char command = websocketConnection.processRemoteInput();
    return Request::v1CommandToFunction(command);
}