// Logging.h

#ifndef _CONFIG_h
#define _CONFIG_h

#include "Arduino.h"
#include "../Enums/SystemComponent.h"
#include "../Enums/TimerPeriod.h"
#include "../Enums/State.h"

#include <SD.h>
class Config {
private:
#define MAX_PROPERTY_LENGTH 12
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
	bool available() const;
protected:
	bool writeProperty(const char* property, const char* value);
	const char* readProperty(const char* property);
	int readInt(const char* propertyName, unsigned char expectedMaxLength);

	
};

#endif

