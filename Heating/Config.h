// Logging.h

#ifndef _CONFIG_h
#define _CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <SD.h>
class Config {
private:
	String fileName;
	char separator = '=';
	File openFile();
public:
	Config(String); // Constructor takes name of file
	Config(String, char); // Constructor takes name of file and separator
	bool writeProperty(String, String);
	String readProperty(String);
};

#endif

