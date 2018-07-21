// 
// 
// 

#include "Timer.h"

bool Timer::getHeatingTimerStatus() { // Function for getting the status of the heating
	// If the heating should be on based on the timer, return true. Otherwise return false
	int time = getTime() + 1;
	if ((time > heatingOnMorning) && (time < heatingOffMorning)) { // If within morning on period...
		return true;
	}
	
	else if ((time > heatingOnAfternoon) && (time < heatingOffAfternoon)) { // If within afternoon on period...
		return true;
	}
	else { // If not in either...
		return false;
	}
}

bool Timer::getWaterTimerStatus() { // Function for getting the status of the hot water
// If the hot water should be on based on the timer, return true. Otherwise return false
	int time = getTime() + 1;
	if ((time > waterOnMorning) && (time < waterOffMorning)) { // If within morning on period...
		return true;
	}
	
	else if (((time) > waterOnAfternoon) && (time < waterOffAfternoon)) { // If within afternoon on period...
		return true;

	}
	else { // If not in either...
		return false;
	}
}

bool Timer::setMidnight(int time) { // Function for setting midnight
		if ((millis() / 60000) - time <= 1440) {
				midnight = time;
				return true;
	}
	
}

void Timer::checkMidnight(UIPUDP &udp) { // Function for updating midnight and checking the time using NTP server
	if (((millis() / 60000) - midnight) >= 1440) {
		midnight = millis() / 60000;
		
		if (!setMidnightNTP(udp)) {
			(setMidnightNTP(udp));
		}
	}
}

int Timer::getHeatingOnMorning() {
	return heatingOnMorning;
}
int Timer::getHeatingOffMorning() {
	return heatingOffMorning;
}
int Timer::getHeatingOnAfternoon() {
	return heatingOnAfternoon;
}
int Timer::getHeatingOffAfternoon() {
	return heatingOffAfternoon;
}
int Timer::getWaterOnMorning() {
	return waterOnMorning;
}
int Timer::getWaterOffMorning() {
	return waterOffMorning;
}
int Timer::getWaterOnAfternoon() {
	return waterOnAfternoon;
}
int Timer::getWaterOffAfternoon() {
	return waterOffAfternoon;
}

bool Timer::setHeatingOnMorning(int time) {
	if (time < heatingOffMorning && time > 0) {
		heatingOnMorning = time;
		return true;
	}
	return false;
}
bool Timer::setHeatingOffMorning(int time) {
	if (time < 720 && time > heatingOnMorning) {
		heatingOffMorning = time;
		return true;
	}
	return false;
}
bool Timer::setHeatingOnAfternoon(int time) {
	if (time < heatingOffAfternoon && time > 720) {
		heatingOnAfternoon = time;
		return true;
	}
	return false;
}
bool Timer::setHeatingOffAfternoon(int time) {
	if (time < 1440 && time > heatingOnAfternoon) {
		heatingOffAfternoon = time;
		return true;
	}
	return false;
}
bool Timer::setWaterOnMorning(int time) {
	if (time < waterOffMorning && time > 0) {
		waterOnMorning = time;
		return true;
	}
	return false;
}
bool Timer::setWaterOffMorning(int time) {
	if (time < 720 && time > waterOnMorning) {
		waterOffMorning = time;
		return true;
	}
	return false;
}
bool Timer::setWaterOnAfternoon(int time) {
	if (time < waterOffAfternoon && time > 720) {
		waterOnAfternoon = time;
		return true;
	}
	return false;
}
bool Timer::setWaterOffAfternoon(int time) {
	if (time < 1440 && time > waterOnAfternoon) {
		waterOffAfternoon = time;
		return true;
	}
	return false;
}

void Timer::setHeatingTimerState(bool state) {
	heatingTimerState = state;
}
void Timer::setWaterTimerState(bool state) {
	waterTimerState = state;
}

int Timer::getMidnight() {
	return midnight;
}

int Timer::getTime() {
	return (millis() / 60000) - midnight;
}
unsigned long inline Timer::ntpUnixTime(UIPUDP &udp)
{
	static int udpInited = udp.begin(123); // open socket on arbitrary port

	const char timeServer[] = "pool.ntp.org";  // NTP server

											   // Only the first four bytes of an outgoing NTP packet need to be set
											   // appropriately, the rest can be whatever.
	const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

	if (!udpInited)
		return 0;

	// Clear received data from possible stray received packets
	udp.flush();

	// Send an NTP request
	if (!(udp.beginPacket(timeServer, 123) // 123 is the NTP port
		&& udp.write((byte *)&ntpFirstFourBytes, 48) == 48
		&& udp.endPacket()))
		return 0;				// sending request failed

								// Wait for response; check every pollIntv ms up to maxPoll times
	const int pollIntv = 150;		// poll every this many ms
	const byte maxPoll = 15;		// poll up to this many times
	int pktLen;				// received packet length
	for (byte i = 0; i<maxPoll; i++) {
		if ((pktLen = udp.parsePacket()) == 48)
			break;
		delay(pollIntv);
	}
	if (pktLen != 48)
		return 0;				// no correct packet received

								// Read and discard the first useless bytes
								// Set useless to 32 for speed; set to 40 for accuracy.
	const byte useless = 40;
	for (byte i = 0; i < useless; ++i)
		udp.read();

	// Read the integer part of sending time
	unsigned long time = udp.read();	// NTP time
	for (byte i = 1; i < 4; i++)
		time = time << 8 | udp.read();
	time += (udp.read() > 115 - pollIntv / 8);

	// Discard the rest of the packet
	udp.flush();
	time = time - 2208988800ul; // Convert to unix time


	return time;
}
int Timer::dstOffset(unsigned long unixTime)
{
	time_t t = unixTime;
	int beginDSTDay = (14 - (1 + year(t) * 5 / 4) % 7);
	int beginDSTMonth = 3;
	int endDSTDay = (7 - (1 + year(t) * 5 / 4) % 7);
	int endDSTMonth = 11;
	if (((month(t) > beginDSTMonth) && (month(t) < endDSTMonth))
		|| ((month(t) == beginDSTMonth) && (day(t) > beginDSTDay))
		|| ((month(t) == beginDSTMonth) && (day(t) == beginDSTDay) && (hour(t) >= 2))
		|| ((month(t) == endDSTMonth) && (day(t) < endDSTDay))
		|| ((month(t) == endDSTMonth) && (day(t) == endDSTDay) && (hour(t) < 1)))
		return (3600);  //Add back in one hours worth of seconds - DST in effect
	else
		return (0);  //NonDST
}

unsigned long Timer::getNTPTime(UIPUDP &udp) {
	
	unsigned long time = ntpUnixTime(udp);
	return time + dstOffset(time);
}

bool Timer::setMidnightNTP(UIPUDP &udp) { // Funciton for setting midnight using NTP server
;
	unsigned long time = 0;

	time = getNTPTime(udp);
	if (time != 0) {
		
		
		int currentTime = (time / 60) % 1440;
		
		setMidnight((millis() / 60000) - currentTime);
		return true;
	}
	return false;
}