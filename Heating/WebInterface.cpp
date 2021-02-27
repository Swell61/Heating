// 
// 
// 

#include "WebInterface.h"
WebInterface::WebInterface(bool SDAvailable) : webFilesAvailable(SDAvailable) {
	Ethernet.begin(mac, ip);
	
	ip = Ethernet.localIP();
	// start listening for clients
	server.begin();
}

void WebInterface::processRemoteOutput(int time, Mode heatingMode, Mode waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost, float internalTemp) {
	// Send a string containing the status of the main display
	
	char buffer[8];
	itoa(time, buffer, 10);
	char output[33] = "0:";
	strcat(output, buffer);

	strcat(output, ":");

	itoa(static_cast<unsigned char>(heatingMode), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(static_cast<unsigned char>(waterMode), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	dtostrf(temp, 5, 2, buffer);
	strcat(output, buffer);
	strcat(output, ":");

	strcat(output, heatingStatus ? "1" : "0");
	strcat(output, ":");

	strcat(output, waterStatus ? "1" : "0");
	strcat(output, ":");

	dtostrf(requestedTemp, 5, 2, buffer);
	strcat(output, buffer);
	strcat(output, ":");

	strcat(output, heatingBoost ? "1" : "0");
	strcat(output, ":");

	strcat(output, waterBoost ? "1" : "0");
	strcat(output, ":");


	dtostrf(internalTemp, 5, 2, buffer);
	strcat(output, buffer);
	strcat(output, "\0");
	strcat(buffer, "\0");

	webServerStack_ProcessMsgOut(output);
	
}
void WebInterface::processRemoteOutput(Mode heatingMode, Mode waterMode, const Timer& timer) {
	// Send a string containing the status of the timer
	//String output = "1:" + (String)heatingTimerStatus + ":" + (String)waterTimerStatus + ":" + (String)heatingOnMorning + ":" + (String)heatingOffMorning + ":" + (String)heatingOnAfternoon + ":" + (String)heatingOffAfternoon + ":" + (String)waterOnMorning + ":" + (String)waterOffMorning + ":" + (String)waterOnAfternoon + ":" + (String)waterOffAfternoon;
	
	char buffer[6];
	char output[72] = "1:";
	
	strcat(output, heatingMode == Mode::Timer ? "1" : "0");
	strcat(output, ":");

	strcat(output, waterMode == Mode::Timer ? "1" : "0");
	strcat(output, ":");

	itoa(timer.getHeatingOnMorning(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(timer.getHeatingOffMorning(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(timer.getHeatingOnAfternoon(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(timer.getHeatingOffAfternoon(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(timer.getWaterOnMorning(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(timer.getWaterOffMorning(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(timer.getWaterOnAfternoon(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(timer.getWaterOffAfternoon(), buffer, 10);
	strcat(output, buffer);
	strcat(output, "\0");
	strcat(buffer, "\0");
	
	
	webServerStack_ProcessMsgOut(output);
}
Function WebInterface::processRemoteInput(const char* buffer, Config config) {
	Ethernet.maintain();
	if (server.available()) { // If a client has a message to send...
		config.writeProperty(buffer, "5");
		return webServerStack_ProcessMsgIn(buffer, config); // Process it
	}
	else { // Otherwise no client message processing needed
		return Function::None;
	}
}
Function WebInterface::processRemoteInput() { // Process a client
	Ethernet.maintain();
	if (server.available()) { // If a client has a message to send...
		return webServerStack_ProcessMsgIn(); // Process it
	}
	else { // Otherwise no client message processing needed
		return Function::None;
	}
}
Function WebInterface::webServerStack_ProcessMsgIn(const char* buffer, Config config) {
	bool inMsgPassed = false;
		// Process for connected client
		for (int i = 0; i < MAX_CLIENT_NUM; i++) {
			wdt_reset();
			if (webSocketStack[i].client) {
				if (webSocketStack[i].client.connected()) {
					// Incoming msg
					if (webSocketStack[i].client.available()) {
						config.writeProperty(buffer, "18");
						const char *data = webSocketStack[i].webSocketServer.getData();
						if (isDigit(data[0]) && (strlen(data) == 1 || (strlen(data) == 2 && isDigit(data[1])))) { // If they have sent a valid message
							return commandToFunction(data);	
						}
						else {
							webSocketStack[i].client.stop(); // IE and Edge send rubbish through websocket when refreshing or closing. Kick the client if non-valid data sent
						}
						
						inMsgPassed = true;
						return Function::None;
					}
				}
				else { // client may be closed
					config.writeProperty(buffer, "19");
					webSocketStack[i].client.stop();
				}
			}
		}

		// If at this point, the client either wants the web page sending or to handshake for websocket connection

		// incomming data has been passed thru client ==> no data for new client! ==> Bug found, kakaka...
		// Return now :)
		if (inMsgPassed) return Function::None;

		// Data here is still available and not yet processed ==> pass to new client!
		
		// Scan for new client
		int j = MAX_CLIENT_NUM;
		for (int i = 0; i < MAX_CLIENT_NUM; i++) {
			if (!webSocketStack[i].client) {
				j = i;
				break;
			}
		}


		if (j < MAX_CLIENT_NUM) { // If there is room on the stack for another client
			webSocketStack[j].client = server.available();
			if (webSocketStack[j].client.connected()) {
				wdt_reset();
				byte request = webSocketStack[j].webSocketServer.handshake(webSocketStack[j].client); // try to handshake. Will return 2 if web files need to be sent
				
				if (request == 1) { // If handshake succeeded
					return Function::NewClient;
				}
				else if (request == 2 && webFilesAvailable) { // If need to send web files and web files are available
					char* requestPath = webSocketStack[j].webSocketServer.getRequestPath(); // Get the requested file path
					//if (strlen(requestPath) == 0) { // If client requests an empty path, send them the main page HTML file
					//	Serial.println("Changing path");
					//	strcpy(requestPath, "index.htm");
					//}
					if (SD.exists(requestPath)) { // if the file is available
						// Return the requested file to the current client
						config.writeProperty(buffer, "26");

						webSocketStack[j].client.println(F("HTTP/1.1 200 OK")); // Acknowledge request
						if (strstr(requestPath, ".jpg") != NULL) { // If sending an image
							webSocketStack[j].client.println(); // Send an empty line

						}
						else if (strstr(requestPath, ".css") != NULL) { // If requesting a stylesheet
							webSocketStack[j].client.println(F("Content-Type: text/css")); // Acknowledge CSS request
							webSocketStack[j].client.println(F("Connection: keep-alive"));
							webSocketStack[j].client.println();

						}
						else { // Else if requesting a HTML document
							webSocketStack[j].client.println(F("Content-Type: text/html")); // Acknowledge HTML request
							webSocketStack[j].client.println(F("Connection: keep-alive"));
							webSocketStack[j].client.println();

						}
						File webFile = SD.open(requestPath); // Get the file
						if (webFile) { // Send the file
							config.writeProperty(buffer, "27");
							int counter = 0;
							float startFileSend = millis();
							wdt_reset();
							while (webFile.available() && (millis() - startFileSend) <= 3800) {
								webSocketStack[j].client.write(webFile.read()); // send web page to client
							}
							config.writeProperty(buffer, "31");
							webFile.close();
						}
						webSocketStack[j].client.stop(); // Close the connection
						return Function::None;
					}
					else { // Otherwise...
						webSocketStack[j].client.stop(); // Close the connection as it cannot be serverd
						return Function::None;
					}
				}
				else { // Otherwise...
					webSocketStack[j].client.stop(); // Close the connection as it cannot be served
					return Function::None;
				}
			}
		
	}
		config.writeProperty(buffer, "22");
	return Function::NewClient;
}
Function WebInterface::webServerStack_ProcessMsgIn() {
	bool inMsgPassed = false;
	// Process for connected client
	for (int i = 0; i < MAX_CLIENT_NUM; i++) {
		wdt_reset();
		if (webSocketStack[i].client) {
			if (webSocketStack[i].client.connected()) {
				// Incoming msg
				if (webSocketStack[i].client.available()) {
					const char *data = webSocketStack[i].webSocketServer.getData();
					if (isDigit(data[0]) && (strlen(data) == 1 || (strlen(data) == 2 && isDigit(data[1])))) { // If they have sent a valid message

						return commandToFunction(data); // Return it
					}
					else {
						webSocketStack[i].client.stop(); // IE and Edge send rubbish through websocket when refreshing or closing. Kick the client if non-valid data sent
					}

					inMsgPassed = true;
					return Function::None;
				}
			}
			else { // client may be closed

				webSocketStack[i].client.stop();
			}
		}
	}

	// If at this point, the client either wants the web page sending or to handshake for websocket connection

	// incomming data has been passed thru client ==> no data for new client! ==> Bug found, kakaka...
	// Return now :)
	if (inMsgPassed) return Function::None;

	// Data here is still available and not yet processed ==> pass to new client!

	// Scan for new client
	int j = MAX_CLIENT_NUM;
	for (int i = 0; i < MAX_CLIENT_NUM; i++) {
		if (!webSocketStack[i].client) {
			j = i;
			break;
		}
	}


	if (j < MAX_CLIENT_NUM) { // If there is room on the stack for another client
		webSocketStack[j].client = server.available();
		if (webSocketStack[j].client.connected()) {
			wdt_reset();
			byte request = webSocketStack[j].webSocketServer.handshake(webSocketStack[j].client); // try to handshake. Will return 2 if web files need to be sent

			if (request == 1) { // If handshake succeeded
				return Function::NewClient;
			}
			else if (request == 2 && webFilesAvailable) { // If need to send web files and web files are available
				char* requestPath = webSocketStack[j].webSocketServer.getRequestPath(); // Get the requested file path
				if (strlen(requestPath) == 0) { // If client requests an empty path, send them the main page HTML file
					strcpy(requestPath, "index.htm");
				}
				if (SD.exists(requestPath)) { // if the file is available
											  // Return the requested file to the current client


					webSocketStack[j].client.println(F("HTTP/1.1 200 OK")); // Acknowledge request
					if (strstr(requestPath, ".jpg") != NULL) { // If sending an image
						webSocketStack[j].client.println(); // Send an empty line

					}
					else if (strstr(requestPath, ".css") != NULL) { // If requesting a stylesheet
						webSocketStack[j].client.println(F("Content-Type: text/css")); // Acknowledge CSS request
						webSocketStack[j].client.println(F("Connection: keep-alive"));
						webSocketStack[j].client.println();

					}
					else { // Else if requesting a HTML document
						webSocketStack[j].client.println(F("Content-Type: text/html")); // Acknowledge HTML request
						webSocketStack[j].client.println(F("Connection: keep-alive"));
						webSocketStack[j].client.println();

					}
					File webFile = SD.open(requestPath); // Get the file
					if (webFile) { // Send the file
						while (webFile.available()) {
							webSocketStack[j].client.write(webFile.read()); // send web page to client
						}

						webFile.close();
					}
					webSocketStack[j].client.stop(); // Close the connection

					return Function::None;
				}
				else { // Otherwise...
					webSocketStack[j].client.stop(); // Close the connection as it cannot be serverd
					return Function::None;
				}
			}
			else { // Otherwise...

				webSocketStack[j].client.stop(); // Close the connection as it cannot be served
				return Function::None;
			}
		}

	}
	return Function::NewClient;
}

Function WebInterface::commandToFunction(const char* data) {
	int command = atoi(data); // Return it
	return (command < static_cast<int>(Function::END) && command <= 0) ? static_cast<Function>(command) : Function::None;
						
}

void WebInterface::webServerStack_ProcessMsgOut(const char *output) {
	
	for (int i = 0; i < MAX_CLIENT_NUM; i++) { // Loop through each client connected
		if (webSocketStack[i].client) {
			wdt_reset(); // Reset the timer
			if (webSocketStack[i].client.connected()) {
				
					sendClientData(i, output);
			}
			else { // client may be closed
				
				webSocketStack[i].client.stop();
			}
		}
	}
}

void WebInterface::sendClientData(int id, const char *s) {
	webSocketStack[id].webSocketServer.sendData(s);
}
