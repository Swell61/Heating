// 
// 
// 

#include "Display.h"
const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
Display::Display() : ts(TouchScreen(XP, YP, XM, YM, 300)) {
	tft.begin(0x65);
	tft.setRotation(3);
	tft.fillScreen(0x00000);
};

void Display::mainDisplay(float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost) {
	tft.fillScreen(0x00000);
	tft.setTextSize(2);
	tft.setTextColor(0xFFFFF);
	tft.fillRect(40, 70, 105, 35, heatingBoost ? 0x7CFC0 : 0xFF000);
	tft.fillRect(40, 145, 105, 35, waterBoost ? 0x7CFC0 : 0xFF000);
	tft.setCursor(50, 80);
	tft.println("HEATING");
	tft.setCursor(62, 155);
	tft.println("WATER");

	tft.setCursor(0, 3);
	tft.setTextColor(0xFFFFF, 0x00000);
	tft.print("Current temp: ");
	tft.setCursor(160, 3);
	tft.print((String)temp);

	tft.setCursor(0, 20);
	tft.println("Status: Heating - ");
	tft.setCursor(215, 20);
	tft.print((String)(heatingStatus ? "ON " : "OFF"));
	tft.print(" Water - ");
	tft.setCursor(350, 20);
	tft.print((String)(waterStatus ? "ON " : "OFF"));

	tft.fillTriangle(300, 70, 250, 90, 350, 90, 0xFFFFF);
	tft.fillTriangle(300, 185, 250, 165, 350, 165, 0xFFFFF);

	tft.fillRect(150, 205, 100, 30, 0xFFFFF);
	tft.setTextColor(0x00000);
	tft.setCursor(170, 213);
	tft.println("TIMER");

	tft.setTextSize(3);
	tft.setCursor(255, 115);
	tft.println(requestedTemp);
}
void Display::displayUpdate(float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost) {
	tft.setTextSize(2);
	tft.setTextColor(0xFFFFF);
	tft.fillRect(40, 70, 105, 35, heatingBoost ? 0x7CFC0 : 0xFF000);
	tft.fillRect(40, 145, 105, 35, waterBoost ? 0x7CFC0 : 0xFF000);
	tft.setCursor(50, 80);
	tft.println("HEATING");
	tft.setCursor(62, 155);
	tft.println("WATER");

	tft.setTextColor(0xFFFFF, 0x00000);
	tft.setCursor(160, 3);
	tft.print((String)temp);

	tft.setCursor(215, 20);
	tft.print((heatingStatus ? "ON " : "OFF"));
	tft.setCursor(350, 20);
	tft.print((waterStatus ? "ON " : "OFF"));

	tft.setTextSize(3);
	tft.setCursor(255, 115);
	tft.println(requestedTemp);
}

