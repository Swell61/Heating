// 
// 
// 

#include "Timer.h"

Timer::Timer() { // Constructor
	setSyncProvider(RTC.get); // Synchronise the Time library with the RTC clock
}

int Timer::getTimeInMinutes() {
	if (((hour() * 60) + minute() + (dstOffset(now()) / 60)) >= 1440) { // If daylight savings pushes clock over at midnight...
		return ((hour() * 60) + minute()) + (dstOffset(now()) / 60) - 1440; // Instead of displaying 24:XX, loop round to display 00:XX
	}
	else {
		return ((hour() * 60) + minute()) + (dstOffset(now()) / 60); // Return the current system time in minutes
	}
}

bool Timer::setSystemTime(unsigned long time) { // Function for setting the RTC clock
	time_t t = time; // Store the current time
	RTC.set(t); // Set the RTC to the current time
	setTime(t); // Set the Time library to the current time (instead of waiting for library to sync to RTC)
}

bool Timer::setSystemHour(int newHour) {
	if (newHour >= 23) { // If trying to go past 23 hours...
		newHour = 0; // Loop back to 0
	}
	else if (newHour <= 0) { // If trying to go behind 0 hours...
		newHour = 23; // Loop back to 23 hours
	}
	setTime(newHour, minute(), 0, day(), month(), year()); // Set new time and reset seconds to 0
	RTC.set(now()); // Set the RTC to the new time
	return true;
}

bool Timer::setSystemMinute(int newMinute) {
	if (newMinute >= 59) { // If trying to go past 59 minutes...
		newMinute = 0; // Loop back to 0
	}
	else if (newMinute <= 0) { // If trying to go behind 0 minutes...
		newMinute = 59;	// Loop back to 59 minutes
	}
	setTime(hour(), newMinute, 0, day(), month(), year()); // Set new time and reset seconds to 0
	RTC.set(now()); // Set the RTC to the new time
	return true;
}

int Timer::getHour() {
	return hour(); // Return the current system hours
}

int Timer::getMinute() {
	return minute(); // Return the current system minutes
}

bool Timer::getHeatingTimerStatus() { // Function for getting the status of the heating
	// If the heating should be on based on the timer, return true. Otherwise return false
	int time = getTimeInMinutes();
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
	int time = getTimeInMinutes() + 1;
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

unsigned long Timer::getNTPTime(UIPUDP &udp) {
	return ntpUnixTime(udp);
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
	udp.stop();

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



