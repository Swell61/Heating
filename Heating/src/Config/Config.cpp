// 
// 
// 

#include "Config.h"

Config::Config(const char* FILE_NAME, const unsigned char sdChipSelectPin) : Config(FILE_NAME, sdChipSelectPin, '=') { }

Config::Config(const char* FILE_NAME, const unsigned char sdChipSelectPin, const char SEPARATOR) : FILE_NAME(FILE_NAME), SD_CHIP_SELECT_PIN(sdChipSelectPin), SEPARATOR(SEPARATOR) {
	SD.begin(sdChipSelectPin);
}

bool Config::available() const {
	return SD.begin(SD_CHIP_SELECT_PIN) && SD.exists(FILE_NAME);
}

bool Config::writeProperty(const char* property, const char* value) {
	if (!available()) {
		return false;
	}

	byte valueLength = strlen(value);
	if ((strlen(property) > MAX_PROPERTY_LENGTH) || (valueLength > MAX_VALUE_LENGTH))
		return 0; // Failed to write value
	File file = SD.open(FILE_NAME, O_READ | O_WRITE | O_CREAT);

	file.seek(0); // Go to start of file (FILE_WRITE defaults to end of file)
	char currentChar;
	char currentProperty[MAX_PROPERTY_LENGTH + 1];
	currentProperty[0] = '\0';
	currentValue[0] = '\0'; // Reset the current value, ready for new search
	while ((currentChar = file.read()) > -1) {
		if (currentChar == SEPARATOR) { // If at end of current property, check against requested property
			if (strncmp(currentProperty, property, strlen(currentProperty)) == 0) { // Compare current property and the requested property
				// Pointer will currently be after the SEPARATOR
				writeValue(file, value);
				file.close();
				return 1; // Value has been written
			}
			else {
				currentProperty[0] = '\0'; // Reset to get next property
				while (file.read() != '\n'); // Move pointer to end of line
			}
		}
		else {
			byte len = strlen(currentProperty);
			currentProperty[len] = currentChar; // Add current character to char array
			currentProperty[len + 1] = '\0'; // Re-terminate the char array
		}
	}
	// If program gets to here, property does not currently exist and pointer is at bottom of file
	file.close();
	file = SD.open(FILE_NAME, FILE_WRITE);
	file.print(property); // Write the property
	file.print(SEPARATOR); // Write the SEPARATOR
	writeValue(file, value); // Write the value
	file.close();
	return 1; // Property write succeeded

	// Search for the current property in the file
	// If it exists, find the index of the SEPARATOR (cursor should automatically be set to the position after. Remove all the value characters and print the new ones
	// If not, go the the end of the file and print the new name, SEPARATOR and value
}
void Config::writeValue(File &file, const char* value) {
	file.print(value);
	for (byte i = strlen(value); i < MAX_VALUE_LENGTH; i++) { // Pad out line to maximum length
		file.print(" ");
	}
	file.print("\n"); // Add newline to end of line
}



const char* Config::readProperty(const char* property) {
	currentValue[0] = '\0'; // Reset the current value, ready for new search
	if (!SD.exists(FILE_NAME)) {
		Serial.println("Not available");
		return currentValue;
	}
	File file = SD.open(FILE_NAME);
	char currentChar;
	char currentProperty[MAX_PROPERTY_LENGTH + 1];
	currentProperty[0] = '\0';
	
	if (strlen(property) > MAX_PROPERTY_LENGTH) {
		return currentValue;
	}
	
	while ((currentChar = file.read()) > -1) {
		if (currentChar == SEPARATOR) { // If at end of current property, check against requested property
			if (strncmp(currentProperty, property, strlen(currentProperty)) == 0) { // Compare current property and the requested property
				while ((currentChar = file.read()) != '\n') {
					byte len = strlen(currentValue);
					currentValue[len] = currentChar;
					currentValue[len + 1] = '\0';
				}
				file.close(); // Close file after finding value
				
				for (byte i = 0; i <= MAX_VALUE_LENGTH; i++) { // Chop off proceeding spaces
					if (currentValue[i] == ' ') {
						currentValue[i] = '\0';
						break;
					}
				}
				return currentValue; // Return value
			}
			else {
				
				currentProperty[0] = '\0'; // Reset to get next property
				while (file.read() != '\n'); // Move pointer to end of line
			}
		}
		else {
			byte len = strlen(currentProperty);
			
			
			currentProperty[len] = currentChar; // Add current character to char array
			currentProperty[len + 1] = '\0'; // Re-terminate the char array
			
		}
	}
	file.close(); // Close file if not found value
	return currentValue; // If property not found, return nothing
}

File Config::openFile() {
	return SD.open(FILE_NAME);
}

int Config::readInt(const char* propertyName, unsigned char expectedMaxLength) {
	Serial.println("Reading int");
	const char* data = readProperty(propertyName);
	Serial.println(data);
	unsigned char len;
	if ((len = strnlen(data, expectedMaxLength)) > 0) {
		bool isNum = true;
		for (unsigned char currentIndex = 0; currentIndex < len; ++currentIndex) {
			isNum &= isDigit(data[currentIndex]);
		}

		if (isNum) {
			Serial.println(data);
			return atoi(data);
		}
		Serial.println("Not num");
	}
	return 0;
}
