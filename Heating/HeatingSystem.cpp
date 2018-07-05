// 
// 
// 

#include "HeatingSystem.h"

HeatingSystem::HeatingSystem(int pumpPin, int boilerPin, int tempSensorPin) : pump(Pump(pumpPin)), boiler(Boiler(boilerPin)), tempSensor(TempSensor(tempSensorPin)), display(new Display()) {
	currentTemp = tempSensor.getTemp();
	display->mainDisplay(tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
};

void HeatingSystem::monitorSystem() { // This function runs through the process required to monitor and manage the heating system
	
	if (updateDisplay) {
		display->displayUpdate(tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
		updateDisplay = false;
		Serial.println("Display updated" + String(millis()));
	}
	if (currentTemp != tempSensor.getTemp()) {
		currentTemp = tempSensor.getTemp();
		updateDisplay = true;
	};
	touchOption = display->touchUpdate();
	if (touchOption == 1) { // User requested temperature up one degree
		requestedTemp++; // Increase the requested temperature up by one
		updateDisplay = true; // The display needs updating
	}
	else if (touchOption == 2) { // User requested temperature down one degree
		requestedTemp--; // Decrease the requested temperature down by one
		updateDisplay = true; // The display needs updating
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
	};
	temperatureCheck();
	
	checkBoosts(); // Run through the boost timers, checking if they need altering
	changeRelayStates();
};
void HeatingSystem::temperatureCheck() {
	if (tempSensor.getTemp() < (requestedTemp - maxDrift) && heatingMaster && !getHeatingStatus()) { // If the temperature of the zone has gone below the required temperature...
		enableHeating(); // ...turn the heating on
		Serial.println("Heating on because temperature too low");
		incorrectTemp = true;
	}
	else if (tempSensor.getTemp() >= requestedTemp && !heatingBoostActive && getHeatingStatus() && !heatingBoostActive && !waterBoostActive) { // If the temperature of the zone has reached the requried temperature...
		disableHeating(); // ...turn the heating off
		Serial.println("1");
		incorrectTemp = false;
	}
}
void HeatingSystem::changeRelayStates() {
	if (!heatingMaster && getHeatingStatus()) {
		setHeatingOff();
		Serial.print("2");
	}
	else if (heatingStatus && !getHeatingStatus()) { // If the heating should be on and it isn't already...
		setHeatingOn(); // ...turn it on
		Serial.println("Set heating on");
		updateDisplay = true;
	}
	else if (waterStatus && !incorrectTemp) { // Else if the hot water should be on and it isn't already...
		if (!heatingStatus && getHeatingStatus()) { // ...and the heating shouldn't be on...
			setWaterWithoutHeating(); // ...turn the boiler on and disable the pump
			Serial.println("Set water on wthout heating");
			Serial.print("4");
			updateDisplay = true;
		}
		else if (!getWaterStatus()){ // Else the heating must be on
			setWaterOn(); // ...turn on the hot water without disabling the pump
			Serial.println("Set water");
			updateDisplay = true;
		}
		
	}
	else if (!waterStatus && !heatingStatus && getWaterStatus() && getHeatingStatus() && !incorrectTemp) { // If both the hot water and heating are off...
		setWaterOff(); // ...turn off the hot water
		setHeatingOff(); // ...turn off the heating
		updateDisplay = true;
		Serial.println("Turn everything off");
	}
	
	// Finally, evaluate the settings of the heating and water, then physically turn the heating systems on or off. Prevents unecessary changes to relay state
	else if (!waterStatus && getWaterStatus() && !heatingStatus && !incorrectTemp) { // If water should be off and it's not...
		setWaterOff(); // Turn the water off
		Serial.println("Should turn water components off");
	}
	else if (!heatingStatus && getHeatingStatus() && !incorrectTemp) {
		setHeatingOff();
		Serial.println("Heating going off");
	}
	
}

void HeatingSystem::setHeatingOn() { // Function for turning on the heating
	boiler.enable(); // Turn the boiler on
	pump.enable(); // Turn the pump on
	Serial.println("Heating componenets on");
};
void HeatingSystem::setHeatingOff() { // Function for turning off the heating
	boiler.disable(); // Turn the boiler off
	pump.disable(); // TUrn the pump off
	Serial.println("Heating now off");
};

void HeatingSystem::setWaterOn() { // Function for turning the hot water on
	boiler.enable(); // Turn the boiler on
	Serial.println("Hot water components on");
}
void HeatingSystem::setWaterOff() { // Function for turning the hot water off
	boiler.disable(); // Turn the boiler off
	Serial.println("Hot water components off");
};
void HeatingSystem::setWaterWithoutHeating() {
	boiler.enable();
	pump.disable();
	Serial.println("Water components on without heating");
}
void HeatingSystem::enableHeating() {
	heatingStatus = true;
	Serial.println("Enabling the heating");
}
void HeatingSystem::disableHeating() {
	heatingStatus = false;
	Serial.println("Disable the heating");
}
void HeatingSystem::enableWater() {
	waterStatus = true;
	Serial.println("Enable the hot water");
}
void HeatingSystem::disableWater() {
	waterStatus = false;
	Serial.println("Disable the hot water");
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

void HeatingSystem::checkBoosts() {
	if (heatingBoostActive && ((millis() - startTimeHeatingBoost) >= (boostLengthHeating * 60000))) {
		boostHeating(false);
	}
	else if (waterBoostActive && ((millis() - startTimeWaterBoost) >= (boostLengthWater * 60000))) {
		boostWater(false);
	}
};

void HeatingSystem::setTemp(int temp) {
	requestedTemp = temp;
}
