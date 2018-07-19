// 
// 
// 

#include "Timer.h"

bool Timer::getHeatingTimerStatus() {
	int time = getTime() + 1;
	if ((time > heatingOnMorning) && (time < heatingOffMorning)) {
		return true;
	}
	
	else if ((time > heatingOnAfternoon) && (time < heatingOffAfternoon)) {
		return true;
	}
	else {
		return false;
	}
}

bool Timer::getWaterTimerStatus() {
	int time = getTime() + 1;
	if ((time > waterOnMorning) && (time < waterOffMorning)) {
		return true;
	}
	
	else if (((time) > waterOnAfternoon) && (time < waterOffAfternoon)) {
		return true;

	}
	else {
		return false;
	}
}

bool Timer::setMidnight(int time) {
		if ((millis() / 60000) - time <= 1440) {
				midnight = time;
				return true;
	}
	
}
unsigned long inline Timer::getNTPtime(UIPUDP &udp)	{
		static int udpInited = udp.begin(123); // open socket on arbitrary port

		const char timeServer[] = "pool.ntp.org";  // NTP server

												   // Only the first four bytes of an outgoing NTP packet need to be set
												   // appropriately, the rest can be whatever.
		const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

												   // Fail if WiFiUdp.begin() could not init a socket
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

		// Round to the nearest second if we want accuracy
		// The fractionary part is the next byte divided by 256: if it is
		// greater than 500ms we round to the next second; we also account
		// for an assumed network delay of 50ms, and (0.5-0.05)*256=115;
		// additionally, we account for how much we delayed reading the packet
		// since its arrival, which we assume on average to be pollIntv/2.
		time += (udp.read() > 115 - pollIntv / 8);

		// Discard the rest of the packet
		udp.flush();
		time = time - 2208988800ul; // Convert to unix time


		return time + dstOffset(time);
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

bool Timer::setMidnightNTP(bool) {
	Serial.println("Getting NTP time");
	EthernetUDP udp;
	unsigned long time = 0;

	time = getNTPtime(udp);
	if (time != 0) {
		Serial.println(time);
		int currentTime = (time / 60) % 1440;
		Serial.println(currentTime);
		setMidnight((millis() / 60000) - currentTime);
		return true;
	}
	return false;
}
void Timer::checkMidnight() {
	if (getTime() >= 1440) {
		midnight = millis() / 60000;
		//setMidnightNTP(false);
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