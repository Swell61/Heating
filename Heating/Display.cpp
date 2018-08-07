// 
// 
// 

#include "Display.h"
const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
Display::Display() : ts(TouchScreen(XP, YP, XM, YM, 300)) { // Constructor. Initialises the touchscreen and the TFT
	tft.begin(0x65); // Initialise the TFT with it's address
	tft.setRotation(3); // Set the rotation to landscape no2
	tft.fillScreen(0x00000); // Fill the screen with black
};

void Display::mainDisplay(int time, byte heatingMode, byte waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost) { // Main display
	
	// This function sets up the TFT to diplsay the main system properties by creating rectangles and triangles for buttons and filling in descriptive text

	tft.fillScreen(0x00000); // Fill the screen with black
	tft.setTextSize(2); // Set the text size to 2
	tft.setTextColor(0xFFFFF); // Set the text colour to white


	tft.fillRect(200, 70, 75, 35, heatingBoost ? 0x7CFC0 : 0xFF000); // Create a rectangle for the heating boost button. If heating boost is current on, colour in green, otherwise colour in red
	tft.fillRect(200, 145, 75, 35, waterBoost ? 0x7CFC0 : 0xFF000); // Create a rectangle for the hot water boost button. If hot water boost is currently on, colour in gree, otherwise colour in red
	tft.setCursor(209, 80); // Set the cursor to write text to the TFT
	tft.println(F("BOOST")); // Write the buttons description
	tft.setCursor(209, 155); // Set the cursor to write text to the TFT
	tft.println(F("BOOST")); // Write the buttons description

	if (heatingMode == 0) { // If heating mode is set to off...
		tft.fillRect(100, 70, 80, 35, 0xFF000); // Create a rectangle for the heating mode button and fill in red
		tft.setCursor(120, 80); // Set the cursor to write text to the TFT
		tft.println(F("OFF")); // Write the buttons description
	}
	else if (heatingMode == 1) { // If heating mode is set to timer...
		tft.fillRect(100, 70, 80, 35, 0xff951c); // Create a rectangle for the heating mode button and fill in blue
		tft.setCursor(110, 80); // Set the cursor to write text to the TFT
		tft.println(F("TIMER")); // Write the buttons description
	}
	else if (heatingMode == 2) { // If heating mode is set to on...
		tft.fillRect(100, 70, 80, 35, 0x7CFC0); // Create a rectangle for the heating mode button and fill in green
		tft.setCursor(125, 80); // Set the cursor to write text to the TFT
		tft.println(F("ON")); // Write the buttons description
	}
	
	if (waterMode == 0) { // If hot water mode is set to off...
		tft.fillRect(100, 145, 80, 35, 0xFF000); // Create a rectangle for the hot water mode button and fill in red
		tft.setCursor(120, 155); // Set the cursor to write text to the TFT
		tft.println(F("OFF"));
	}
	else if (waterMode == 1) {
		tft.fillRect(100, 145, 80, 35, 0xff951c); // Create a rectangle for the hot water mode button and fill in blue
		tft.setCursor(110, 155); // Set the cursor to write text to the TFT
		tft.println(F("TIMER"));
	}
	else if (waterMode == 2) {
		tft.fillRect(100, 145, 80, 35, 0x7CFC0); // Create a rectangle for the hot water mode button and fill in green
		tft.setCursor(125, 155); // Set the cursor to write text to the TFT
		tft.println(F("ON")); // Write the buttons description
	}

	tft.setCursor(0, 80); // Set the cursor to write text to the TFT
	tft.println(F("HEATING:")); // Print label
	tft.setCursor(0, 155); // Set the cursor to write text to the TFT
	tft.println(F("WATER:")); // Print label

	tft.setCursor(0, 3); // Set the cursor to write text to the TFT
	tft.setTextColor(0xFFFFF, 0x00000); // Set text colour to white with background of text in black (overwrites existing text)
	tft.print(F("Current temp: ")); // Print label
	tft.setCursor(160, 3); // Set the cursor to write text to the TFT
	tft.print((String)temp); // Print the current temperature

	tft.setCursor(0, 20); // Set the cursor to write text to the TFT
	tft.println(F("Status: Heating - ")); //Print label
	tft.setCursor(215, 20); // Set the cursor to write text to the TFT
	tft.print((String)(heatingStatus ? "ON " : "OFF")); // If heating is on, print on. Otherwise, print off
	tft.print(" Water - "); // Print label
	tft.setCursor(360, 20); // Set the cursor to write text to the TFT
	tft.print((String)(waterStatus ? "ON " : "OFF")); // If hot water is on, print on. Otherwise, print off

	tft.fillTriangle(340, 70, 290, 90, 390, 90, 0xFFFFF); // Create triangle button for requested temperature adjust
	tft.fillTriangle(340, 185, 290, 165, 390, 165, 0xFFFFF); // Create triangle button for requested temperature adjust

	tft.fillRect(290, 205, 100, 30, 0xFFFFF); // Create a rectangle for the button to adjust the timer and fill in white
	tft.setTextColor(0x00000); // Set text colour to black
	tft.setCursor(310, 213); // Set the cursor to write text to the TFT
	tft.println(F("TIMER")); // Write the button label

	tft.setTextColor(0xFFFFF); // Set text colour back to white
	
	tft.setTextSize(3); // Increase the text size to 3
	tft.setCursor(295, 115); // Set the cursor to write text to the TFT
	tft.println(requestedTemp); // Print the requested temperature
	tft.setCursor(30, 210); // Set the cursor to write text to the TFT
	int minutes; // Create variable to store computed minutes
	int hours; // Create variable to store computed houes

	hours = time / 60; // Calculate number of hours
	minutes = time % 60; // Calculate number of minutes

	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes)); // Print the time
	
}
void Display::displayUpdate(int time, byte heatingMode, byte waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost) { // Main display update

	// Code in this method does the same as main display, overwriting what was originally there with the new status

	tft.setTextSize(2);
	tft.setTextColor(0xFFFFF);

	tft.fillRect(200, 70, 75, 35, heatingBoost ? 0x7CFC0 : 0xFF000);
	tft.fillRect(200, 145, 75, 35, waterBoost ? 0x7CFC0 : 0xFF000);
	tft.setCursor(209, 80);
	tft.println(F("BOOST"));
	tft.setCursor(209, 155);
	tft.println(F("BOOST"));

	if (heatingMode == 0) {
		tft.fillRect(100, 70, 80, 35, 0xFF000);
		tft.setCursor(120, 80);
		tft.println(F("OFF"));
	}
	else if (heatingMode == 1) {
		tft.fillRect(100, 70, 80, 35, 0xff951c);
		tft.setCursor(110, 80);
		tft.println(F("TIMER"));
	}
	else if (heatingMode == 2) {
		tft.fillRect(100, 70, 80, 35, 0x7CFC0);
		tft.setCursor(125, 80);
		tft.println(F("ON"));
	}
	if (waterMode == 0) {
		tft.fillRect(100, 145, 80, 35, 0xFF000);
		tft.setCursor(120, 155);
		tft.println(F("OFF"));
	}
	else if (waterMode == 1) {
		tft.fillRect(100, 145, 80, 35, 0xff951c);
		tft.setCursor(110, 155);
		tft.println(F("TIMER"));
	}
	else if (waterMode == 2) {
		tft.fillRect(100, 145, 80, 35, 0x7CFC0);
		tft.setCursor(125, 155);
		tft.println(F("ON"));
	}

	tft.setTextColor(0xFFFFF, 0x00000);
	tft.setCursor(160, 3);
	tft.print((String)temp);

	tft.setCursor(215, 20);
	tft.print((heatingStatus ? "ON " : "OFF"));
	tft.setCursor(360, 20);
	tft.print((waterStatus ? "ON " : "OFF"));

	tft.setTextSize(3);
	tft.setCursor(295, 115);
	tft.println(requestedTemp);
	tft.setCursor(30, 210);

	int minutes;
	int hours;

	hours = time / 60;
	minutes = time % 60;

	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));
}

