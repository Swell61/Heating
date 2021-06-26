// 
// 
// 

#include "HeatingSystem.h"

HeatingSystem::HeatingSystem(int pumpPin, int boilerPin, int tempSensorPin, int intTempSensorPin) : pump(Pump(pumpPin)), boiler(Boiler(boilerPin)), timer(Timer()), remote(WebInterface(SD.begin(5))), config(Config("config.txt")) { // Constructor. Initialises the componenets of the heating system
	TempSensor::begin(tempSensorPin);
	tempSensor = TempSensor();
	uint8_t internalTempSensorAddress[8] = { 0x28, 0xFF, 0x68, 0x08, 0xA2, 0x17, 0x04, 0x21 };
	internalTempSensor = TempSensor(internalTempSensorAddress);
	currentTemp = tempSensor.getTemp(); // Gets the current temperature to display
	currentInternalTemp = internalTempSensor.getTemp();
	setHeatingOff(); // Disables the heating
	setWaterOff(); // Disables the ho twater
	Serial.println((SDAvailable = SD.begin(5)) ? "SD UP" : "SD DOWN"); // Checks whether the SD card is available or not
	if (SDAvailable) {
		loadTimer();
	}
	int NTPTryCount = 0; // Variable to store the number of tries to get the time from the NTP server
	unsigned long time = 0; // Variable to store the time from the NTP server
	while (NTPTryCount < 5) { // Tries 5 times to get the time
		Serial.println("NTP");
		//display->loadingScreen(SDAvailable, NTPTryCount + 1); // Shows the loading screen
		bool success = timer.getClock().synchroniseWithNtp(udp);
		if (success) { // If the time is not zero (very unlikely it will be zero and if it is, it will run another check if it has 1+ remaining)
			NTPTryCount = 5; // Exceeds the max tries so the loop will break
		}
		else { // If failed to get time...
			NTPTryCount++; // ...increase the try count and loop round again if 1+ tries left
		}
	}
	setupWatchdog();

	//resetCounter = atoi(config.readProperty("reset"));
	//resetCounter++;
	//itoa(resetCounter, buffer, 10);
	//config.writeProperty("reset", buffer);

	//display->mainDisplay(timer.getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive); // Show the main display
	pinMode(12, OUTPUT);
	digitalWrite(12, HIGH);
	pinMode(5, OUTPUT);
	digitalWrite(5, HIGH);
};

void HeatingSystem::setupWatchdog() {
	cli(); // disable all interrupts
	wdt_reset(); // reset the WDT timer
	wdt_enable(WDTO_4S);
	wdt_reset(); // reset the WDT timer

}

