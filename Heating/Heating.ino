/*
 Name:		Heating.ino
 Created:	5/6/2018 11:57:33 AM
 Author:	Samuel
*/

// the setup function runs once when you press reset or power the board
#include "src/Controller.h"
Controller* ashanti;
void setup() {
    Serial.begin(115200);
    ashanti = new Controller(2,3,A4);
    Serial.println("Init");
}
void loop() {
    ashanti->loop();
};