byte Display::touchUpdate(byte screen) { // Function for checking the touchscreen
  
#define MIN_PRESSURE 200 // Define the minimum needed pressure
#define MAX_PRESSURE 1000 // Define the maximum needed pressure
TSPoint tp = ts.getPoint(); // Get the point currently being touched
pinMode(XM, OUTPUT); // Reset pin to output
pinMode(YP, OUTPUT); // reset pin to output
if (tp.z > MIN_PRESSURE && tp.z < MAX_PRESSURE) { // If the required pressur ehas been met...
if (screen == 0) { // If current on main display..

	
						if (tp.x >= 560 && tp.x <= 700 && tp.y >= 500 && tp.y <= 700) { // Heating boost button
						return 3;
		}
		else if (tp.x >= 330 && tp.x <= 480 && tp.y >= 500 && tp.y <= 700) { // Water boost button
						return 4;
		}
		else if (tp.x >= 600 && tp.x <= 693 && tp.y >= 700 && tp.y <= 900) { // Up 1 degree
						return 1;
		}
		else if (tp.x >= 325 && tp.x <= 400 && tp.y >= 700 && tp.y <= 900) { // Down 1 degree
						return 2;
		}
		else if (tp.x >= 190 && tp.x <= 290 && tp.y >= 680 && tp.y <= 920) { // Timer display button
						return 5;
		}
		else if (tp.x >= 570 && tp.x <= 700 && tp.y >= 320 && tp.y <= 500) {
			// Mode change
						return 25; // Change heating mode
		}
		else if (tp.x >= 330 && tp.x <= 470 && tp.y >= 320 && tp.y <= 500) {
			// Mode change
						return 26; // Change water mode
		}
		else if (tp.x >= 190 && tp.x <= 250 && tp.y >= 200 && tp.y <= 350) { // Edit time button
						return 27;
		}
	}
}
else if (screen == 1) { // If current on timer display
	if (tp.x >= 820 && tp.x <= 890 && tp.y >= 200 && tp.y <= 320) { // Back button
		return 6;
	}
	else if (tp.x >= 660 && tp.x <= 775 && tp.y >= 360 && tp.y <= 400) {
		return 7; // Decrease heating morning on by 5 minutes
	}
	else if (tp.x >= 660 && tp.x <= 775 && tp.y >= 540 && tp.y <= 620) {
		return 8; // Increase heating morning on by 5 minutes
	}
	else if (tp.x >= 530 && tp.x <= 610 && tp.y >= 360 && tp.y <= 400) {
		return 9; // Decrease heating morning off by 5 minutes
	}
	else if (tp.x >= 530 && tp.x <= 610 && tp.y >= 540 && tp.y <= 620) {
		return 10; // Increase heating morning off by 5 minutes
	}
	else if (tp.x >= 375 && tp.x <= 470 && tp.y >= 360 && tp.y <= 400) {
		return 11; // Decrease heating afternoon on by 5 minutes
	}
	else if (tp.x >= 375 && tp.x <= 470 && tp.y >= 540 && tp.y <= 620) {
				return 12; // Increase heating afternoon on by 5 minutes
	}
	else if (tp.x >= 215 && tp.x <= 310 && tp.y >= 360 && tp.y <= 400) {
				return 13; // Decrease heating afternoon off by 5 minutes
	}
	else if (tp.x >= 215 && tp.x <= 310 && tp.y >= 540 && tp.y <= 620) {
		return 14; // Increase heating afternoon off by 5 minutes
	}

	else if (tp.x >= 685 && tp.x <= 778 && tp.y >= 650 && tp.y <= 730) {
		return 15; // Decrease water morning on by 5 minutes
	}
	else if (tp.x >= 685 && tp.x <= 778 && tp.y >= 850 && tp.y <= 910) {
		return 16; // Increase water morning on by 5 minutes
	}
	else if (tp.x >= 515 && tp.x <= 620 && tp.y >= 650 && tp.y <= 730) {
		return 17; // Decrease water morning off by 5 minutes
	}
	else if (tp.x >= 515 && tp.x <= 620 && tp.y >= 850 && tp.y <= 910) {
		return 18; // Increase water morning off by 5 minutes
	}
	else if (tp.x >= 375 && tp.x <= 470 && tp.y >= 650 && tp.y <= 730) {
		return 19; // Decrease water afternoon on by 5 minutes
	}
	else if (tp.x >= 375 && tp.x <= 470 && tp.y >= 850 && tp.y <= 910) {
		return 20; // Increase water afternoon on by 5 minutes
	}
	else if (tp.x >= 215 && tp.x <= 310 && tp.y >= 650 && tp.y <= 730) {
		return 21; // Decrease water afternoon off by 5 minutes
	}
	else if (tp.x >= 215 && tp.x <= 310 && tp.y >= 850 && tp.y <= 910) {
		return 22; // Increase water afternoon off by 5 minutes
	}
	
}
else if (screen == 2) { // If currently on time edit button
	if (tp.x >= 820 && tp.x <= 890 && tp.y >= 200 && tp.y <= 320) { // Back button
		return 6;
	}
	if (tp.x >= 715 && tp.x <= 800 && tp.y >= 320 && tp.y <= 380) {
		return 28; // Increase 10 hours
	}
	if (tp.x >= 240 && tp.x <= 340 && tp.y >= 320 && tp.y <= 380) {
		return 29; // Decrease 10 hours
	}
	if (tp.x >= 715 && tp.x <= 800 && tp.y >= 400 && tp.y <= 480) {
		return 30; // Increase 1 hour
	}
	if (tp.x >= 240 && tp.x <= 340 && tp.y >= 400 && tp.y <= 480) {
		return 31; // Decrease 1 hour
	}
	if (tp.x >= 715 && tp.x <= 800 && tp.y >= 570 && tp.y <= 645) {
		return 32; // Increase 10 minutes
	}
	if (tp.x >= 240 && tp.x <= 340 && tp.y >= 570 && tp.y <= 645) {
		return 33; // Decrease 10 minutes
	}
	if (tp.x >= 715 && tp.x <= 800 && tp.y >= 665 && tp.y <= 735) {
		return 34; // Increase 1 minute
	}
	if (tp.x >= 240 && tp.x <= 340 && tp.y >= 665 && tp.y <= 735) {
		return 35; // Decrease 1 minute
	}
}
return 0; // else if no touch registered (pressure requirements not met), return 0
}

