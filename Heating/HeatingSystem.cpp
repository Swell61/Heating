// 
// 
// 

#include "HeatingSystem.h"

HeatingSystem::HeatingSystem(int pumpPin, int boilerPin, int tempSensorPin) : pump(Pump(pumpPin)), boiler(Boiler(boilerPin)), tempSensor(TempSensor(tempSensorPin)), display(new Display()) {
	display->mainDisplay(tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);
};

void HeatingSystem::monitorSystem() { // This function runs through the process required to monitor and manage the heating system
	if (tempSensor.getTemp() < (requestedTemp - maxDrift) && !heatingMaster) { // If the temperature of the zone has gone below the required temperature...
		enableHeating(); // ...turn the heating on
	}
	else if (tempSensor.getTemp() >= requestedTemp && !heatingBoostActive) { // If the temperature of the zone has reached the requried temperature...
		disableHeating(); // ...turn the heating off
	}
	checkBoosts(); // Run through the boost timers, checking if they need altering

	display->mainDisplay(tempSensor.getTemp(), getHeatingStatus(), getWaterStatus(), requestedTemp, heatingBoostActive, waterBoostActive);

	touchOption = display->touchUpdate();
	if (touchOption == 1) {
		requestedTemp++;
	}
	else if (touchOption == 2) {
		requestedTemp--;
	}
	else if (touchOption == 3 && !heatingBoostActive) {
		heatingBoostActive = true;
	}
	else if (touchOption == 3 && heatingBoostActive) {
		heatingBoostActive = false;
	}
	else if (touchOption == 4 && !waterBoostActive) {
		waterBoostActive = true;
	}
	else if (touchOption == 4 && waterBoostActive) {
		waterBoostActive = false;
	}


	




	// Finally, evaluate the settings of the heating and water, then physically turn the heating systems on or off. Prevents unecessary changes to relay state
	if (heatingStatus && !getHeatingStatus()){ // If the heating should be on and it isn't already...
		setHeatingOn(); // ...turn it on
	}
	else if (waterStatus && !getWaterStatus()) { // Else if the hot water should be on and it isn't already...
		if (!heatingStatus) { // ...and the heating shouldn't be on...
			setWaterWithoutHeating(); // ...turn the boiler on and disable the pump
		}
		else { // Else the heating must be on
			setWaterOn(); // ...turn on the hot water without disabling the pump
		}
		
	}
	
	if (!waterStatus && !heatingStatus) { // If both the hot water and heating are off...
		disableWater(); // ...turn off the hot water
		disableHeating(); // ...turn off the heating
	}
};

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
	}
};

bool HeatingSystem::getHeatingStatus() {
	return (pump.getStatus() && boiler.getStatus());
};
bool HeatingSystem::getWaterStatus() {
	return pump.getStatus(); // In our system, if the heating is on then the hot water is also on
};

void HeatingSystem::checkBoosts() {
	if (heatingBoostActive && (abs(millis() - startTimeHeatingBoost) >= (boostLengthHeating * 60000))) {
		disableHeating();
	}
	else if (waterBoostActive && (abs(millis() - startTimeWaterBoost) >= (boostLengthWater * 60000))) {
		disableWater();
	}
};

void HeatingSystem::setTemp(int temp) {
	requestedTemp = temp;
}
