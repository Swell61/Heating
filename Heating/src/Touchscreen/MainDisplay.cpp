#include "MainDisplay.h"

bool MainDisplay::display(CoreComponents& components, MCUFRIEND_kbv& display) { // Main display
	
	// This function sets up the display to diplsay the main system properties by creating rectangles and triangles for buttons and filling in descriptive text
    
	display.fillScreen(0x00000); // Fill the screen with black
	printLabels(display);

	display.setTextSize(2);
	display.setTextColor(0xFFFFF); // Set the text colour to white

	printHeatingBoostButton(components.getComponentControl().getHeating(), components.getClock(), display);
	printWaterBoostButton(components.getComponentControl().getWater(), components.getClock(), display);

	printHeatingMode(components.getComponentControl().getHeating(), display);
    printWaterMode(components.getComponentControl().getWater(), display);
	
	
	printTemperature(components.getTempSensor(), display);

	
	printHeatingRequired(components.getComponentControl().getHeating(), components.getTempSensor(), components.getClock(), display);
	printHeatingStatus(components.getComponentControl().getHeating(), components.getTempSensor(), components.getClock(), display);
	printWaterRequired(components.getComponentControl().getWater(), components.getClock(), display);
	printWaterStatus(components.getComponentControl().getWater(), components.getClock(), display);

	printRequestedTemp(components.getComponentControl().getHeating(), display);
	
	printTime(components.getClock(), display);
}

void MainDisplay::printTime(Clock& clock, MCUFRIEND_kbv& display) {
	display.setCursor(0, 215);
	display.setTextColor(0xFFFFF);
	display.setTextSize(3);
	Screen::printTime(clock, display);
}

void MainDisplay::printLabels(MCUFRIEND_kbv& display) {

	display.fillRect(290, 205, 100, 30, 0xFFFFF); // Create a rectangle for the button to adjust the timer and fill in white
	display.setTextColor(0x00000); // Set text colour to black
	display.setCursor(310, 213); // Set the cursor to write text to the display
	display.println(F("TIMER")); // Write the button label

	display.setTextColor(0xFFFFF);
	display.setTextSize(2);
	display.setCursor(0, 80); // Set the cursor to write text to the display
	display.println(F("HEATING:")); // Print label
	display.setCursor(0, 155); // Set the cursor to write text to the display
	display.println(F("WATER:")); // Print label

	display.setCursor(0, 3); // Set the cursor to write text to the display
	display.setTextColor(0xFFFFF, 0x00000); // Set text colour to white with background of text in black (overwrites existing text)
	display.print(F("Current temp: ")); // Print label

	display.setCursor(0, 20); // Set the cursor to write text to the display
	display.println(F("Status: Heating - ")); //Print label

	display.setCursor(260, 20);
	display.print("Water - "); // Print label

	display.fillTriangle(340, 70, 290, 90, 390, 90, 0xFFFFF); // Create triangle button for requested temperature adjust
	display.fillTriangle(340, 185, 290, 165, 390, 165, 0xFFFFF); // Create triangle button for requested temperature adjust


}

void MainDisplay::printHeatingBoostButton(Heating& heating, Clock& clock, MCUFRIEND_kbv& display) {
	display.setTextColor(0xFFFFF);
	display.setTextSize(2);
	display.fillRect(200, 70, 75, 35, heating.getBoost().checkState(clock) == State::ON ? 0x7CFC0 : 0xFF000); // Create a rectangle for the heating boost button. If heating boost is current on, colour in green, otherwise colour in red
	display.setCursor(209, 80); // Set the cursor to write text to the display
	display.println(F("BOOST")); // Write the buttons description
}

void MainDisplay::printWaterBoostButton(Water& water, Clock& clock, MCUFRIEND_kbv& display) {
	display.setTextColor(0xFFFFF);
	display.setTextSize(2);
	display.fillRect(200, 145, 75, 35, water.getBoost().checkState(clock) == State::ON ? 0x7CFC0 : 0xFF000); // Create a rectangle for the hot water boost button. If hot water boost is currently on, colour in gree, otherwise colour in red
	display.setCursor(209, 155); // Set the cursor to write text to the display
	display.println(F("BOOST")); // Write the buttons description
}

bool MainDisplay::update(CoreComponents& components, MCUFRIEND_kbv& display, SystemFunction function) { // Main display update

	// Code in this method does the same as main display, overwriting what was originally there with the new status

	printRequestedTemp(components.getComponentControl().getHeating(), display);

	display.setTextSize(2);
	display.setTextColor(0xFFFFF);

	printHeatingBoostButton(components.getComponentControl().getHeating(), components.getClock(), display);
	printWaterBoostButton(components.getComponentControl().getWater(), components.getClock(), display);

	printHeatingMode(components.getComponentControl().getHeating(), display);
    printWaterMode(components.getComponentControl().getWater(), display);

	display.setTextColor(0xFFFFF, 0x00000);
	display.setCursor(160, 3);
	display.print(components.getTempSensor().getTemp());

	printHeatingRequired(components.getComponentControl().getHeating(), components.getTempSensor(), components.getClock(), display);
	printWaterRequired(components.getComponentControl().getWater(), components.getClock(), display);	
    printTime(components.getClock(), display);

	return true;
}

void MainDisplay::printHeatingRequired(Heating& heating, TempSensor& tempSensor, Clock& clock, MCUFRIEND_kbv& display) {
	display.setCursor(215, 20); // Set the cursor to write text to the display
	display.setTextColor(0xFFFFF);
	display.setTextSize(2);
	display.print((heating.required(clock, tempSensor) ? "ON " : "OFF")); // If heating is on, print on. Otherwise, print off
}

