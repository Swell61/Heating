// 
// 
// 

#include "HeatingSystem.h"

HeatingSystem::HeatingSystem(int pumpPin, int boilerPin, int tempSensorPin) : pump(Pump(pumpPin)), boiler(Boiler(boilerPin)), tempSensor(TempSensor(tempSensorPin)), timer(Timer()), display(new Display()), remote(WebInterface(SD.begin(49))), config(Config("config.txt")) { // Constructor. Initialises the componenets of the heating system
	currentTemp = tempSensor.getTemp(); // Gets the current temperature to display
	setHeatingOff(); // Disables the heating
	setWaterOff(); // Disables the ho twater
	Serial.println((SDAvailable = SD.begin(49)) ? "SD UP" : "SD DOWN"); // Checks whether the SD card is available or not
	if (SDAvailable) {
		loadTimer();
	}
	int NTPTryCount = 0; // Variable to store the number of tries to get the time from the NTP server
	unsigned long time = 0; // Variable to store the time from the NTP server
	while (NTPTryCount < 5) { // Tries 5 times to get the time
		
		display->loadingScreen(SDAvailable, NTPTryCount + 1); // Shows the loading screen
		time = timer.getNTPTime(udp); // Tries to get the time
		if (time != 0) { // If the time is not zero (very unlikely it will be zero and if it is, it will run another check if it has 1+ remaining)
			int currentTime = (time / 60) % 1440; // Gets the current time in minutes
			
			timer.setMidnight((millis() / 60000) - currentTime); // Sets midnight
			NTPTryCount = 5; // Exceeds the max tries so the loop will break
		}
		else { // If failed to get time...
			NTPTryCount++; // ...increase the try count and loop round again if 1+ tries left
		}
	}

	display->mainDisplay(timer.getTime(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive); // Show the main display
	
	//setupWatchdog();
};

void HeatingSystem::setupWatchdog() {
	cli(); // disable all interrupts
	wdt_reset(); // reset the WDT timer
				 
	WDTCSR |= (1 << WDCE) | (1 << WDE); // Enter watchdog config mode
	// Set Watchdog settings:
	WDTCSR = (0 << WDIE) | (1 << WDE) | (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0);
	sei();
}

void HeatingSystem::loadTimer() {
	// Load timer
	int time;
	if ((time = atoi(config.readProperty("heatMon"))) != 0) {
		timer.setHeatingOnMorning(time);
	}
	if ((time = atoi(config.readProperty("heatMoff"))) != 0) {
		timer.setHeatingOffMorning(time);
	}
	if ((time = atoi(config.readProperty("heatAon"))) != 0) {
		timer.setHeatingOnAfternoon(time);
	}
	if ((time = atoi(config.readProperty("heatAoff"))) != 0) {
		timer.setHeatingOffAfternoon(time);
	}
	
	if ((time = atoi(config.readProperty("waterMon"))) != 0) {
		timer.setWaterOnMorning(time);
	}
	if ((time = atoi(config.readProperty("waterMoff"))) != 0) {
		timer.setWaterOffMorning(time);
	}
	if ((time = atoi(config.readProperty("waterAon"))) != 0) {
		timer.setWaterOnAfternoon(time);
	}
	if ((time = atoi(config.readProperty("waterAoff"))) != 0) {
		timer.setWaterOffAfternoon(time);
	}
}

bool HeatingSystem::saveTimer(const char* timerCase, int time) {
	if (SDAvailable && time < 1440 && strlen(timerCase) >= 7 && strlen(timerCase) <= 9) { // If SD card available and time is in correct range and timercase is valid range
		char buffer[5]; // Buffer for int to char array conversion
		itoa(time, buffer, 10); // Convert time to char array
		config.writeProperty(timerCase, buffer); // Write the proeprty to the file
		return true; // Success
	}
	return false; // Failed
}

void HeatingSystem::monitorSystem() { // This function runs through the process required to monitor and manage the heating system
	wdt_reset(); // Reset the timer
	if (updateDisplay) { // If the screen needs updating
		if (screen == 0) { // If on main display
			display->displayUpdate(timer.getTime(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive); // Show the main display
			// Update any connected clients with the current status
			remote.processRemoteOutput(timer.getTime(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
			remote.processRemoteOutput(heatingMode == 1 ? true : false, waterMode == 1 ? true : false, timer.getHeatingOnMorning(), timer.getHeatingOffMorning(), timer.getHeatingOnAfternoon(), timer.getHeatingOffAfternoon(), timer.getWaterOnMorning(), timer.getWaterOffMorning(), timer.getWaterOnAfternoon(), timer.getWaterOffAfternoon());

		}
		else if (screen == 1) { // If on the timer display
			display->timerUpdate(heatingMode == 1 ? true : false, waterMode == 1 ? true : false, timer.getHeatingOnMorning(), timer.getHeatingOffMorning(), timer.getHeatingOnAfternoon(), timer.getHeatingOffAfternoon(), timer.getWaterOnMorning(), timer.getWaterOffMorning(), timer.getWaterOnAfternoon(), timer.getWaterOffAfternoon()); // Show the timer display
			// Update any connected clients with the current status
			remote.processRemoteOutput(heatingMode == 1 ? true : false, waterMode == 1 ? true : false, timer.getHeatingOnMorning(), timer.getHeatingOffMorning(), timer.getHeatingOnAfternoon(), timer.getHeatingOffAfternoon(), timer.getWaterOnMorning(), timer.getWaterOffMorning(), timer.getWaterOnAfternoon(), timer.getWaterOffAfternoon());
			remote.processRemoteOutput(timer.getTime(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
			
		}
		else if (screen == 2) { // If on the time edit display
			display->updateEditTime(timer.getTime()); // Show the time edit display
					}
		updateDisplay = false; // Updating screen has completed
			}
	if (abs(currentTemp - tempSensor.getTemp()) >= minTempDifference) { // Run temperature check for currently displayed temperature
		currentTemp = tempSensor.getTemp(); // If not correct, get corret temperature and...
		updateDisplay = true; // ...update the display
	};
	remoteOption = remote.processRemoteInput(); // Process any clients
	touchOption = display->touchUpdate(screen); // Get the current requested touchscreen option
	if (remoteOption == 255) { // If a new client has just joined
		// Send current system status
		remote.processRemoteOutput(timer.getTime(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
		remote.processRemoteOutput(heatingMode == 1 ? true : false, waterMode == 1 ? true : false, timer.getHeatingOnMorning(), timer.getHeatingOffMorning(), timer.getHeatingOnAfternoon(), timer.getHeatingOffAfternoon(), timer.getWaterOnMorning(), timer.getWaterOffMorning(), timer.getWaterOnAfternoon(), timer.getWaterOffAfternoon());
	}
	else if (touchOption == 0) { // Touschreen takes priority. If no touch option, check remote requests
		touchOption = remoteOption;
	}
	wdt_reset(); // Reset the timer
	if (touchOption == 1) { // User requested temperature up one degree
		if (requestedTemp < 28) {
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
	else if (touchOption == 7) { // Change heating on morning time 
		if (timer.setHeatingOnMorning(timer.getHeatingOnMorning() - timerTimeInc)) {
			updateDisplay = true;
			saveTimer("heatMon", timer.getHeatingOnMorning());
		}
	}
	else if (touchOption == 8) { // Change heating on morning time 
		if (timer.setHeatingOnMorning(timer.getHeatingOnMorning() + timerTimeInc)) {
			updateDisplay = true;
			saveTimer("heatMon", timer.getHeatingOnMorning());
		}
	}
	else if (touchOption == 9) { // Change heating off morning time 
		if (timer.setHeatingOffMorning(timer.getHeatingOffMorning() - timerTimeInc)) {
			updateDisplay = true;
			saveTimer("heatMoff", timer.getHeatingOffMorning());
		}
	}
	else if (touchOption == 10) { // Change heating off morning time
		if (timer.setHeatingOffMorning(timer.getHeatingOffMorning() + timerTimeInc)) {
			updateDisplay = true;
			saveTimer("heatMoff", timer.getHeatingOffMorning());
		}
	}
	else if (touchOption == 11) { // Change heating on afternoon time
		if (timer.setHeatingOnAfternoon(timer.getHeatingOnAfternoon() - timerTimeInc)) {
			updateDisplay = true;
			saveTimer("heatAon", timer.getHeatingOnAfternoon());
		}
	}
	else if (touchOption == 12) { // Change heating on afternoon time
		if (timer.setHeatingOnAfternoon(timer.getHeatingOnAfternoon() + timerTimeInc)) {
			updateDisplay = true;
			saveTimer("heatAon", timer.getHeatingOnAfternoon());
		}
	}
	else if (touchOption == 13) { // Change heating off afternoon time
		if (timer.setHeatingOffAfternoon(timer.getHeatingOffAfternoon() - timerTimeInc)) {
			updateDisplay = true;
			saveTimer("heatAoff", timer.getHeatingOffAfternoon());
		}
	}
	else if (touchOption == 14) { // Change heating off afternoon time
		if (timer.setHeatingOffAfternoon(timer.getHeatingOffAfternoon() + timerTimeInc)) {
			updateDisplay = true;
			saveTimer("heatAoff", timer.getHeatingOffAfternoon());
		}
	}
	else if (touchOption == 15) { // Change hot water on morning time
		if (timer.setWaterOnMorning(timer.getWaterOnMorning() - timerTimeInc)) {
			updateDisplay = true;
			saveTimer("waterMon", timer.getWaterOnMorning());
		}
	}
	else if (touchOption == 16) { // Change hot water on morning time
		if (timer.setWaterOnMorning(timer.getWaterOnMorning() + timerTimeInc)) {
			updateDisplay = true;
			saveTimer("waterMon", timer.getWaterOnMorning());
		}
	}
	else if (touchOption == 17) { // Change hot water off morning time
		if (timer.setWaterOffMorning(timer.getWaterOffMorning() - timerTimeInc)) {
			updateDisplay = true;
			saveTimer("waterMoff", timer.getWaterOffMorning());
		}
	}
	else if (touchOption == 18) { // Change hot water off morning time
		if (timer.setWaterOffMorning(timer.getWaterOffMorning() + timerTimeInc)) {
			updateDisplay = true;
			saveTimer("waterMoff", timer.getWaterOffMorning());
		}
	}
	else if (touchOption == 19) { // Change hot water on afternoon time
		if (timer.setWaterOnAfternoon(timer.getWaterOnAfternoon() - timerTimeInc)) {
			updateDisplay = true;
			saveTimer("waterAon", timer.getWaterOnAfternoon());
		}
	}
	else if (touchOption == 20) { // Change hot water on afternoon time
		if (timer.setWaterOnAfternoon(timer.getWaterOnAfternoon() + timerTimeInc)) {
			updateDisplay = true;
			saveTimer("waterAon", timer.getWaterOnAfternoon());
		}
	}
	else if (touchOption == 21) { // Change hot water off afternoon time
		if (timer.setWaterOffAfternoon(timer.getWaterOffAfternoon() - timerTimeInc)) {
			updateDisplay = true;
			saveTimer("waterAoff", timer.getWaterOffAfternoon());
		}
	}
	else if (touchOption == 22) { // Change hot water off afternoon time
		if (timer.setWaterOffAfternoon(timer.getWaterOffAfternoon() + timerTimeInc)) {
			updateDisplay = true;
			saveTimer("waterAoff", timer.getWaterOffAfternoon());
		}
	}
	else if (touchOption == 23) { // Change heating timer state
		timer.setHeatingTimerState(!timer.getHeatingTimerStatus());
		updateDisplay = true;
	}
	else if (touchOption == 24) { // Change hot water timer state
		timer.setWaterTimerState(!timer.getWaterTimerStatus());
		updateDisplay = true;
	}
	else if (touchOption == 25) { // Change heating mode
		heatingMode++;
		if (heatingMode > 2) {
			heatingMode = 0;
		}
		updateDisplay = true;
	}
	else if (touchOption == 26) { // Change hot water mode
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

	changeRelayStates(); // make any required changed to the relays

	if ((millis() - lastTimeUpdate) >= 60000) { // Update the display once every minute to update the time
		updateDisplay = true; // Update the display
		//timer.checkMidnight(udp); // Check if its midnight (update midnight and check that the time is correct
		timer.checkMidnight();
		lastTimeUpdate = millis(); // Update the lsat time the time was updated
	}

	if ((millis() - lastHourlyUpdate) >= 3600000) { // Hourly updates
		//timer.setMidnightNTP(udp); // Try to update the time every hour
	}
	wdt_reset(); // Reset timer
};

void HeatingSystem::checkBoosts() { // Boosts have priority over everything
	if (heatingBoostActive && ((millis() - startTimeHeatingBoost) >= (boostLengthHeating * 60000))) { // If heating boost is active and boost timer is over, turn off the boost
		boostHeating(false);
	}
	else if (waterBoostActive && ((millis() - startTimeWaterBoost) >= (boostLengthWater * 60000))) { // Same as heating but for hot water. If heating boost is on, hot water doesn't need adjusting because it will be on anyway
		boostWater(false);
	}
};

void HeatingSystem::changeRelayStates() { // Function checks current system status and decides whether the heating and hot water need to be on or not

	// Heating settings
	if (heatingBoostActive) { // If heating boost is active...
		enableHeating(); // ...turn the heating on
	}
	else if (temperatureCheck() == 1 && (heatingMode == 2 || (heatingMode == 1 && timer.getHeatingTimerStatus()))) { // if ((Heating is ON OR (Heating Timer is ON and ACTIVE)) AND temperature is too low)
		enableHeating();
	}
	else if (heatingMode == 0 || (temperatureCheck() == 0 && (heatingMode == 2 || (heatingMode == 1 && timer.getHeatingTimerStatus()))) || (heatingMode == 1 && !timer.getHeatingTimerStatus())) {
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

	// lastSystemMode will stop the system trying to enter a state it is already in
	if ((lastSystemMode != 0) && heatingStatus && waterStatus) { // If the heating and hot water should be on
		// Turn them on
		setHeatingOn();
		setWaterOn();
		lastSystemMode = 0;
		updateDisplay = true; // Update the display
	}
	else if ((lastSystemMode != 1) && heatingStatus && !waterStatus) { // If the heating should be on but not the hot water
		setHeatingOn(); // Turn the heating on
		lastSystemMode = 1;
		updateDisplay = true; // Update the display
	}
	else if ((lastSystemMode != 2) && !heatingStatus && waterStatus) { // If the hot water should be on but bot the heating
		setWaterWithoutHeating(); // Turn the hot water components on and disable the unneeded heating components
		lastSystemMode = 2;
		updateDisplay = true; // Update the display
	}
	else if ((lastSystemMode != 3) && !heatingStatus && !waterStatus) { // If heating and hot water should be off
		setHeatingOff(); // Turn the heating off
		setWaterOff(); // Turn the hot water off
		lastSystemMode = 3;
		updateDisplay = true; // Update the display
	}

}


byte HeatingSystem::temperatureCheck() { // Function for checking the temperature
	if (tempSensor.getTemp() <= (requestedTemp - maxDrift)) { // If the temperature of the zone has gone below the required temperature...
		return 1;
	}
	else if (tempSensor.getTemp() >= requestedTemp) { // If the temperature of the zone has reached the requried temperature...
		return 0;
	}
	return 2;
}

bool HeatingSystem::needToChangeTemp() {
	if (abs(lastChangedTemp - tempSensor.getTemp()) >= minTempDifference) { // If the difference between the current temperature and the last temperature is greater than min diff
		lastChangedTemp = tempSensor.getTemp();
		return true; // Heating system can be changed
	}
	return false; // Heating system won't need to be changed
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
void HeatingSystem::setWaterWithoutHeating() { // Function for turning hot water on without the unneeded heating components
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

void HeatingSystem::boostWater(bool state) { // Function sets the state of the system when the hot water boost is changed
	if (state == true) { // If boost has been turned on...
		enableWater(); // turn hot water on
		waterBoostActive = true; // Set waterBoostActive to on
		startTimeWaterBoost = millis(); // Store the time at which the boost was turned on for the timer
	}
	else if (state == false) { // If the boos thas been turned off...
		disableWater(); // Disable the water
		waterBoostActive = false; // Set waterBoostActive to off
		updateDisplay = true; // Update the display
	}
};

bool HeatingSystem::getHeatingStatus() { // Get the status of the heating
	return (pump.getStatus() && boiler.getStatus()); // If pump and boiler are on, heating is on
};
bool HeatingSystem::getWaterStatus() { // Function for getting the status of the hot waterg
	return boiler.getStatus(); // In our system, if the heating is on then the hot water is also on
};

void HeatingSystem::setTemp(int temp) { // Function for setting the requested temperature
	requestedTemp = temp; // Set requested temperature to the value passed
}
