// WebInterface.h

#ifndef _WEBINTERFACE_h
#define _WEBINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <WebSocketServer.h>
#include <SPI.h>
#include "UIPEthernet.h"
class WebInterface {
#define MAX_CLIENT_NUM  4
#define PORT_NUM 80
typedef struct {
		EthernetClient client;
		WebSocketServer webSocketServer;
		unsigned long previousMillis;
} WebSocketStack_T;

private:
	EthernetServer server = EthernetServer(80);
	IPAddress ip = IPAddress(192, 168, 1, 201);
	byte mac[6] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
	WebSocketStack_T webSocketStack[MAX_CLIENT_NUM]; // Stack to store each connection
	int webServerStack_ProcessMsgIn();
	void webServerStack_ProcessMsgOut(String);
	void sendClientData(int, String);
	bool handleClientData(String & dataString);
public:
	WebInterface();

	void processRemoteOutput(int, byte, byte, float, bool, bool, float, bool, bool);
	void processRemoteOutput(bool, bool, int, int, int, int, int, int, int, int);

		int processRemoteInput();
};

#endif