byte Display::touchUpdate(byte screen) {
  
#define MIN_PRESSURE 200
#define MAX_PRESSURE 1000
TSPoint tp = ts.getPoint();
pinMode(XM, OUTPUT);
pinMode(YP, OUTPUT);
if (tp.z > MIN_PRESSURE && tp.z < MAX_PRESSURE) {
if (screen == 0) {

	
		Serial.println("Press");
		Serial.println(String(tp.x) + " " + String(tp.y));
		if (tp.x >= 560 && tp.x <= 700 && tp.y >= 220 && tp.y <= 420) { // Heating boost button
			Serial.println("Heating boost");
			return 3;
		}
		else if (tp.x >= 330 && tp.x <= 480 && tp.y >= 220 && tp.y <= 420) { // Water boost button
			Serial.println("Water boost");
			return 4;
		}
		else if (tp.x >= 600 && tp.x <= 693 && tp.y >= 630 && tp.y <= 830) { // Up 1 degree
			Serial.println("Up 1 degree");
			return 1;
		}
		else if (tp.x >= 325 && tp.x <= 400 && tp.y >= 630 && tp.y <= 830) { // Down 1 degree
			Serial.println("Down 1 degree");
			return 2;
		}
		else if (tp.x >= 220 && tp.x <= 290 && tp.y >= 400 && tp.y <= 630) {
			Serial.println("Switch to timer display");
			return 5;
		}
	}
}
else if (screen == 1) {
	if (tp.x >= 820 && tp.x <= 890 && tp.y >= 200 && tp.y <= 320) {
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
	else if (tp.x >= 660 && tp.x <= 775 && tp.y >= 360 && tp.y <= 400) {
		return 11; // Decrease heating afternoon on by 5 minutes
	}
	else if (tp.x >= 660 && tp.x <= 775 && tp.y >= 540 && tp.y <= 620) {
		return 12; // Increase heating afternoon on by 5 minutes
	}
	else if (tp.x >= 530 && tp.x <= 610 && tp.y >= 360 && tp.y <= 400) {
		return 13; // Decrease heating afternoon off by 5 minutes
	}
	else if (tp.x >= 530 && tp.x <= 610 && tp.y >= 540 && tp.y <= 620) {
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
	else if (tp.x >= 780 && tp.x <= 890 && tp.y >= 400 && tp.y <= 550) {
		Serial.print("Heating timer pressed");
		return 23; // Toggle heating timer
	}
	else if (tp.x >= 780 && tp.x <= 890 && tp.y >= 700 && tp.y <= 870) {
		return 24; // Toggle water timer
	}
}
return 0;
}

void Display::timerDisplay(bool heatingTimerStatus, bool waterTimerStatus, int heatingOnMorning, int heatingOffMorning, int heatingOnAfternoon, int heatingOffAfternoon, int waterOnMorning, int waterOffMorning, int waterOnAfternoon, int waterOffAfternoon) {
	int minutes;
	int hours;
	
	tft.fillScreen(0x00000);
	tft.setTextSize(3);
	
	tft.fillRect(0, 0, 80, 20, 0xFFFFF);
	tft.setTextColor(0x00000);
	tft.setCursor(5, 0);
	tft.println("BACK");

	tft.fillRect(145, 5, 56, 25, heatingTimerStatus ? 0x7CFC0 : 0xFF000);
	tft.fillRect(295, 5, 68, 25, waterTimerStatus ? 0x7CFC0 : 0xFF000);
	tft.setTextSize(2);
	tft.setTextColor(0xFFFFF);
	tft.setCursor(150, 10);
	tft.println("HEAT");
	tft.setCursor(300, 10);
	tft.println("WATER");

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

	int y = 50;
	for (int count = 0; count <= 3; count++) {
		tft.setCursor(120, y);
		tft.print("-");
		tft.setCursor(215, y);
		tft.print("+");
		y = y + 50;
	}

	y = 50;
	for (int count = 0; count <= 3; count++) {
		tft.setCursor(275, y);
		tft.print("-");
		tft.setCursor(370, y);
		tft.print("+");
		y = y + 50;
	}

	tft.setTextSize(1);
	tft.setCursor(0, 50);
	tft.println("Morning on:");
	tft.setCursor(0, 100);
	tft.println("Morning off:");
	tft.setCursor(0, 150);
	tft.println("Afternoon on:");
	tft.setCursor(0, 200);
	tft.println("Afternoon off:");
}

void Display::timerUpdate(bool heatingTimerStatus, bool waterTimerStatus, int heatingOnMorning, int heatingOffMorning, int heatingOnAfternoon, int heatingOffAfternoon, int waterOnMorning, int waterOffMorning, int waterOnAfternoon, int waterOffAfternoon) {
	int minutes;
	int hours;

	tft.setTextSize(3);

	tft.fillRect(145, 5, 56, 25, heatingTimerStatus ? 0x7CFC0 : 0xFF000);
	tft.fillRect(295, 5, 68, 25, waterTimerStatus ? 0x7CFC0 : 0xFF000);
	tft.setTextSize(2);
	tft.setTextColor(0xFFFFF);
	tft.setCursor(150, 10);
	tft.println("HEAT");
	tft.setCursor(300, 10);
	tft.println("WATER");

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