/*
 Name:		Heating.ino
 Created:	5/6/2018 11:57:33 AM
 Author:	Samuel
*/

// the setup function runs once when you press reset or power the board



#include <SPI.h>
#include "HeatingSystem.h"
HeatingSystem ashanti(3, 2, A4, A4);
void setup() {
}
void loop() {
	ashanti.monitorSystem();
};

