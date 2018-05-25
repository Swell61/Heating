/*
 Name:		Heating.ino
 Created:	5/6/2018 11:57:33 AM
 Author:	Samuel
*/

// the setup function runs once when you press reset or power the board

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <UIPEthernet.h>
#include <SPI.h>
#include <SD.h>
#include "HeatingSystem.h"
void setup() {
	HeatingSystem ashanti(2, 5);
}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.print("Loop");
}