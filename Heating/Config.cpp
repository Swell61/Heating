// 
// 
// 

#include "Config.h"

Config::Config(const char* fn) : fileName(fn) {
	//strncpy(fileName, fn, 8);
	// Separator defaults to '='
}

Config::Config(const char* fn, char separator) : separator(separator) {
	//strncpy(fileName, fn, 8);
}

bool Config::writeProperty(const char* property, const char* value) {
	Serial.println(fileName);
	Serial.println(SD.exists(fileName) ? "Config file found" : "Config file not found");
	byte valueLength = strlen(value);
	if ((strlen(property) > MAX_PROPERTY_LENGTH) || (valueLength > MAX_VALUE_LENGTH))
		return 0; // Failed to write value
	File file = openFile();
	char currentChar;
	char currentProperty[MAX_PROPERTY_LENGTH + 1];
	currentValue[0] = '\0'; // Reset the current value, ready for new search
	while ((currentChar = file.read()) > -1) {
		Serial.print(currentChar);
		if (currentChar == separator) { // If at end of current property, check against requested property
			Serial.println(" At separator ");
			if (strncmp(currentProperty, property, strlen(currentProperty)) == 0) { // Compare current property and the requested property
				Serial.println("Found property");
				// Pointer will currently be after the separator
				writeValue(file, value);
				file.close();
				return 1; // Value has been written
			}
			else {
				Serial.println("Incorrect property");
				currentProperty[0] = '\0'; // Reset to get next property
				while (file.read() != '\n'); // Move pointer to end of line
			}
		}
		else {
			byte len = strlen(currentProperty);
			if (len >= MAX_PROPERTY_LENGTH) { // Overload protection

				break;
			}
			currentProperty[len + 1] = currentChar; // Add current character to char array
			currentProperty[len + 2] = '\0'; // Re-terminate the char array
			Serial.print("Current currentProperty: ");
			Serial.println(currentProperty);
		}
	}
	// If program gets to here, property does not currently exist and pointer is at bottom of file
	file.close();
	file = SD.open(fileName, FILE_WRITE);
	Serial.println("Not found property. Writing new");
	Serial.println(property);
	Serial.println(file.print(property)); // Write the property
	Serial.println(property);
	Serial.println(file.print(separator)); // Write the separator
	writeValue(file, value); // Write the value
	file.close();
	return 1; // Property write succeeded

	// Search for the current property in the file
	// If it exists, find the index of the separator (cursor should automatically be set to the position after. Remove all the value characters and print the new ones
	// If not, go the the end of the file and print the new name, separator and value
}
void Config::writeValue(File &file, const char* value) {
	file.write(value);
	for (byte i = 0; i < MAX_VALUE_LENGTH; i++) { // Pad out line to maximum length
		file.write("\0");
	}
	file.write("\n"); // Add newline to end of line
}
char* Config::readProperty(const char* property) {
	
	
	File file = SD.open(fileName);
	char currentChar;
	char currentProperty[MAX_PROPERTY_LENGTH + 1];
	currentProperty[0] = '\0';
	currentValue[0] = '\0'; // Reset the current value, ready for new search
	if (strlen(property) > MAX_PROPERTY_LENGTH) {
		return currentValue;
	}
	
	while ((currentChar = file.read()) > -1) {
		if (currentChar == separator) { // If at end of current property, check against requested property
			
			if (strncmp(currentProperty, property, strlen(currentProperty)) == 0) { // Compare current property and the requested property
				
				while ((currentChar = file.read()) != '\n') {
					byte len = strlen(currentValue);
					currentValue[len] = currentChar;
					currentValue[len + 1] = '\0';
				}
				file.close(); // Close file after finding value
				
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
	return SD.open(fileName);
}