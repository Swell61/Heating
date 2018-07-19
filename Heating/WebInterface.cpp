// 
// 
// 

#include "WebInterface.h"

WebInterface::WebInterface(bool SDAvailable) : webFilesAvailable(SDAvailable) {
	Ethernet.begin(mac, ip);
	
	ip = Ethernet.localIP();
	for (byte thisByte = 0; thisByte < 4; thisByte++) {
		// print the value of each byte of the IP address:
		
		
	}
	

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
		lastClientConnect = millis();
		// Process for connected client
		for (int i = 0; i < MAX_CLIENT_NUM; i++) {
			if (webSocketStack[i].client) {
				if (webSocketStack[i].client.connected()) {
					// Incoming msg
					if (webSocketStack[i].client.available()) {
						String data = webSocketStack[i].webSocketServer.getData();
						if (data.length() == (1 || 2) && isDigit(data.charAt(0))) {
							
							
							return data.toInt();
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
				
				String request = webSocketStack[j].webSocketServer.handshake(webSocketStack[j].client);
				
				
				if (request == "1") {
					
				}
				else if (request != "0" && webFilesAvailable) {
					if (request == "") {
						request = "index.htm";
					}
					// Return the requested file to the current client
					
					
					webSocketStack[j].client.println("HTTP/1.1 200 OK");
					if (request.indexOf(".jpg") > 0) {
						webSocketStack[j].client.println();
						
					}
					else if (request.indexOf(".css") > 0) {
						webSocketStack[j].client.println("Content-Type: text/css");
						webSocketStack[j].client.println("Connection: keep-alive");
						webSocketStack[j].client.println();
						
					}
					else {
						webSocketStack[j].client.println("Content-Type: text/html");
						webSocketStack[j].client.println("Connection: keep-alive");
						webSocketStack[j].client.println();
						
					}
					File webFile = SD.open(request);
					if (webFile) {
						while (webFile.available()) {
							webSocketStack[j].client.write(webFile.read()); // send web page to client
						}
						
						webFile.close();
					}
					webSocketStack[j].client.stop();
					
					return 0;
				}
				else {
					
					webSocketStack[j].client.stop();
					return 0;
				}
			}
		
	}
	return 255;
}

String WebInterface::modifyRequest(String request) {

}

void WebInterface::webServerStack_ProcessMsgOut(String output) {
	for (int i = 0; i < MAX_CLIENT_NUM; i++) { // Loop through each client connected
		if (webSocketStack[i].client) {
			if (webSocketStack[i].client.connected()) {
				
					sendClientData(i, output);
			}
			else { // client may be closed
				
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