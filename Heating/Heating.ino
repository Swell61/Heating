/*
 Name:		Heating.ino
 Created:	5/6/2018 11:57:33 AM
 Author:	Samuel
*/

// the setup function runs once when you press reset or power the board

#include <MemoryFree.h>
#include <SPI.h>
#include <UIPEthernet.h>
#include "HeatingSystem.h"
unsigned long lastUpdate = 0;
HeatingSystem* ashanti;
void setup() {
	
	ashanti = new HeatingSystem(45, 46, 47); // Pump pin, boiler pin, thermometer pin
	Serial.begin(9600);
}
void loop() {
	ashanti->monitorSystem();
	if ((millis() - lastUpdate) >= 60000) {
		Serial.println(freeMemory());
		lastUpdate = millis();
	}
};

