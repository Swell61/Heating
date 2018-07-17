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

void Display::mainDisplay(int time, byte heatingMode, byte waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost) {
	tft.fillScreen(0x00000);
	tft.setTextSize(2);
	tft.setTextColor(0xFFFFF);


	tft.fillRect(200, 70, 75, 35, heatingBoost ? 0x7CFC0 : 0xFF000);
	tft.fillRect(200, 145, 75, 35, waterBoost ? 0x7CFC0 : 0xFF000);
	tft.setCursor(209, 80);
	tft.println("BOOST");
	tft.setCursor(209, 155);
	tft.println("BOOST");

	if (heatingMode == 0) {
		tft.fillRect(100, 70, 80, 35, 0xFF000);
		tft.setCursor(120, 80);
		tft.println("OFF");
	}
	else if (heatingMode == 1) {
		tft.fillRect(100, 70, 80, 35, 0xff951c);
		tft.setCursor(110, 80);
		tft.println("TIMER");
	}
	else if (heatingMode == 2) {
		tft.fillRect(100, 70, 80, 35, 0x7CFC0);
		tft.setCursor(125, 80);
		tft.println("ON");
	}
	
	if (waterMode == 0) {
		tft.fillRect(100, 145, 80, 35, 0xFF000);
		tft.setCursor(120, 155);
		tft.println("OFF");
	}
	else if (waterMode == 1) {
		tft.fillRect(100, 145, 80, 35, 0xff951c);
		tft.setCursor(110, 155);
		tft.println("TIMER");
	}
	else if (waterMode == 2) {
		tft.fillRect(100, 145, 80, 35, 0x7CFC0);
		tft.setCursor(125, 155);
		tft.println("ON");
	}

	

	tft.setCursor(0, 80);
	tft.println("HEATING:");
	tft.setCursor(0, 155);
	tft.println("WATER:");

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
	tft.setCursor(360, 20);
	tft.print((String)(waterStatus ? "ON " : "OFF"));

	tft.fillTriangle(340, 70, 290, 90, 390, 90, 0xFFFFF);
	tft.fillTriangle(340, 185, 290, 165, 390, 165, 0xFFFFF);

	tft.fillRect(290, 205, 100, 30, 0xFFFFF);
	tft.setTextColor(0x00000);
	tft.setCursor(310, 213);
	tft.println("TIMER");

	tft.setTextColor(0xFFFFF);
	

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
void Display::displayUpdate(int time, byte heatingMode, byte waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost) {
	tft.setTextSize(2);
	tft.setTextColor(0xFFFFF);

	tft.fillRect(200, 70, 75, 35, heatingBoost ? 0x7CFC0 : 0xFF000);
	tft.fillRect(200, 145, 75, 35, waterBoost ? 0x7CFC0 : 0xFF000);
	tft.setCursor(209, 80);
	tft.println("BOOST");
	tft.setCursor(209, 155);
	tft.println("BOOST");

	if (heatingMode == 0) {
		tft.fillRect(100, 70, 80, 35, 0xFF000);
		tft.setCursor(120, 80);
		tft.println("OFF");
	}
	else if (heatingMode == 1) {
		tft.fillRect(100, 70, 80, 35, 0xff951c);
		tft.setCursor(110, 80);
		tft.println("TIMER");
	}
	else if (heatingMode == 2) {
		tft.fillRect(100, 70, 80, 35, 0x7CFC0);
		tft.setCursor(125, 80);
		tft.println("ON");
	}
	if (waterMode == 0) {
		tft.fillRect(100, 145, 80, 35, 0xFF000);
		tft.setCursor(120, 155);
		tft.println("OFF");
	}
	else if (waterMode == 1) {
		tft.fillRect(100, 145, 80, 35, 0xff951c);
		tft.setCursor(110, 155);
		tft.println("TIMER");
	}
	else if (waterMode == 2) {
		tft.fillRect(100, 145, 80, 35, 0x7CFC0);
		tft.setCursor(125, 155);
		tft.println("ON");
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

byte Display::touchUpdate(byte screen) {
  
#define MIN_PRESSURE 200
#define MAX_PRESSURE 1000
TSPoint tp = ts.getPoint();
pinMode(XM, OUTPUT);
pinMode(YP, OUTPUT);
if (tp.z > MIN_PRESSURE && tp.z < MAX_PRESSURE) {
if (screen == 0) {

	
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
		else if (tp.x >= 190 && tp.x <= 290 && tp.y >= 680 && tp.y <= 920) {
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
		else if (tp.x >= 190 && tp.x <= 250 && tp.y >= 200 && tp.y <= 350) {
						return 27;
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
else if (screen == 2) {
	if (tp.x >= 820 && tp.x <= 890 && tp.y >= 200 && tp.y <= 320) {
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

void Display::editTime(int time) {
	int minutes;
	int hours;

	hours = time / 60;
	minutes = time % 60;

	tft.fillScreen(0x00000);
	tft.setTextColor(0xFFFFF);
	tft.setTextSize(7);
	tft.setCursor(95, 100);
	tft.println(((hours <= 9) ? "0" : "") + (String)(hours)+":" + ((minutes <= 9) ? "0" : "") + (String)(minutes));

	tft.setTextSize(4);

	tft.setCursor(100, 30);
	tft.print("+");
	tft.setCursor(145, 30);
	tft.print("+");
	tft.setCursor(227, 30);
	tft.print("+"); 
	tft.setCursor(272, 30);
	tft.print("+");

	tft.setCursor(100, 185);
	tft.print("-");
	tft.setCursor(145, 185);
	tft.print("-");
	tft.setCursor(227, 185);
	tft.print("-");
	tft.setCursor(272, 185);
	tft.print("-");

	tft.setTextSize(3);

	tft.fillRect(0, 0, 80, 20, 0xFFFFF);
	tft.setTextColor(0x00000);
	tft.setCursor(5, 0);
	tft.println("BACK");
}

void Display::updateEditTime(int time) {
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

void Display::loadingScreen(int tryNum) {
	tft.setTextColor(0xFFFFF, 0x00000);
	tft.setCursor(0, 0);
	tft.setTextSize(2);
	tft.println("Loading...");
	tft.println("Getting time from NTP server...  (" + (String)tryNum + "/5)");
	if (tryNum > 1) {
		tft.println("Check Ethernet cable and internet conectivity");
	}
}