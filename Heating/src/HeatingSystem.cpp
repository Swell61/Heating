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
	Serial.println((SDAvailable = SD.begin(5)) ? "SD UP" : "SD DOWN"); // Checks whether the SD card is available or not
	if (SDAvailable) {
		loadTimer();
	}
	int NTPTryCount = 0; // Variable to store the number of tries to get the time from the NTP server
	unsigned long time = 0; // Variable to store the time from the NTP server

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

	SystemFunction option = convertCommand(remoteOption);
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

SystemFunction HeatingSystem::convertCommand(unsigned short int command) {
	if (command <= 8 || (command >= 25 && command <= 28)) {
		return static_cast<SystemFunction>(command);
	}
	else if (command >= 8 && command <= 24) {
		// do nothing at the moment
	}
	return SystemFunction::NONE;
	
}



bool HeatingSystem::needToChangeTemp() {
	if (abs(lastChangedTemp - tempSensor.getTemp()) >= minTempDifference) { // If the difference between the current temperature and the last temperature is greater than min diff
		lastChangedTemp = tempSensor.getTemp();
		return true; // Heating system can be changed
	}
	return false; // Heating system won't need to be changed
}

