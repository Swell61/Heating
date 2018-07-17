// 
// 
// 

#include "HeatingSystem.h"

HeatingSystem::HeatingSystem(int pumpPin, int boilerPin, int tempSensorPin) : pump(Pump(pumpPin)), boiler(Boiler(boilerPin)), tempSensor(TempSensor(tempSensorPin)), timer(Timer()), display(new Display()), remote(WebInterface()) {
	currentTemp = tempSensor.getTemp();
	setHeatingOff();
	setWaterOff();
	EthernetUDP udp;
	int NTPTryCount = 0;
	unsigned long time = 0;
	while (NTPTryCount < 10) {
		Serial.println("Trying to get time");
		time = ntpUnixTime(udp);
		if (time != 0) {
			Serial.println("Got time");
			Serial.println(time);
			int currentTime = (time / 60) % 1440;
			Serial.println(currentTime);
			timer.setMidnight((millis() / 60000) - currentTime);
			NTPTryCount = 10;
		}
		else {
			time++;
			delay(1000);
		}
	}

	display->mainDisplay(timer.getTime(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
	
};

void HeatingSystem::monitorSystem() { // This function runs through the process required to monitor and manage the heating system
	
	if (updateDisplay) {
		if (screen == 0) {
			display->displayUpdate(timer.getTime(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
			remote.processRemoteOutput(timer.getTime(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
		}
		else if (screen == 1) {
			display->timerUpdate(heatingMode == 1 ? true : false, waterMode == 1 ? true : false, timer.getHeatingOnMorning(), timer.getHeatingOffMorning(), timer.getHeatingOnAfternoon(), timer.getHeatingOffAfternoon(), timer.getWaterOnMorning(), timer.getWaterOffMorning(), timer.getWaterOnAfternoon(), timer.getWaterOffAfternoon());
			remote.processRemoteOutput(heatingMode == 1 ? true : false, waterMode == 1 ? true : false, timer.getHeatingOnMorning(), timer.getHeatingOffMorning(), timer.getHeatingOnAfternoon(), timer.getHeatingOffAfternoon(), timer.getWaterOnMorning(), timer.getWaterOffMorning(), timer.getWaterOnAfternoon(), timer.getWaterOffAfternoon());
					}
		else if (screen == 2) {
			display->updateEditTime(timer.getTime());
					}
		updateDisplay = false;
			}
	if (currentTemp != tempSensor.getTemp()) {
		currentTemp = tempSensor.getTemp();
		updateDisplay = true;
	};
	remoteOption = remote.processRemoteInput();
	touchOption = display->touchUpdate(screen);
	if (remoteOption == 255) {
		remote.processRemoteOutput(timer.getTime(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
		remote.processRemoteOutput(heatingMode == 1 ? true : false, waterMode == 1 ? true : false, timer.getHeatingOnMorning(), timer.getHeatingOffMorning(), timer.getHeatingOnAfternoon(), timer.getHeatingOffAfternoon(), timer.getWaterOnMorning(), timer.getWaterOffMorning(), timer.getWaterOnAfternoon(), timer.getWaterOffAfternoon());
	}
	else if (touchOption == 0) {
		touchOption = remoteOption;
	}
	
	if (touchOption == 1) { // User requested temperature up one degree
		if (requestedTemp < 25) {
			requestedTemp++; // Increase the requested temperature up by one
			updateDisplay = true; // The display needs updating
		}
	}
	else if (touchOption == 2) { // User requested temperature down one degree
		if (requestedTemp > 10) {
			requestedTemp--; // Decrease the requested temperature down by one
			updateDisplay = true; // The display needs updating
		}
	}
	else if (touchOption == 3 && !heatingBoostActive) { // User wants to turn the heating boost on
		updateDisplay = true; // Display needs updating
		boostHeating(true);
	}
	else if (touchOption == 3 && heatingBoostActive) { // User wants to turn the heating boost off
		updateDisplay = true; // Display needs updating
		boostHeating(false);
	}
	else if (touchOption == 4 && !waterBoostActive) { // User wants to turn the water boost on
		updateDisplay = true; // Display needs updating
		boostWater(true);
	}
	else if (touchOption == 4 && waterBoostActive) { // User wants to turn the water boost off
		updateDisplay = true; // Display needs updating
		boostWater(false);
	}
	else if (touchOption == 5) { // Go to timer display
		updateDisplay = true;
		screen = 1;
		display->timerDisplay(heatingMode == 1 ? true : false, waterMode == 1 ? true : false, timer.getHeatingOnMorning(), timer.getHeatingOffMorning(), timer.getHeatingOnAfternoon(), timer.getHeatingOffAfternoon(), timer.getWaterOnMorning(), timer.getWaterOffMorning(), timer.getWaterOnAfternoon(), timer.getWaterOffAfternoon());
	}
	else if (touchOption == 6) { // Go to main display
		updateDisplay = true;
		screen = 0;
		display->mainDisplay(timer.getTime(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
	}
	else if (touchOption == 7) {
		if (timer.setHeatingOnMorning(timer.getHeatingOnMorning() - timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 8) {
		if (timer.setHeatingOnMorning(timer.getHeatingOnMorning() + timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 9) {
		if (timer.setHeatingOffMorning(timer.getHeatingOffMorning() - timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 10) {
		if (timer.setHeatingOffMorning(timer.getHeatingOffMorning() + timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 11) {
		if (timer.setHeatingOnAfternoon(timer.getHeatingOnAfternoon() - timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 12) {
		if (timer.setHeatingOnAfternoon(timer.getHeatingOnAfternoon() + timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 13) {
		if (timer.setHeatingOffAfternoon(timer.getHeatingOffAfternoon() - timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 14) {
		if (timer.setHeatingOffAfternoon(timer.getHeatingOffAfternoon() + timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 15) {
		if (timer.setWaterOnMorning(timer.getWaterOnMorning() - timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 16) {
		if (timer.setWaterOnMorning(timer.getWaterOnMorning() + timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 17) {
		if (timer.setWaterOffMorning(timer.getWaterOffMorning() - timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 18) {
		if (timer.setWaterOffMorning(timer.getWaterOffMorning() + timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 19) {
		if (timer.setWaterOnAfternoon(timer.getWaterOnAfternoon() - timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 20) {
		if (timer.setWaterOnAfternoon(timer.getWaterOnAfternoon() + timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 21) {
		if (timer.setWaterOffAfternoon(timer.getWaterOffAfternoon() - timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 22) {
		if (timer.setWaterOffAfternoon(timer.getWaterOffAfternoon() + timerTimeInc)) {
			updateDisplay = true;
		}
	}
	else if (touchOption == 23) {
		timer.setHeatingTimerState(!timer.getHeatingTimerStatus());
				updateDisplay = true;
	}
	else if (touchOption == 24) {
		timer.setWaterTimerState(!timer.getWaterTimerStatus());
		updateDisplay = true;
	}
	else if (touchOption == 25) {
		heatingMode++;
		if (heatingMode > 2) {
			heatingMode = 0;
		}
				updateDisplay = true;
	}
	else if (touchOption == 26) {
		waterMode++;
		if (waterMode > 2) {
			waterMode = 0;
		}
		updateDisplay = true;
	}
	else if (touchOption == 27) {
		screen = 2; // Change to edit time screen
				display->editTime(timer.getTime());
		updateDisplay = true;
	}
	else if (touchOption == 28) {
		
		if (timer.setMidnight(timer.getMidnight() - 600)) { // Increase current time by 10 hours
						updateDisplay = true;
		}
	}
	else if (touchOption == 29) {
				if (timer.setMidnight(timer.getMidnight() + 600)) { // Decrease current time by 10 hours
			updateDisplay = true;
		}
	}
	else if (touchOption == 30) {
				if (timer.setMidnight(timer.getMidnight() - 60)) { // Increase current time by 1 hour
			updateDisplay = true;
		}
	}
	else if (touchOption == 31) {
				if (timer.setMidnight(timer.getMidnight() + 60)) { // Decrease current time by 1 hour
			updateDisplay = true;
		}
	}
	else if (touchOption == 32) {
				if (timer.setMidnight(timer.getMidnight() - 10)) { // Increase current time by 10 minutes
			updateDisplay = true;
		}
	}
	else if (touchOption == 33) {
				if (timer.setMidnight(timer.getMidnight() + 10)) { // Decrease current time by 10 hours
			updateDisplay = true;
		}
	}
	else if (touchOption == 34) {
		
		if (timer.setMidnight(timer.getMidnight() - 1)) { // Increase current time by 1 minute
			updateDisplay = true;
					}
	}
	else if (touchOption == 35) {
		
		if (timer.setMidnight(timer.getMidnight() + 1)) { // Decrease current time by 1 minute
			updateDisplay = true;
					}
	}
	checkBoosts(); // Run through the boost timers, checking if they need altering

	changeRelayStates();

	if ((millis() - lastTimeUpdate) >= 60000) {
		updateDisplay = true;
		lastTimeUpdate = millis();
	}
};

void HeatingSystem::checkBoosts() { // Boosts have priority over everything
	if (heatingBoostActive && ((millis() - startTimeHeatingBoost) >= (boostLengthHeating * 60000))) {
		boostHeating(false);
	}
	else if (waterBoostActive && ((millis() - startTimeWaterBoost) >= (boostLengthWater * 60000))) {
		boostWater(false);
	}
};

void HeatingSystem::changeRelayStates() {

	// Heating settings
	if (heatingBoostActive) { // If heating boost is active...
		enableHeating(); // ...turn the heating on
	}
	else if (temperatureCheck() && (heatingMode == 2 || (heatingMode == 1 && timer.getHeatingTimerStatus()))) { // if ((Heating is ON OR (Heating Timer is ON and ACTIVE)) AND temperature is too low)
		enableHeating();
	}
	else {
		disableHeating();
	}

	// Water settings
	if (waterBoostActive) { // If water boost is active...
		enableWater(); // ...turn the water on
	}
	else if (waterMode == 2 || (waterMode == 1 && timer.getWaterTimerStatus())) { // if (Heating is ON OR (Water Timer is ON and ACTIVE))
		enableWater();
	}
	else {
		disableWater();
	}


	if ((lastSystemMode != 0) && heatingStatus && waterStatus) {
		setHeatingOn();
		setWaterOn();
		lastSystemMode = 0;
		updateDisplay = true;
	}
	else if ((lastSystemMode != 1) && heatingStatus && !waterStatus) {
		setHeatingOn();
		lastSystemMode = 1;
		updateDisplay = true;
	}
	else if ((lastSystemMode != 2) && !heatingStatus && waterStatus) {
		setWaterWithoutHeating();
		lastSystemMode = 2;
		updateDisplay = true;
	}
	else if ((lastSystemMode != 3) && !heatingStatus && !waterStatus) {
		setHeatingOff();
		setWaterOff();
		lastSystemMode = 3;
		updateDisplay = true;
	}

}


bool HeatingSystem::temperatureCheck() {
	if (tempSensor.getTemp() < (requestedTemp - maxDrift)) { // If the temperature of the zone has gone below the required temperature...
		return true;
	}
	else if (tempSensor.getTemp() >= requestedTemp) { // If the temperature of the zone has reached the requried temperature...
		return false;
	}
}


void HeatingSystem::setHeatingOn() { // Function for turning on the heating
	boiler.enable(); // Turn the boiler on
	pump.enable(); // Turn the pump on
	};
void HeatingSystem::setHeatingOff() { // Function for turning off the heating
	boiler.disable(); // Turn the boiler off
	pump.disable(); // TUrn the pump off
	};

void HeatingSystem::setWaterOn() { // Function for turning the hot water on
	boiler.enable(); // Turn the boiler on
	}
void HeatingSystem::setWaterOff() { // Function for turning the hot water off
	boiler.disable(); // Turn the boiler off
	};
void HeatingSystem::setWaterWithoutHeating() {
	boiler.enable();
	pump.disable();
	}
void HeatingSystem::enableHeating() {
	heatingStatus = true;
}
void HeatingSystem::disableHeating() {
	heatingStatus = false;
}
void HeatingSystem::enableWater() {
	waterStatus = true;
}
void HeatingSystem::disableWater() {
	waterStatus = false;
}


void HeatingSystem::boostHeating(bool state) { // Function for the heating boost
	if (state == true) { // If boost is being turned on...
		enableHeating(); // ...turn the heating on
		heatingBoostActive = true;
		startTimeHeatingBoost = millis();
	}
	else if (state == false) { // Else if the boost is being turned off...
		disableHeating();
		heatingBoostActive = false;
		updateDisplay = true;
	}
};

void HeatingSystem::boostWater(bool state) {
	if (state == true) {
		enableWater();
		waterBoostActive = true;
		startTimeWaterBoost = millis();
	}
	else if (state == false) {
		disableWater();
		waterBoostActive = false;
		updateDisplay = true;
	}
};

bool HeatingSystem::getHeatingStatus() {
	return (pump.getStatus() && boiler.getStatus());
};
bool HeatingSystem::getWaterStatus() {
	return boiler.getStatus(); // In our system, if the heating is on then the hot water is also on
};



void HeatingSystem::setTemp(int temp) {
	requestedTemp = temp;
}

unsigned long inline HeatingSystem::ntpUnixTime(UIPUDP &udp)
{
	static int udpInited = udp.begin(123); // open socket on arbitrary port

	const char timeServer[] = "pool.ntp.org";  // NTP server

											   // Only the first four bytes of an outgoing NTP packet need to be set
											   // appropriately, the rest can be whatever.
	const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

											   // Fail if WiFiUdp.begin() could not init a socket
	if (!udpInited)
		return 0;

	// Clear received data from possible stray received packets
	udp.flush();

	// Send an NTP request
	if (!(udp.beginPacket(timeServer, 123) // 123 is the NTP port
		&& udp.write((byte *)&ntpFirstFourBytes, 48) == 48
		&& udp.endPacket()))
		return 0;				// sending request failed

								// Wait for response; check every pollIntv ms up to maxPoll times
	const int pollIntv = 150;		// poll every this many ms
	const byte maxPoll = 15;		// poll up to this many times
	int pktLen;				// received packet length
	for (byte i = 0; i<maxPoll; i++) {
		if ((pktLen = udp.parsePacket()) == 48)
			break;
		delay(pollIntv);
	}
	if (pktLen != 48)
		return 0;				// no correct packet received

								// Read and discard the first useless bytes
								// Set useless to 32 for speed; set to 40 for accuracy.
	const byte useless = 40;
	for (byte i = 0; i < useless; ++i)
		udp.read();

	// Read the integer part of sending time
	unsigned long time = udp.read();	// NTP time
	for (byte i = 1; i < 4; i++)
		time = time << 8 | udp.read();

	// Round to the nearest second if we want accuracy
	// The fractionary part is the next byte divided by 256: if it is
	// greater than 500ms we round to the next second; we also account
	// for an assumed network delay of 50ms, and (0.5-0.05)*256=115;
	// additionally, we account for how much we delayed reading the packet
	// since its arrival, which we assume on average to be pollIntv/2.
	time += (udp.read() > 115 - pollIntv / 8);

	// Discard the rest of the packet
	udp.flush();
	time = time - 2208988800ul; // Convert to unix time


	return time;
}
