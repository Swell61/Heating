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
	tft.setTextSize(2);
	tft.setTextColor(0xFFFFF);
	tft.fillRect(40, 70, 105, 35, heatingBoost ? 0x7CFC00 : 0xFF0000);
	tft.fillRect(40, 145, 105, 35, heatingBoost ? 0x7CFC00 : 0xFF0000);
	tft.setCursor(50, 80);
	tft.println("HEATING");
	tft.setCursor(62, 155);
	tft.println("WATER");
	
	tft.setCursor(0, 3);
	tft.setTextColor(0xFFFFF, 0x00000);
	tft.println("Current temp: " + (String) temp);
	
	tft.setCursor(0, 20);
	tft.println("Status: Heating - " + (String)(heatingStatus ? "ON" : "OFF") + ", Water - " + (String)(waterStatus ? "ON" : "OFF"));

	tft.fillTriangle(300, 70, 250, 90, 350, 90, 0xFFFFF);
	tft.fillTriangle(300, 185, 250, 165, 350, 165, 0xFFFFF);

	tft.setTextSize(3);
	tft.setCursor(255, 115);
	tft.println(requestedTemp);
}

byte Display::touchUpdate() {
#define MIN_PRESSURE 200
#define MAX_PRESSURE 1000
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;
pinMode(XM, OUTPUT);
pinMode(YP, OUTPUT);
TSPoint tp = ts.getPoint();
	if (ts.getPoint().z > MIN_PRESSURE && ts.getPoint().z < MAX_PRESSURE) {
		xpos = map(ts.getPoint().x, TS_RT, TS_LEFT, 0, tft.width());
		ypos = map(ts.getPoint().y, TS_BOT, TS_TOP, 0, tft.height());
		Serial.println("Press");
		return 1;
		if (xpos <= 560 && xpos >= 222 && ypos <= 648 && ypos >= 548) { // Heating boost button
		return 3;
	}
	else if (xpos <= 560 && xpos >= 222 && ypos <= 339 && ypos >= 407) { // Water boost button
		return 4;
	}
	else if (xpos <= 653 && xpos >= 702 && ypos <= 661 && ypos >= 820) { // Up 1 degree
		return 1; 
	}
	else if (xpos <= 376 && xpos >= 436 && ypos <= 661 && ypos >= 820) { // Down 1 degree
		return 2;
	}
	}

	
}