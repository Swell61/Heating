/*
 Name:		Heating.ino
 Created:	5/6/2018 11:57:33 AM
 Author:	Samuel
*/

// the setup function runs once when you press reset or power the board
#include "BoilerControlComponentOut.h"
#include <TFT.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>
#include <SD.h>
#include "HeatingSystem.h"
void setup() {
	HeatingSystem ashanti(2, 5);
	Serial.print(ashanti.getPump().getStatus());
	ashanti.getPump().enable();
	Serial.print(ashanti.getPump().getStatus());
}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.print("Loop");
}
