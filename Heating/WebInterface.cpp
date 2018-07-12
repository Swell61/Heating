// 
// 
// 

#include "WebInterface.h"

WebInterface::WebInterface() {
	Ethernet.begin(mac, ip);
	Serial.print("IP is: ");
	ip = Ethernet.localIP();
	for (byte thisByte = 0; thisByte < 4; thisByte++) {
		// print the value of each byte of the IP address:
		Serial.print(ip[thisByte], DEC);
		Serial.print(".");
	}
	Serial.println();

	// start listening for clients
	server.begin();
}

void WebInterface::processRemoteOutput(int time, byte heatingMode, byte waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost) {
	String output = "0:" + (String)time + ":" + (String)heatingMode + ":" + (String)waterMode + ":" + (String)temp + ":" + (String)heatingStatus + ":" + (String)waterStatus + ":" + (String)requestedTemp + ":" + (String)heatingBoost + ":" + (String)waterBoost;
	webServerStack_ProcessMsgOut(output);
}
void WebInterface::processRemoteOutput(bool heatingTimerStatus, bool waterTimerStatus, int heatingOnMorning, int heatingOffMorning, int heatingOnAfternoon, int heatingOffAfternoon, int waterOnMorning, int waterOffMorning, int waterOnAfternoon, int waterOffAfternoon) {
	String output = "1:" + (String)heatingTimerStatus + ":" + (String)waterTimerStatus + ":" + (String)heatingOnMorning + ":" + (String)heatingOffMorning + ":" + (String)heatingOnAfternoon + ":" + (String)heatingOffAfternoon + ":" + (String)waterOnMorning + ":" + (String)waterOffMorning + ":" + (String)waterOnAfternoon + ":" + (String)waterOffAfternoon;
	webServerStack_ProcessMsgOut(output);
}

int WebInterface::processRemoteInput() {
	if (server.available()) {
		return webServerStack_ProcessMsgIn();
	}
	else {
		return 0;
	}
}
int WebInterface::webServerStack_ProcessMsgIn() {
	bool inMsgPassed = false;

	// Process for connected client
	for (int i = 0; i < MAX_CLIENT_NUM; i++) {
		if (webSocketStack[i].client) {
			if (webSocketStack[i].client.connected()) {
				// Incoming msg
				if (webSocketStack[i].client.available()) {
					String data = webSocketStack[i].webSocketServer.getData();
					if (data.length() > 0) {
						Serial.print("data!\n\r");
						if (data != "") {
							
							return data.toInt();
							
						}
					}
					inMsgPassed = true;
					break;
				}
			}
			else { // client may be closed
				Serial.print("closed!\n\r");
				webSocketStack[i].client.stop();
			}
		}
	}

	// incomming data has been passed thru client ==> no data for new client! ==> Bug found, kakaka...
	// Return now :)
	if (inMsgPassed) return 0;

	// Data here is still available and not yet processed ==> pass to new client!

	// Scan for new client
	int j = MAX_CLIENT_NUM;
	for (int i = 0; i < MAX_CLIENT_NUM; i++) {
		if (!webSocketStack[i].client) {
			j = i;
			break;
		}
	}

	if (j < MAX_CLIENT_NUM) {
		webSocketStack[j].client = server.available();
		if (webSocketStack[j].client.connected()) {
			Serial.print("Client connected\r\n");
			if (webSocketStack[j].webSocketServer.handshake(webSocketStack[j].client)) {
				Serial.print("Client handshaked\r\n");
			}
			else {
				webSocketStack[j].client.stop();
			}
		}
	}
	return 255;
}

void WebInterface::webServerStack_ProcessMsgOut(String output) {
	for (int i = 0; i < MAX_CLIENT_NUM; i++) { // Loop through each client connected
		if (webSocketStack[i].client) {
			if (webSocketStack[i].client.connected()) {
				
					sendClientData(i, output);
			}
			else { // client may be closed
				Serial.print("closed!\n\r");
				webSocketStack[i].client.stop();
			}
		}
	}
}

void WebInterface::sendClientData(int id, String s) {
	webSocketStack[id].webSocketServer.sendData(s);
}

bool WebInterface::handleClientData(String &dataString) {

}