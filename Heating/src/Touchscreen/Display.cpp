// 
// 
// 

#include "Display.h"
const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
Display::Display() : touchScreen(TouchScreen(XP, YP, XM, YM, 300)), currentScreen(&mainDisplay) { // Constructor. Initialises the touchscreen and the TFT
	display.begin(0x9327); // Initialise the TFT with it's address
	display.setRotation(3); // Set the rotation to landscape no2
	display.fillScreen(0x00000); // Fill the screen with black
};




SystemFunction Display::getTouchInput() { // Function for checking the touchscreen
	return currentScreen->getTouchInput(touchScreen);
}

bool Display::update(CoreComponents& components, SystemFunction systemFunction) {
	Serial.println("Screen update");
	switch (systemFunction) {
		case SystemFunction::MAIN_DISPLAY: {
			Serial.println("Showing main screen");
			currentScreen = &mainDisplay;
			return currentScreen->display(components, display);
		}
		case SystemFunction::TIMER_DISPLAY: {
			Serial.println("Showing timer screen");
			currentScreen = &timerDisplay;
			return currentScreen->display(components, display);
		}
		case SystemFunction::CURRENT_TIME_DISPLAY: {
			currentScreen = &timeDisplay;
			return currentScreen->display(components, display);
		}
		default: {
			return currentScreen->update(components, display, systemFunction);
		}
	}
}

void Display::loadingScreen(bool SDAvailable, int tryNum) { // Function for displaying the loading screen
	
}

MCUFRIEND_kbv& Display::getDisplay() {
	return display;
}