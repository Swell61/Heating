// 
// 
// 

#include "Config.h"

Config::Config(String fileName) : fileName(fileName) {
}

Config::Config(String filename, char separator) : fileName(fileName), separator(separator) {
	if (!SD.exists(filename)) {
		SD.mkdir(filename);
	}
}

bool Config::writeProperty(String name, String value) {
	// Search for the current property in the file
	// If it exists, find the index of the separator (cursor should automatically be set to the position after. Remove all the value characters and print the new ones
	// If not, go the the end of the file and print the new name, separator and value
}

String Config::readProperty(String name) {
	File file = openFile();
	char currentChar;
	String currentProperty = "";
	while ((currentChar = file.read()) > -1) {
		if (currentChar == '\n') { // If finished reading the current property
			if (currentProperty.indexOf(name) > 0) { // If the current property is the correct one
				return currentProperty.substring(currentProperty.indexOf(separator) + 1);
			}
			else {
				currentProperty = "";
			}
		}
	}
}

File Config::openFile() {
	return SD.open(fileName);
}