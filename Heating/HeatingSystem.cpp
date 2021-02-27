// 
// 
// 

#include "HeatingSystem.h"

HeatingSystem::HeatingSystem(int pumpPin, int boilerPin, int tempSensorPin, int intTempSensorPin) : pump(Pump(pumpPin)), boiler(Boiler(boilerPin)), timer(Timer()), display(new Display()), remote(WebInterface(SD.begin(5))), config(Config("config.txt")) { // Constructor. Initialises the componenets of the heating system
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
		
		display->loadingScreen(SDAvailable, NTPTryCount + 1); // Shows the loading screen
		time = timer.getNTPTime(udp); // Tries to get the time
		if (time != 0) { // If the time is not zero (very unlikely it will be zero and if it is, it will run another check if it has 1+ remaining)
			timer.setSystemTime(time);
			NTPTryCount = 5; // Exceeds the max tries so the loop will break
		}
		else { // If failed to get time...
			NTPTryCount++; // ...increase the try count and loop round again if 1+ tries left
		}
	}
	setupWatchdog();

	resetCounter = atoi(config.readProperty("reset"));
	resetCounter++;
	itoa(resetCounter, buffer, 10);
	config.writeProperty("reset", buffer);

	display->mainDisplay(timer.getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive); // Show the main display
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

		if (screen == Screen::Main) { // If on main display
			display->displayUpdate(timer.getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive); // Show the main display
																																																	   // Update any connected clients with the current status
			pinMode(12, OUTPUT);
			digitalWrite(12, LOW);
			remote.processRemoteOutput(timer.getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive, currentInternalTemp);
			remote.processRemoteOutput(heatingMode, waterMode, timer);
			pinMode(12, OUTPUT);
			digitalWrite(12, HIGH);
		}
		else if (screen == Screen::Timer) { // If on the timer display
			config.writeProperty(buffer, "2");

			display->timerUpdate(heatingMode, waterMode, timer); // Show the timer display
																																																																																			  // Update any connected clients with the current status
			pinMode(12, OUTPUT);
			digitalWrite(12, LOW);
			remote.processRemoteOutput(heatingMode, waterMode, timer);
			remote.processRemoteOutput(timer.getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive, currentInternalTemp);
			pinMode(12, OUTPUT);
			digitalWrite(12, HIGH);
		}
		else if (screen == Screen::AlterTime) { // If on the time edit display
			config.writeProperty(buffer, "3");

			display->updateEditTime(timer.getTimeInMinutes()); // Show the time edit display
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
	touchOption = display->touchUpdate(screen); // Get the current requested touchscreen option

	

	if (remoteOption == Function::NewClient) { // If a new client has just joined
		config.writeProperty(buffer, "6");
							   // Send current system status
		pinMode(12, OUTPUT);
		digitalWrite(12, LOW);
		remote.processRemoteOutput(timer.getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive, currentInternalTemp);
		remote.processRemoteOutput(heatingMode, waterMode, timer);
		pinMode(12, OUTPUT);
		digitalWrite(12, HIGH);
	}
	else if (touchOption == Function::None) { // Touschreen takes priority. If no touch option, check remote requests
		touchOption = remoteOption;
	}
	wdt_reset(); // Reset the timer
	pinMode(5, OUTPUT);
	digitalWrite(5, LOW);
	switch (touchOption) {
		case Function::Up : { // User requested temperature up one degree
			if (requestedTemp < 28) {
				requestedTemp = requestedTemp + tempChange; // Increase the requested temperature up by one
				updateDisplay = true; // The display needs updating
			}
			break;
		}
		case Function::Down : { // User requested temperature down one degree
			if (requestedTemp > 10) {
				requestedTemp = requestedTemp - tempChange; // Decrease the requested temperature down by one
				updateDisplay = true; // The display needs updating
			}
			break;
		}
		case Function::HeatingBoost :
			updateDisplay = true;
			boostHeating(!heatingBoostActive);
			break;

		case Function::WaterBoost :
			updateDisplay = true;
			boostWater(!waterBoostActive);
			break;
		
		case Function::TimerDisplay : { // Go to timer display
				updateDisplay = true;
				screen = Screen::Timer;
				display->timerDisplay(heatingMode, waterMode, timer);
			}
			break;
		case Function::MainDisplay : { // Go to main display
				updateDisplay = true;
				screen = Screen::Main;
				display->mainDisplay(timer.getTimeInMinutes(), heatingMode, waterMode, tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
			}
			break;
		case Function::ScheduleHeatingMorningOnDown : { // Change heating on morning time 
				if (timer.setHeatingOnMorning(timer.getHeatingOnMorning() - timerTimeInc)) {
					updateDisplay = true;
					saveTimer("heatMon", timer.getHeatingOnMorning());
				}
			}
			break;
		case Function::ScheduleHeatingMorningOnUp : { // Change heating on morning time 
				if (timer.setHeatingOnMorning(timer.getHeatingOnMorning() + timerTimeInc)) {
					updateDisplay = true;
					saveTimer("heatMon", timer.getHeatingOnMorning());
				}
			}
			break;
		case Function::ScheduleHeatingMorningOffDown : { // Change heating off morning time 
				if (timer.setHeatingOffMorning(timer.getHeatingOffMorning() - timerTimeInc)) {
					updateDisplay = true;
					saveTimer("heatMoff", timer.getHeatingOffMorning());
				}
			}
			break;
		case Function::ScheduleHeatingMorningOffUp : { // Change heating off morning time
				if (timer.setHeatingOffMorning(timer.getHeatingOffMorning() + timerTimeInc)) {
					updateDisplay = true;
					saveTimer("heatMoff", timer.getHeatingOffMorning());
				}
			}
			break;
		case Function::ScheduleHeatingAfternoonOnDown : { // Change heating on afternoon time
				if (timer.setHeatingOnAfternoon(timer.getHeatingOnAfternoon() - timerTimeInc)) {
					updateDisplay = true;
					saveTimer("heatAon", timer.getHeatingOnAfternoon());
				}
			}
			break;
		case Function::ScheduleHeatingAfternoonOnUp : { // Change heating on afternoon time
				if (timer.setHeatingOnAfternoon(timer.getHeatingOnAfternoon() + timerTimeInc)) {
					updateDisplay = true;
					saveTimer("heatAon", timer.getHeatingOnAfternoon());
				}
			}
			break;
		case Function::ScheduleHeatingAfternoonOffDown : { // Change heating off afternoon time
				if (timer.setHeatingOffAfternoon(timer.getHeatingOffAfternoon() - timerTimeInc)) {
					updateDisplay = true;
					saveTimer("heatAoff", timer.getHeatingOffAfternoon());
				}
			}
			break;
		case Function::ScheduleHeatingAfternoonOffUp : { // Change heating off afternoon time
				if (timer.setHeatingOffAfternoon(timer.getHeatingOffAfternoon() + timerTimeInc)) {
					updateDisplay = true;
					saveTimer("heatAoff", timer.getHeatingOffAfternoon());
				}
			}
			break;
		case Function::ScheduleHotWaterMorningOnDown : { // Change hot water on morning time
			if (timer.setWaterOnMorning(timer.getWaterOnMorning() - timerTimeInc)) {
				updateDisplay = true;
				saveTimer("waterMon", timer.getWaterOnMorning());
			}
		}
			break;
		case Function::ScheduleHotWaterMorningOnUp : { // Change hot water on morning time
			if (timer.setWaterOnMorning(timer.getWaterOnMorning() + timerTimeInc)) {
				updateDisplay = true;
				saveTimer("waterMon", timer.getWaterOnMorning());
			}
		}
			break;
		case Function::ScheduleHotWaterMorningOffDown : { // Change hot water off morning time
			if (timer.setWaterOffMorning(timer.getWaterOffMorning() - timerTimeInc)) {
				updateDisplay = true;
				saveTimer("waterMoff", timer.getWaterOffMorning());
			}
		}
			break;
		case Function::ScheduleHotWaterMorningOffUp : { // Change hot water off morning time
			if (timer.setWaterOffMorning(timer.getWaterOffMorning() + timerTimeInc)) {
				updateDisplay = true;
				saveTimer("waterMoff", timer.getWaterOffMorning());
			}
		}
			break;
		case Function::ScheduleHotWaterAfternoonOnDown : { // Change hot water on afternoon time
			if (timer.setWaterOnAfternoon(timer.getWaterOnAfternoon() - timerTimeInc)) {
				updateDisplay = true;
				saveTimer("waterAon", timer.getWaterOnAfternoon());
			}
		}
			break;
		case Function::ScheduleHotWaterAfternoonOnUp : { // Change hot water on afternoon time
			if (timer.setWaterOnAfternoon(timer.getWaterOnAfternoon() + timerTimeInc)) {
				updateDisplay = true;
				saveTimer("waterAon", timer.getWaterOnAfternoon());
			}
		}
			break;
		case Function::ScheduleHotWaterAfternoonOffDown : { // Change hot water off afternoon time
			if (timer.setWaterOffAfternoon(timer.getWaterOffAfternoon() - timerTimeInc)) {
				updateDisplay = true;
				saveTimer("waterAoff", timer.getWaterOffAfternoon());
			}
		}
			break;
		case Function::ScheduleHotWaterAfternoonOffUp : { // Change hot water off afternoon time
			if (timer.setWaterOffAfternoon(timer.getWaterOffAfternoon() + timerTimeInc)) {
				updateDisplay = true;
				saveTimer("waterAoff", timer.getWaterOffAfternoon());
			}
		}
			break;
		case Function::HeatingTimerState : { // Change heating timer state
			timer.setHeatingTimerState(!timer.getHeatingTimerStatus());
			updateDisplay = true;
		}
			break;
		case Function::HotWaterTimerState : { // Change hot water timer state
			timer.setWaterTimerState(!timer.getWaterTimerStatus());
			updateDisplay = true;
		}
			break;
		case Function::HeatingMode : { // Change heating mode
			heatingMode = incrementMode(heatingMode);
			updateDisplay = true;
		}
			break;
		case Function::HotWaterMode : { // Change hot water mode
			waterMode = incrementMode(heatingMode);
			updateDisplay = true;
		}
			break;
		case Function::CurrentTimeDisplay : {
			screen = Screen::AlterTime; // Change to edit time screen
			display->editTime(timer.getTimeInMinutes());
			updateDisplay = true;
		}
			break;
		case Function::IncreaseTensHours : {

			if (timer.setSystemHour(timer.getHour() + 10)) { // Increase current time by 10 hours
				updateDisplay = true;
			}
		}
			break;
		case Function::DecreaseTensHours : {
			if (timer.setSystemHour(timer.getHour() - 10)) { // Decrease current time by 10 hours
				updateDisplay = true;
			}
		}
		break;
		case Function::IncreaseHours : {
			if (timer.setSystemHour(timer.getHour() + 1)) { // Increase current time by 1 hour
				updateDisplay = true;
			}
		}
		break;
		case Function::DecreaseHours : {
			if (timer.setSystemHour(timer.getHour() - 1)) { // Decrease current time by 1 hour
				updateDisplay = true;
			}
		}
		break;
		case Function::IncreaseTensMinutes : {
			if (timer.setSystemMinute(timer.getMinute() + 10)) { // Increase current time by 10 minutes
				updateDisplay = true;
			}
		}
		break;
		case Function::DecreaseTensMinutes: {
			if (timer.setSystemMinute(timer.getMinute() - 10)) { // Decrease current time by 10 hours
				updateDisplay = true;
			}
		}
		break;
		case Function::IncreaseMinutes :{
			if (timer.setSystemMinute(timer.getMinute() + 1)) { // Increase current time by 1 minute
				updateDisplay = true;
			}
		}
		break;
		case Function::DecreaseMinutes : {

			if (timer.setSystemMinute(timer.getMinute() - 1)) { // Decrease current time by 1 minute
				updateDisplay = true;
			}
		}
		break;
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

Mode HeatingSystem::incrementMode(Mode mode) {
	switch (mode) {
		case Mode::Off: return Mode::Timer;
		case Mode::Timer: return Mode::On;
		case Mode::On: return Mode::Off;
	}
}

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
	if (temperatureCheck() == 1 && (heatingBoostActive || heatingMode == Mode::On || (heatingMode == Mode::Timer && timer.getHeatingTimerStatus()))) { // if ((Heating is ON OR (Heating Timer is ON and ACTIVE)) AND temperature is too low)

		enableHeating();
	}
	else if (heatingMode == Mode::Off || (temperatureCheck() == 0 && (heatingBoostActive || heatingMode == Mode::On || (heatingMode == Mode::Timer && timer.getHeatingTimerStatus()))) || (heatingMode == Mode::Timer && !timer.getHeatingTimerStatus())) {

		disableHeating();
	}

	// Water settings
	if (waterBoostActive) { // If water boost is active...
		enableWater(); // ...turn the water on
	}
	else if (waterMode == Mode::On || (waterMode == Mode::Timer && timer.getWaterTimerStatus())) { // if (Heating is ON OR (Water Timer is ON and ACTIVE))
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
