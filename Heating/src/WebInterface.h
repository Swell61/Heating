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
#include "Enums/SystemFunction.h"
#include "Time/ComponentTimer.h"

class Controller;
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
	EthernetServer server{80}; // Ethernet server
	
	WebSocketStack_T webSocketStack[MAX_CLIENT_NUM]; // Stack to store each connection
	unsigned char webServerStack_ProcessMsgIn(); // Function for processing a message from a client
	unsigned char webServerStack_ProcessMsgIn(const char* buffer, Config config); // Function for processing a message from a client
	void sendTimerData(ComponentTimer& heatingTimer, ComponentTimer& waterTimer, Clock& clock); // Function for sending timer status to clients
	void addTimerDataToOutput(char* output, ComponentTimer& timer);
	void sendMainData(Controller& controller);
	void webServerStack_ProcessMsgOut(const char *output); // Function for sending status to clients
	void sendClientData(int client, const char *output); // Function to send data to a client
	bool webFilesAvailable = false; // Variable to store whether web server files are available or not
public:
	WebInterface(bool webFilesAvailabe); // Constructor which takes parameter for whether web server files are available or not
	WebInterface();

	void processRemoteOutput(Controller& controller); // Function for sending main display status to clients

	unsigned char processRemoteInput(); // Function for processing client messages
	unsigned char processRemoteInput(const char* buffer, Config config);

	void begin();
};

#include "Controller.h"

#endif