void MainDisplay::printWaterRequired(Water& water, Clock& clock, MCUFRIEND_kbv& display) {
	display.setCursor(360, 20); // Set the cursor to write text to the display
	display.setTextColor(0xFFFFF);
	display.setTextSize(2);
	display.print((water.required(clock) ? "ON " : "OFF")); // If hot water is on, print on. Otherwise, print off
}

void MainDisplay::printRequestedTemp(Heating& heating, MCUFRIEND_kbv& display) {
	display.setTextSize(3); // Increase the text size to 3
	display.setCursor(295, 115); // Set the cursor to write text to the display
	display.setTextColor(0xFFFFF);
	display.println(heating.getRequestedTemp()); // Print the requested temperature
}

void MainDisplay::printHeatingStatus(Heating& heating, TempSensor& tempSensor, Clock& clock, MCUFRIEND_kbv& display) {
	display.setCursor(215, 20); // Set the cursor to write text to the display
	display.setTextColor(0xFFFFF);
	display.setTextSize(2);
	display.print(heating.required(clock, tempSensor) ? "ON " : "OFF"); // If heating is on, print on. Otherwise, print off
}

void MainDisplay::printWaterStatus(Water& water, Clock& clock, MCUFRIEND_kbv& display) {
	display.setCursor(360, 20); // Set the cursor to write text to the display
	display.setTextColor(0xFFFFF);
	display.setTextSize(2);
	display.print(water.required(clock) ? "ON " : "OFF"); // If hot water is on, print on. Otherwise, print off
}

void MainDisplay::printTemperature(TempSensor& tempSensor, MCUFRIEND_kbv& display) {
	display.setTextColor(0xFFFFF, 0x00000);
	display.setTextSize(2);
	display.setCursor(160, 3); // Set the cursor to write text to the display
	display.print(tempSensor.getTemp()); // Print the current temperature
}

void MainDisplay::printHeatingMode(Heating& heating, MCUFRIEND_kbv& display) {
	display.setTextColor(0xFFFFF);
	display.setTextSize(2);
    switch (heating.getMode()) {
        case Mode::OFF: {
            display.fillRect(100, 70, 80, 35, 0xFF000); // Create a rectangle for the heating mode button and fill in red
            display.setCursor(120, 80); // Set the cursor to write text to the display
            display.println(F("OFF")); // Write the buttons description
            break;
        }
        case Mode::TIMER: {
            display.fillRect(100, 70, 80, 35, 0xff951c); // Create a rectangle for the heating mode button and fill in blue
            display.setCursor(110, 80); // Set the cursor to write text to the display
            display.println(F("TIMER")); // Write the buttons description
            break;
        }
        case Mode::ON: {
            display.fillRect(100, 70, 80, 35, 0x7CFC0); // Create a rectangle for the heating mode button and fill in green
            display.setCursor(125, 80); // Set the cursor to write text to the display
            display.println(F("ON")); // Write the buttons description
            break;
        }
    }
}

void MainDisplay::printWaterMode(Water& water, MCUFRIEND_kbv& display) {
	display.setTextColor(0xFFFFF);
	display.setTextSize(2);
    switch (water.getMode()) {
        case Mode::OFF: {
            display.fillRect(100, 145, 80, 35, 0xFF000); // Create a rectangle for the hot water mode button and fill in red
            display.setCursor(120, 155); // Set the cursor to write text to the display
            display.println(F("OFF"));
	    }
	    case Mode::TIMER: {
            display.fillRect(100, 145, 80, 35, 0xff951c); // Create a rectangle for the hot water mode button and fill in blue
            display.setCursor(110, 155); // Set the cursor to write text to the display
            display.println(F("TIMER"));
	    }
	    case Mode::ON: {
            display.fillRect(100, 145, 80, 35, 0x7CFC0); // Create a rectangle for the hot water mode button and fill in green
            display.setCursor(125, 155); // Set the cursor to write text to the display
            display.println(F("ON")); // Write the buttons description  
	    }
    }
}

SystemFunction MainDisplay::getTouchInput(TouchScreen& touchScreen) {
	TSPoint tp = touchScreen.getPoint();
	if (tp.z > MIN_PRESSURE && tp.z < MAX_PRESSURE) {
		if (tp.x >= 560 && tp.x <= 700 && tp.y >= 500 && tp.y <= 700) { // Heating boost button
			return SystemFunction::HEATING_BOOST;
		}
		else if (tp.x >= 330 && tp.x <= 480 && tp.y >= 500 && tp.y <= 700) { // Water boost button
			return SystemFunction::WATER_BOOST;
		}
		else if (tp.x >= 600 && tp.x <= 693 && tp.y >= 700 && tp.y <= 900) { // Up 1 degree
			return SystemFunction::UP;
		}
		else if (tp.x >= 325 && tp.x <= 400 && tp.y >= 700 && tp.y <= 900) { // Down 1 degree
			return SystemFunction::DOWN;
		}
		else if (tp.x >= 190 && tp.x <= 290 && tp.y >= 680 && tp.y <= 920) { // Timer display button
			return SystemFunction::TIMER_DISPLAY;
		}
		else if (tp.x >= 570 && tp.x <= 700 && tp.y >= 320 && tp.y <= 500) { // Mode change
			return SystemFunction::HEATING_MODE;
		}
		else if (tp.x >= 330 && tp.x <= 470 && tp.y >= 320 && tp.y <= 500) {	// Mode change
			return SystemFunction::HOT_WATER_MODE;
		}
		else if (tp.x >= 190 && tp.x <= 250 && tp.y >= 200 && tp.y <= 350) { // Edit time button
			return SystemFunction::CURRENT_TIME_DISPLAY;
		}
	}
	return SystemFunction::NONE;
}