void HeatingSystem::loadTimer() {
	// Load timer
	// int time;
	// if ((time = atoi(config.readProperty("heatMon"))) != 0) {
	// 	timer.setHeatingOnMorning(time);
	// }
	// if ((time = atoi(config.readProperty("heatMoff"))) != 0) {
	// 	timer.setHeatingOffMorning(time);
	// }
	// if ((time = atoi(config.readProperty("heatAon"))) != 0) {
	// 	timer.setHeatingOnAfternoon(time);
	// }
	// if ((time = atoi(config.readProperty("heatAoff"))) != 0) {
	// 	timer.setHeatingOffAfternoon(time);
	// }

	// if ((time = atoi(config.readProperty("waterMon"))) != 0) {
	// 	timer.setWaterOnMorning(time);
	// }
	// if ((time = atoi(config.readProperty("waterMoff"))) != 0) {
	// 	timer.setWaterOffMorning(time);
	// }
	// if ((time = atoi(config.readProperty("waterAon"))) != 0) {
	// 	timer.setWaterOnAfternoon(time);
	// }
	// if ((time = atoi(config.readProperty("waterAoff"))) != 0) {
	// 	timer.setWaterOffAfternoon(time);
	// }
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
			//display->displayUpdate(timer.getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive); // Show the main display
																																																	   // Update any connected clients with the current status
			pinMode(12, OUTPUT);
			digitalWrite(12, LOW);

			Serial.print("Update: ");
			Serial.println(requestedTemp);
			remote.processRemoteOutput(timer.getClock().getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive, currentInternalTemp);
			remote.processRemoteOutput(heatingMode == 1, waterMode == 1, timer);
			Serial.println("Done update");
			pinMode(12, OUTPUT);
			digitalWrite(12, HIGH);
		}
		else if (screen == 1) { // If on the timer display
			config.writeProperty(buffer, "2");

			//display->timerUpdate(heatingMode == 1 ? true : false, waterMode == 1 ? true : false, timer.getHeatingOnMorning(), timer.getHeatingOffMorning(), timer.getHeatingOnAfternoon(), timer.getHeatingOffAfternoon(), timer.getWaterOnMorning(), timer.getWaterOffMorning(), timer.getWaterOnAfternoon(), timer.getWaterOffAfternoon()); // Show the timer display
																																																																																			  // Update any connected clients with the current status
			pinMode(12, OUTPUT);
			digitalWrite(12, LOW);
			remote.processRemoteOutput(heatingMode == 1, waterMode == 1, timer);
			remote.processRemoteOutput(timer.getClock().getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive, currentInternalTemp);
			pinMode(12, OUTPUT);
			digitalWrite(12, HIGH);
		}
		else if (screen == 2) { // If on the time edit display
			config.writeProperty(buffer, "3");

			//display->updateEditTime(timer.getTimeInMinutes()); // Show the time edit display
		}
		updateDisplay = false; // Updating screen has completed
	}
	if (abs(currentTemp - tempSensor.getTemp()) >= minTempDifference) { // Run temperature check for currently displayed temperature

		currentTemp = tempSensor.getTemp(); // If not correct, get corret temperature and...
		updateDisplay = true; // ...update the display
	};
	if (abs(currentInternalTemp - internalTempSensor.getTemp()) >= minTempDifference) {
		currentInternalTemp = internalTempSensor.getTemp();
		updateDisplay = true;
	};

	pinMode(12, OUTPUT);
	digitalWrite(12, LOW);
	remoteOption = remote.processRemoteInput(buffer, config); // Process any clients
	pinMode(12, OUTPUT);
	digitalWrite(12, HIGH);
	touchOption = 0; //display->touchUpdate(screen); // Get the current requested touchscreen option

	

	if (remoteOption == 255) { // If a new client has just joined
		config.writeProperty(buffer, "6");
							   // Send current system status
		pinMode(12, OUTPUT);
		digitalWrite(12, LOW);
			Serial.print("New client: ");
			Serial.println(requestedTemp);
		remote.processRemoteOutput(timer.getClock().getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive, currentInternalTemp);
		remote.processRemoteOutput(heatingMode == 1, waterMode == 1, timer);
		pinMode(12, OUTPUT);
		digitalWrite(12, HIGH);
	}
	else if (touchOption == 0) { // Touschreen takes priority. If no touch option, check remote requests
		touchOption = remoteOption;
	}
	wdt_reset(); // Reset the timer
	if (remoteOption >= 1 && remoteOption <= 6) {
	}
	else if (remoteOption >= 7 && remoteOption <= 22) {
	}
	else if (remoteOption >= 23 && remoteOption <= 27) {
	}
	else if (remoteOption >= 28 && remoteOption <= 35) {
	}
	SystemFunction option = SystemFunction::NONE;
	pinMode(5, OUTPUT);
	digitalWrite(5, LOW);
	switch (option) {
		case SystemFunction::INCREASE_REQ_TEMP: { // User requested temperature up one degree
			if (requestedTemp < 28) {
				requestedTemp = requestedTemp + tempChange; // Increase the requested temperature up by one
				Serial.print("Main case: ");
				Serial.println(requestedTemp);
				updateDisplay = true; // The display needs updating
			}
			break;
		}
		case SystemFunction::DECREASE_REQ_TEMP: { // User requested temperature down one degree
			if (requestedTemp > 10) {
				requestedTemp = requestedTemp - tempChange; // Decrease the requested temperature down by one
				updateDisplay = true; // The display needs updating
			}
			break;
		}
		case SystemFunction::HEATING_BOOST: {
			if (!heatingBoostActive) { // User wants to turn the heating boost on
				updateDisplay = true; // Display needs updating
				boostHeating(true);
			}
			else if (heatingBoostActive) { // User wants to turn the heating boost off
				updateDisplay = true; // Display needs updating
				boostHeating(false);
			}
			break;
		}
		case SystemFunction::WATER_BOOST: { 
			if (!waterBoostActive) { // User wants to turn the water boost on
				updateDisplay = true; // Display needs updating
				boostWater(true);
			}
			else if (waterBoostActive) { // User wants to turn the water boost off
				updateDisplay = true; // Display needs updating
				boostWater(false);
			}
			break;
		}
		case SystemFunction::TIMER_DISPLAY: { // Go to timer display
			updateDisplay = true;
			screen = 1;
			//display->timerDisplay(heatingMode == 1 ? true : false, waterMode == 1 ? true : false, timer.getHeatingOnMorning(), timer.getHeatingOffMorning(), timer.getHeatingOnAfternoon(), timer.getHeatingOffAfternoon(), timer.getWaterOnMorning(), timer.getWaterOffMorning(), timer.getWaterOnAfternoon(), timer.getWaterOffAfternoon());
			break;
		}
		case SystemFunction::MAIN_DISPLAY: { // Go to main display
			updateDisplay = true;
			screen = 0;
			//display->mainDisplay(timer.getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
			break;
		}
		
		// case 23: { // Change heating timer state
		// 	timer.setHeatingTimerState(!timer.getHeatingTimerStatus());
		// 	updateDisplay = true;
		// 	break;
		// }
		// case 24: { // Change hot water timer state
		// 	timer.setWaterTimerState(!timer.getWaterTimerStatus());
		// 	updateDisplay = true;
		// 	break;
		// }
		case SystemFunction::TIMER_ADJUST: {
			break;
		}
		case SystemFunction::HEATING_MODE: { // Change heating mode
			heatingMode++;
			if (heatingMode > 2) {
				heatingMode = 0;
			}
			updateDisplay = true;
			break;
		}
		case SystemFunction::WATER_MODE: { // Change hot water mode
			waterMode++;
			if (waterMode > 2) {
				waterMode = 0;
			}
			updateDisplay = true;
			break;
		}
		case SystemFunction::TIME_DISPLAY: {
			screen = 2; // Change to edit time screen
			//display->editTime(timer.getTimeInMinutes());
			updateDisplay = true;
			break;
		}
		case SystemFunction::TIME_ADJUST: {
			break;
		}
	}
	pinMode(5, OUTPUT);
	digitalWrite(5, HIGH);
	checkBoosts(); // Run through the boost timers, checking if they need altering
	changeRelayStates(); // make any required changed to the relays

	if ((millis() - lastTimeUpdate) >= 59900) { // Update the display once every minute to update the time

		updateDisplay = true; // Update the display
		lastTimeUpdate = millis(); // Update the lsat time the time was updated
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
	if (temperatureCheck() == 1 && (heatingBoostActive || heatingMode == 2 || (heatingMode == 1 && timer.getHeatingTimerStatus()))) { // if ((Heating is ON OR (Heating Timer is ON and ACTIVE)) AND temperature is too low)

		enableHeating();
	}
	else if (heatingMode == 0 || (temperatureCheck() == 0 && (heatingBoostActive || heatingMode == 2 || (heatingMode == 1 && timer.getHeatingTimerStatus()))) || (heatingMode == 1 && !timer.getHeatingTimerStatus())) {

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
