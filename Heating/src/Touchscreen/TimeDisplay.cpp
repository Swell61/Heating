#include "TimeDisplay.h"

bool TimeDisplay::display(CoreComponents& components, MCUFRIEND_kbv& display) {

	display.fillScreen(0x00000); // Fill the screen black
	
	displayTime(components.getClock(), display);

	display.setTextSize(4); // Decrease text size to 4

	// Print the plus and minus adjust buttons
	display.setCursor(100, 30);
	display.print(F("+"));
	display.setCursor(145, 30);
	display.print(F("+"));
	display.setCursor(227, 30);
	display.print(F("+")); 
	display.setCursor(272, 30);
	display.print(F("+"));

	display.setCursor(100, 185);
	display.print(F("-"));
	display.setCursor(145, 185);
	display.print(F("-"));
	display.setCursor(227, 185);
	display.print(F("-"));
	display.setCursor(272, 185);
	display.print(F("-"));

	display.setTextSize(3); // Decrease text size to 3

	display.fillRect(0, 0, 80, 20, 0xFFFFF); // Create rectangle for back button and fill in white
	display.setTextColor(0x00000); // Set text colour to black
	display.setCursor(5, 0); // Set the cursor to write text to the display
	display.println(F("BACK")); // Print button label
}

void TimeDisplay::displayTime(Clock& clock, MCUFRIEND_kbv& display) {
	display.setTextColor(0xFFFFF, 0x0000); // Set tetx colour to white
	display.setTextSize(7); // Increase text size to 7
	display.setCursor(95, 100); // Set the cursor to write text to the display
	printTime(clock, display);
}

bool TimeDisplay::update(CoreComponents& components, MCUFRIEND_kbv& display, SystemFunction function) {
    displayTime(components.getClock(), display);
}

SystemFunction TimeDisplay::getTouchInput(TSPoint& point) {
	if (point.z > MIN_PRESSURE && point.z < MAX_PRESSURE) {
		Serial.print("x: ");
		Serial.print(point.x);
		Serial.print(", y: ");
		Serial.println(point.y);
		if (point.x >= 820 && point.x <= 890 && point.y >= 200 && point.y <= 320) { // Back button
			return SystemFunction::MAIN_DISPLAY;
		}
		if (point.x >= 770 && point.x <= 830 && point.y >= 375 && point.y <= 420) {
			return SystemFunction::INCREASE_TENS_HOURS; // Increase 10 hours
		}
		if (point.x >= 280 && point.x <= 355 && point.y >= 377 && point.y <= 430) {
			return SystemFunction::DECREASE_TENS_HOURS; // Decrease 10 hours
		}
		if (point.x >= 760 && point.x <= 830 && point.y >= 460 && point.y <= 500) {
			return SystemFunction::INCREASE_HOURS; // Increase 1 hour
		}
		if (point.x >= 270 && point.x <= 350 && point.y >= 465 && point.y <= 515) {
			return SystemFunction::DECREASE_HOURS; // Decrease 1 hour
		}
		if (point.x >= 750 && point.x <= 815 && point.y >= 628 && point.y <= 665) {
			return SystemFunction::INCREASE_TENS_MINUTES; // Increase 10 minutes
		}
		if (point.x >= 285 && point.x <= 350 && point.y >= 625 && point.y <= 675) {
			return SystemFunction::DECREASE_TENS_MINUTES; // Decrease 10 minutes
		}
		if (point.x >= 755 && point.x <= 830 && point.y >= 715 && point.y <= 765) {
			return SystemFunction::INCREASE_MINUTES; // Increase 1 minute
		}
		if (point.x >= 280 && point.x <= 350 && point.y >= 720 && point.y <= 770) {
			return SystemFunction::DECREASE_MINUTES; // Decrease 1 minute
		}
	}
	return SystemFunction::NONE;
}