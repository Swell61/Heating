// WebInterface.h

#ifndef _WEBINTERFACE_h
#define _WEBINTERFACE_h

#include "Arduino.h"

#include <WebSocketServer.h>
#include <SPI.h>
#include "UIPEthernet.h"
#include <SD.h>
#include "Config.h"
#include <avr/wdt.h>
#include "Time/Timer.h"

// Class for web and remote interface interraction

class WebInterface {
#define MAX_CLIENT_NUM  4 // Max number of simultaneous clients 
#define PORT_NUM 80 // Port number for server
typedef struct { // Structure for web socket stack
		EthernetClient client;
		WebSocketServer webSocketServer;
		unsigned long previousMillis;
} WebSocketStack_T;

private:
	EthernetServer server = EthernetServer(80); // Ethernet server
	IPAddress ip = IPAddress(192, 168, 1, 201); // IP address of Arduino
	byte mac[6] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; // MAC address of network interface
	WebSocketStack_T webSocketStack[MAX_CLIENT_NUM]; // Stack to store each connection
	int webServerStack_ProcessMsgIn(); // Function for processing a message from a client
	int webServerStack_ProcessMsgIn(const char* buffer, Config config); // Function for processing a message from a client

	void webServerStack_ProcessMsgOut(const char *output); // Function for sending status to clients
	void sendClientData(int client, const char *output); // Function to send data to a client
	bool webFilesAvailable = false; // Variable to store whether web server files are available or not
public:
	WebInterface(bool webFilesAvailabe); // Constructor which takes parameter for whether web server files are available or not

	void processRemoteOutput(int time, byte heatingMode, byte waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost, float internalTemp); // Function for sending main display status to clients
	void processRemoteOutput(bool heatingTimerStatus, bool waterTimerStatus, const Timer&); // Function for sending timer status to clients

	int processRemoteInput(); // Function for processing client messages
	int processRemoteInput(const char* buffer, Config config);
};

#endif

