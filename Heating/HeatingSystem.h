// HeatingSystem.h

#ifndef _HEATINGSYSTEM_h
#define _HEATINGSYSTEM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "WebInterface.h"
#include "Pump.h"
#include "Boiler.h"
#include "TempSensor.h"
#include "Display.h"
#include "Timer.h"
#include <TimeLib.h>
#include <Time.h>
#include <SD.h>
#include "Config.h"
#include <avr/wdt.h>
#include "HeatingFunction.h"
#include "ScreenTypes.h"
#include "ComponentOperatingMode.h"

// This class contains all components of the heating system

class HeatingSystem {

public:
	HeatingSystem(int boilerPinNum, int pumpPinNum, int tempSensorPinNum, int intTempSensorPin); // Contructor. Takes boiler pin number, pump pin number and temperature sensor pin number as parameters
	void monitorSystem(); // Function for monitoring all system states
	void boostHeating(bool state); // Function for enabling or disabling heating boost
	void boostWater(bool state); // Function for enabling or disabling hot water boost
	bool getHeatingStatus(); // Function for getting heating status
	bool getWaterStatus(); // Function for getting hot water status
	void disableHeating(); // Function to disable the heating
	void disableWater(); // Function to disable the hot water
	void enableHeating(); // Function to enable the heating
	void enableWater(); // Function to enable the hot water
	void setTemp(int temp); // Function to set requested temperature


private:
	const byte boostLengthWater = 20; // Length of time in minutes for hot water boost
	const byte boostLengthHeating = 30; // Length of time in minutes for heating boost
	const byte timerTimeInc = 5; // Length of time in minutes for the adjustment value for timer
	const int maxDrift = 1; // Set the maximum number of degrees the temperature can drift before heating is turned on
	const float tempChange = 0.5;
	const float minTempDifference = 0.5; // Variable to store the minimum temperature difference neded to warrat any heating system change
	bool incorrectTemp = false; // Variable to store whether the requested temperature is currently being met
	float lastChangedTemp = 0; // Variable to store the temperature at which the heating system was changed due to temperature difference
	Function touchOption = Function::None; // Variable to store what the current touch option is
	Function remoteOption = Function::None; // variable to store what the current option selected by a remote source (web interface, app, etc.)
	bool heatingMaster = true; // Variable to store the heating master value. Defaults to on. This is to disable automated temperature checks when the heating is in the off mode
	unsigned long startTimeHeatingBoost; // Variable to store the time at which the heating boost was turned on
	unsigned long startTimeWaterBoost; // Variable to store the time at which the hot water boost was turned on
	bool requestTemp; // Variable to store if we need to collect the temperature

	bool updateDisplay = false; // Whether the system needs to update the display or not
	Screen screen = Screen::Main; // 0 = Regular screen. 1 = Timer screen. 2 = Time change screen
	byte lastSystemMode = 3; // 0 = Heating and Water ON, 1 = Heating ON and Water OFF, 2 = Heating OFF and Water ON, 3 = Heating and Water OFF
	unsigned long lastTimeUpdate = 0; // Variable to store the last state the system was in. Stops the system trying to enter a state it is already in
	unsigned long lastHourlyUpdate = 0; // Variable to store when the last hourly update was done

	bool SDAvailable = false; // Variable to store whether the SD card containing some required files is available

	// Thermostat data
	float requestedTemp = 11.5; // Variable to store requested temperature. Default is 11.5 degrees
	bool heatingStatus = false; // Variable to store the required status of the heating
	bool waterStatus = false; // Variable to store the required status of the hot water
	bool heatingBoostActive = false; // Variable to store whether heating boost is currently active or not. Defaults to off
	bool waterBoostActive = false; // Variable to store whether the hot water boost is currently active or not. Defaults to off
	float currentTemp; // Variable to store the current temperature being displayed
	float currentInternalTemp; // Variale to store the current internal temperature of the thermostat
	Mode heatingMode = Mode::On; // 0 = off, 1 = timer, 2 = on. Defaults to on at 15 degrees
	Mode waterMode = Mode::Off; // 0 = off, 1 = timer, 2 = on
	

	EthernetUDP udp; // UDP client to request time form NTP server
	Pump pump; // Pump component
	Boiler boiler; // Boiler component
	TempSensor tempSensor; // Temperature sensor component
	TempSensor internalTempSensor; // Temperature sensor inside the thermostat
	Display* display; // Display component
	Timer timer; // Timer component
	WebInterface remote; // Web interface component
	Config config; // Configuration
	void setHeatingOff(); // Function for setting the state of heating components to off
	void setWaterOff(); // Function for setting state of hot water components to off
	void setHeatingOn(); // Function for setting the state of heating components to on
	void setWaterOn(); // Function for setting state of hot water components to on
	void setWaterWithoutHeating(); // Function for setting the state of the hot water when the heating is not on
	void checkBoosts(); // Function for checking whether heating and hot water need to be on or off depending on active boosts
	byte temperatureCheck(); // Function for checking the temperature against requested temperature
	bool needToChangeTemp(); // Function for checking whether temperature has moved enough to warrant changing the state of the heating
	void changeRelayStates(); // Function for setting final relay states
	void loadTimer(); // Function for loading configuration from SD card
	bool saveTimer(const char* timerCase, int time); // Function for saving timer configuration
	Mode incrementMode(Mode mode);

	void setupWatchdog(); // Function for initiating the watchdog

	int resetCounter = 0;
	char buffer[3];
	


};

#endif