void Display::timerDisplay(bool heatingTimerStatus, bool waterTimerStatus, int heatingOnMorning, int heatingOffMorning, int heatingOnAfternoon, int heatingOffAfternoon, int waterOnMorning, int waterOffMorning, int waterOnAfternoon, int waterOffAfternoon) { // Timer display
	int minutes; // Create variable for storing calculated mintues
	int hours; // Create variable for storing calculated hours
	
	tft.fillScreen(0x00000); // Fill the screen with black
	tft.setTextSize(3); // Increase text size to 3
	
	tft.fillRect(0, 0, 80, 20, 0xFFFFF); // Fill a rectangle for the back button
	tft.setTextColor(0x00000); // Set text colour to black
	tft.setCursor(5, 0); // Set the cursor to write text to the TFT
	tft.println(F("BACK")); // Print the button label

	tft.fillRect(145, 5, 56, 25, heatingTimerStatus ? 0x7CFC0 : 0xFF000); // Create rectangle for heating timer status. If heating timer is on, fill green, otherwise fill red
	tft.fillRect(295, 5, 68, 25, waterTimerStatus ? 0x7CFC0 : 0xFF000); // Create rectangle for hot water timer status. If hot water timer is on, fill green, otherwise fill red
	tft.setTextSize(2); // Decrease text size to 2
	tft.setTextColor(0xFFFFF); // Set text colour to white
	tft.setCursor(150, 10); // Set the cursor to write text to the TFT
	tft.println(F("HEAT")); // Print label
	tft.setCursor(300, 10); // Set the cursor to write text to the TFT
	tft.println(F("WATER")); // Print label

	// Print morning on for heating
	tft.setCursor(145, 50);
	hours = (heatingOnMorning / 60);
	minutes = (heatingOnMorning % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours) + ":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print morning off for heating
	tft.setCursor(145, 100);
	hours = (heatingOffMorning / 60);
	minutes = (heatingOffMorning % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+ ":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print afternoon on for heating
	tft.setCursor(145, 150);
	hours = (heatingOnAfternoon / 60);
	minutes = (heatingOnAfternoon % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+ ":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print afternoon off for heating
	tft.setCursor(145, 200);
	hours = (heatingOffAfternoon / 60);
	minutes = (heatingOffAfternoon % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+ ":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));


	// Print morning on for water
	tft.setCursor(300, 50);
	hours = (waterOnMorning / 60);
	minutes = (waterOnMorning % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+ ":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print morning off for water
	tft.setCursor(300, 100);
	hours = (waterOffMorning / 60);
	minutes = (waterOffMorning % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+ ":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print afternoon on for water
	tft.setCursor(300, 150);
	hours = (waterOnAfternoon / 60);
	minutes = (waterOnAfternoon % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+ ":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print afternoon off for water
	tft.setCursor(300, 200);
	hours = (waterOffAfternoon / 60);
	minutes = (waterOffAfternoon % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+ ":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print the plus and minus buttons for heating
	int y = 50;
	for (int count = 0; count <= 3; count++) {
		tft.setCursor(120, y);
		tft.print(F("-"));
		tft.setCursor(215, y);
		tft.print(F("+"));
		y = y + 50;
	}

	// Print the plus and minus buttins for hot water
	y = 50;
	for (int count = 0; count <= 3; count++) {
		tft.setCursor(275, y);
		tft.print("-");
		tft.setCursor(370, y);
		tft.print(F("+"));
		y = y + 50;
	}

	// Print the labels for each time
	tft.setTextSize(1);
	tft.setCursor(0, 50);
	tft.println(F("Morning on:"));
	tft.setCursor(0, 100);
	tft.println(F("Morning off:"));
	tft.setCursor(0, 150);
	tft.println(F("Afternoon on:"));
	tft.setCursor(0, 200);
	tft.println(F("Afternoon off:"));
}

void Display::timerUpdate(bool heatingTimerStatus, bool waterTimerStatus, int heatingOnMorning, int heatingOffMorning, int heatingOnAfternoon, int heatingOffAfternoon, int waterOnMorning, int waterOffMorning, int waterOnAfternoon, int waterOffAfternoon) { // Function for updating the timer screen

	// The code in this function does what timerDisplay does, but it overwrites with the new status

	int minutes;
	int hours;

	tft.setTextSize(3);

	tft.fillRect(145, 5, 56, 25, heatingTimerStatus ? 0x7CFC0 : 0xFF000);
	tft.fillRect(295, 5, 68, 25, waterTimerStatus ? 0x7CFC0 : 0xFF000);
	tft.setTextSize(2);
	tft.setTextColor(0xFFFFF);
	tft.setCursor(150, 10);
	tft.println(F("HEAT"));
	tft.setCursor(300, 10);
	tft.println(F("WATER"));

	tft.setTextSize(2);
	tft.setTextColor(0xFFFFF, 0x00000);
	// Print morning on for heating
	tft.setCursor(145, 50);
	hours = (heatingOnMorning / 60);
	minutes = (heatingOnMorning % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print morning off for heating
	tft.setCursor(145, 100);
	hours = (heatingOffMorning / 60);
	minutes = (heatingOffMorning % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print afternoon on for heating
	tft.setCursor(145, 150);
	hours = (heatingOnAfternoon / 60);
	minutes = (heatingOnAfternoon % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print afternoon off for heating
	tft.setCursor(145, 200);
	hours = (heatingOffAfternoon / 60);
	minutes = (heatingOffAfternoon % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));


	// Print morning on for water
	tft.setCursor(300, 50);
	hours = (waterOnMorning / 60);
	minutes = (waterOnMorning % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print morning off for water
	tft.setCursor(300, 100);
	hours = (waterOffMorning / 60);
	minutes = (waterOffMorning % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print afternoon on for water
	tft.setCursor(300, 150);
	hours = (waterOnAfternoon / 60);
	minutes = (waterOnAfternoon % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	// Print afternoon off for water
	tft.setCursor(300, 200);
	hours = (waterOffAfternoon / 60);
	minutes = (waterOffAfternoon % 60);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));
}

void Display::editTime(int time) { // Function for displaying the time update screen
	int minutes; // Create variable to store calculated minutes
	int hours; // Create variable to store calculated hours

	hours = time / 60; // Calculate hours
	minutes = time % 60; // Calculate minutes

	tft.fillScreen(0x00000); // Fill the screen black
	tft.setTextColor(0xFFFFF); // Set tetx colour to white
	tft.setTextSize(7); // Increase text size to 7
	tft.setCursor(95, 100); // Set the cursor to write text to the TFT
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes)); // Print the current time

	tft.setTextSize(4); // Decrease text size to 4

	// Print the plus and minus adjust buttons
	tft.setCursor(100, 30);
	tft.print(F("+"));
	tft.setCursor(145, 30);
	tft.print(F("+"));
	tft.setCursor(227, 30);
	tft.print(F("+")); 
	tft.setCursor(272, 30);
	tft.print(F("+"));

	tft.setCursor(100, 185);
	tft.print(F("-"));
	tft.setCursor(145, 185);
	tft.print(F("-"));
	tft.setCursor(227, 185);
	tft.print(F("-"));
	tft.setCursor(272, 185);
	tft.print(F("-"));

	tft.setTextSize(3); // Decrease text size to 3

	tft.fillRect(0, 0, 80, 20, 0xFFFFF); // Create rectangle for back button and fill in white
	tft.setTextColor(0x00000); // Set text colour to black
	tft.setCursor(5, 0); // Set the cursor to write text to the TFT
	tft.println(F("BACK")); // Print button label
}

void Display::updateEditTime(int time) { // Function for updating time edit screen

	// The code in this function does the same as timeEdit but it overwrites it with the current status

	int minutes;
	int hours;

	hours = time / 60;
	minutes = time % 60;

	tft.setTextColor(0xFFFFF, 0x00000);
	tft.setTextSize(7);
	tft.setCursor(95, 100);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	tft.setTextSize(3);
}

void Display::loadingScreen(bool SDAvailable, int tryNum) { // Function for displaying the loading screen
	tft.setTextColor(0xFFFFF, 0x00000); // Set the text colour to white and background to black
	tft.setCursor(0, 0); // Set the cursor to write text to the TFT
	tft.setTextSize(2); // Set text size to 2
	tft.println(F("Loading...")); // Print description of what is happening
	tft.print(F("SD "));
	tft.println(SDAvailable ? "UP" : "DOWN");
	tft.println("Getting time from NTP server...  (" + (String)tryNum + "/5)"); // Print current number of tries to get time from NTP server
	if (tryNum > 1) { // If it has taken more than one try...
		tft.println(F("Check Ethernet cable and internet conectivity")); // ...ask the user to check possible faults
	}
}