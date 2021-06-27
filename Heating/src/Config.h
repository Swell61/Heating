// Logging.h

#ifndef _CONFIG_h
#define _CONFIG_h

#include "Arduino.h"

#include <SD.h>
class Config {
private:
#define MAX_PROPERTY_LENGTH 10
#define MAX_VALUE_LENGTH 6
	const unsigned char SD_CHIP_SELECT_PIN;
	const char *FILE_NAME;
	const char SEPARATOR; // Character used to separate property from it's value. Defaults to '='
	File openFile();
	char currentValue[MAX_VALUE_LENGTH + 1];
	void writeValue(File &file, const char* vaule);
public:
	Config(const char* fileName, const unsigned char sdChipSelect); // Constructor takes name of file
	Config(const char* fileName, const unsigned char sdChipSelect, const char separator); // Constructor takes name of file and separator
	bool writeProperty(const char* property, const char* value);
	const char* readProperty(const char* property);
	bool available() const;
};

#endif

