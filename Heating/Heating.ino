/*
 Name:		Heating.ino
 Created:	5/6/2018 11:57:33 AM
 Author:	Samuel
*/

// the setup function runs once when you press reset or power the board




#include <SPI.h>
#include "HeatingSystem.h"
HeatingSystem* ashanti;
void setup() {
	Serial.begin(9600);
	ashanti = new HeatingSystem(3, 2, A4); // Pump pin, boiler pin, thermometer pin
	
}
void loop() {
	ashanti->monitorSystem();
};

