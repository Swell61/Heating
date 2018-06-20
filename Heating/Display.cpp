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

byte Display::touchUpdate() {
  
#define MIN_PRESSURE 200
#define MAX_PRESSURE 1000
TSPoint tp = ts.getPoint();
pinMode(XM, OUTPUT);
pinMode(YP, OUTPUT);


if (tp.z > MIN_PRESSURE && tp.z < MAX_PRESSURE) {
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
}
return 0;

  
}