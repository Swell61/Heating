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
#include <SD.h>
unsigned long lastUpdate = 0;
HeatingSystem* ashanti;
File logFile;
int previousMemory = 0;
void setup() {
	
	ashanti = new HeatingSystem(45, 46, 47); // Pump pin, boiler pin, thermometer pin
	Serial.begin(9600);
	SD.begin(49);
	if (!SD.exists("log.txt")) {
		SD.mkdir("log.txt");
	}
}
void loop() {
	ashanti->monitorSystem();
	if ((millis() - lastUpdate) >= 60000) {
		Serial.println(freeMemory());
		lastUpdate = millis();
	}
	if (freeMemory() != previousMemory) {
		Serial.println("Writing to file"); 
		previousMemory = freeMemory();
		Serial.println(freeMemory());
		logFile = SD.open("log.txt", FILE_WRITE);
		logFile.println(freeMemory());
		
		logFile.close();
		
	}
};

