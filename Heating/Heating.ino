/*
 Name:		Heating.ino
 Created:	5/6/2018 11:57:33 AM
 Author:	Samuel
*/

// the setup function runs once when you press reset or power the board
#include "src/Controller.h"
#include <Arduino.h>
Controller* ashanti;
void setup() {
    Serial.begin(115200);
	wdt_enable(WDTO_4S);
    ashanti = new Controller(2,3,A4);
}
void loop() {
    ashanti->loop();
};

