#include "WebInterface.h"

WebInterface::WebInterface(bool SDAvailable) : webFilesAvailable(SDAvailable) {
}

WebInterface::WebInterface() : WebInterface(false) {}

void WebInterface::begin() {
	server.begin();
}

void WebInterface::processRemoteOutput(Controller& controller) {
	sendMainData(controller);
	sendTimerData(controller.getComponentControl().getHeating().getTimer(), controller.getComponentControl().getWater().getTimer(), controller.getClock());
}

void WebInterface::sendMainData(Controller& controller) {
// Send a string containing the status of the main display
	char buffer[8];
	itoa(controller.getClock().getTimeInMinutes(), buffer, 10);
	char output[33] = "0:";
	strcat(output, buffer);

	strcat(output, ":");

	itoa(static_cast<unsigned char>(controller.getComponentControl().getHeating().getMode()), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(static_cast<unsigned char>(controller.getComponentControl().getWater().getMode()), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	dtostrf(controller.getTempSensor().getTemp(), 5, 2, buffer);
	strcat(output, buffer);
	strcat(output, ":");

	strcat(output, controller.getComponentControl().getHeating().required(controller.getClock(), controller.getTempSensor().getTemp()) ? "1" : "0");
	strcat(output, ":");

	strcat(output, controller.getComponentControl().getWater().required(controller.getClock()) ? "1" : "0");
	strcat(output, ":");

	dtostrf(controller.getComponentControl().getHeating().getRequestedTemp(), 5, 2, buffer);
	strcat(output, buffer);
	strcat(output, ":");

	strcat(output, controller.getComponentControl().getHeating().getBoost().checkState(controller.getClock()) == State::ON ? "1" : "0");
	strcat(output, ":");

	strcat(output, controller.getComponentControl().getWater().getBoost().checkState(controller.getClock()) == State::ON ? "1" : "0");
	strcat(output, ":");


	dtostrf(0.0f, 5, 2, buffer);
	strcat(output, buffer);
	strcat(output, "\0");
	strcat(buffer, "\0");

	webServerStack_ProcessMsgOut(output);
}

void WebInterface::sendTimerData(ComponentTimer& heatingTimer, ComponentTimer& waterTimer, Clock& clock) {
	// Send a string containing the status of the timer
	//String output = "1:" + (String)heatingTimerStatus + ":" + (String)waterTimerStatus + ":" + (String)heatingOnMorning + ":" + (String)heatingOffMorning + ":" + (String)heatingOnAfternoon + ":" + (String)heatingOffAfternoon + ":" + (String)waterOnMorning + ":" + (String)waterOffMorning + ":" + (String)waterOnAfternoon + ":" + (String)waterOffAfternoon;
	
	char output[72] = "1:";
	
	strcat(output, heatingTimer.timerStatus(clock) ? "1" : "0");
	strcat(output, ":");

	strcat(output, waterTimer.timerStatus(clock) ? "1" : "0");
	strcat(output, ":");

	addTimerDataToOutput(output, heatingTimer);
	addTimerDataToOutput(output, waterTimer);

	strcat(output, "\0");
	
	
	webServerStack_ProcessMsgOut(output);
}

void WebInterface::addTimerDataToOutput(char* output, ComponentTimer& timer) {
	char buffer[6];
	itoa(timer.getMorningTimer().getOnTime(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(timer.getMorningTimer().getOffTime(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(timer.getAfternoonTimer().getOnTime(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");

	itoa(timer.getAfternoonTimer().getOffTime(), buffer, 10);
	strcat(output, buffer);
	strcat(output, ":");
}

unsigned char WebInterface::processRemoteInput() { // Process a client
	if (server.available()) { // If a client has a message to send...
		return webServerStack_ProcessMsgIn(); // Process it
	}
	else { // Otherwise no client message processing needed
		return 0;
	}
}

unsigned char WebInterface::webServerStack_ProcessMsgIn() {
	bool inMsgPassed = false;
	// Process for connected client
	for (int i = 0; i < MAX_CLIENT_NUM; i++) {
		if (webSocketStack[i].client) {
			if (webSocketStack[i].client.connected()) {
				// Incoming msg
				if (webSocketStack[i].client.available()) {
					const char *data = webSocketStack[i].webSocketServer.getData();
					if (isDigit(data[0]) && (strlen(data) == 1 || (strlen(data) == 2 && isDigit(data[1])))) { // If they have sent a valid message


						return atoi(data); // Return it
					}
					else {
						webSocketStack[i].client.stop(); // IE and Edge send rubbish through websocket when refreshing or closing. Kick the client if non-valid data sent
					}

					inMsgPassed = true;
					return 0;
				}
			}
			else { // client may be closed

				webSocketStack[i].client.stop();
			}
		}
	}

	// // If at this point, the client either wants the web page sending or to handshake for websocket connection

	// // incomming data has been passed thru client ==> no data for new client! ==> Bug found, kakaka...
	// // Return now :)
	// if (inMsgPassed) return 0;

	// // Data here is still available and not yet processed ==> pass to new client!

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
			byte request = webSocketStack[j].webSocketServer.handshake(webSocketStack[j].client); // try to handshake. Will return 2 if web files need to be sent

			if (request == 1) { // If handshake succeeded
				return static_cast<unsigned char>(SystemFunction::NEW_CLIENT);
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

					return 0;
				}
				else { // Otherwise...
					webSocketStack[j].client.stop(); // Close the connection as it cannot be serverd
					return 0;
				}
			}
			else { // Otherwise...

				webSocketStack[j].client.stop(); // Close the connection as it cannot be served
				return 0;
			}
		}

	}
	return 0;
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
