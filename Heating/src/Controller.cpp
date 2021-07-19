#include "Controller.h"

Controller::Controller(unsigned char boilerPin, unsigned char pumpPin, unsigned char oneWirePin) : config(Config("config.txt", 5)), coreComponents(boilerPin, pumpPin, oneWirePin, config), websocketConnection(WebInterface(config.available())) 
   {
    Ethernet.init(12);
    Ethernet.begin(mac, ip);
    LoadingDisplay loadingScreen;
    unsigned char NTP_RETRIES = 3;
    loadingScreen.display(config, NTP_RETRIES, display.getDisplay());
    for (unsigned char ntpTryCount = 0; ntpTryCount < NTP_RETRIES; ++ntpTryCount) {
        loadingScreen.update(ntpTryCount + 1, display.getDisplay());
        if (coreComponents.getClock().synchroniseWithNtp(udpInterface)) {
            break;
        }
        //wdt_reset();
    }
    websocketConnection.begin();
    Serial.println("Done");
    display.update(coreComponents, SystemFunction::MAIN_DISPLAY);
}

void Controller::setupWatchdog() {
}

bool Controller::periodicUpdate() {
    bool updateOccurred = false;
    
    if (systemCheckIntervalMet()) {
        updateOccurred |= coreComponents.update();
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
    wdt_reset();
    SystemFunction currentRequest = getRequest();
    bool updateOccurred = false;
    if (currentRequest != SystemFunction::NONE) {
        updateOccurred |= request.execute(currentRequest, *this);
    }

    updateOccurred |= periodicUpdate();

    if (updateOccurred) {
        sendUpdatesToClients(currentRequest);    
    }

}

void Controller::sendUpdatesToClients(SystemFunction updateType) {
    websocketConnection.processRemoteOutput(*this);
    display.update(coreComponents, updateType);
}

ComponentControl& Controller::getComponentControl() {
    return coreComponents.getComponentControl();
}

Clock& Controller::getClock() {
    return coreComponents.getClock();
}

TempSensor& Controller::getTempSensor() {
    return coreComponents.getTempSensor();
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