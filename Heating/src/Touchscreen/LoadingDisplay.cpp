#include "LoadingDisplay.h"

bool LoadingDisplay::display(Config& config, unsigned char ntpMaxRetries, MCUFRIEND_kbv& display) {
    display.setTextColor(0xFFFFF, 0x00000); // Set the text colour to white and background to black
	display.setCursor(0, 0); // Set the cursor to write text to the display
	display.setTextSize(2); // Set text size to 2
	display.println(F("Loading...")); // Print description of what is happening
	display.print(F("Config "));
	display.println(config.available() ? "UP" : "DOWN");
	display.print("Getting time from NTP server...  (");
	display.print("1");
	display.print("/"); // Print current number of tries to get time from NTP server
	display.print(ntpMaxRetries);
	display.println(")");
}

bool LoadingDisplay::update(unsigned char ntpTry, MCUFRIEND_kbv& display) {
	display.setCursor(2, 35);
	display.print(ntpTry);
	if (ntpTry > 1) { // If it has taken more than one try...
		display.setCursor(3, 0);
		display.println(F("Check Ethernet cable and internet conectivity")); // ...ask the user to check possible faults
	}
}