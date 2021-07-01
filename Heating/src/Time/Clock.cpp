#include "Clock.h"

Clock::Clock() {
    setSyncProvider(RTC.get);
}

unsigned short int Clock::getTimeInMinutes() {
    unsigned short int currentTime = ((hour() * 60) + minute() + (dstOffset(now()) / 60));
	return (currentTime >= 1440) ? currentTime - 1440 : currentTime; // If daylight savings pushes clock over at midnight
}

unsigned short int Clock::dstOffset(unsigned long unixTime)
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

bool Clock::setSystemTime(unsigned long time) { // Function for setting the RTC clock
	time_t t = time; // Store the current time
	setTime(t); // Set the Time library to the current time (instead of waiting for library to sync to RTC)
	return RTC.set(now()) == 0; // Set the RTC to the current time
}


bool Clock::synchroniseWithNtp(UIPUDP& udp) {
    unsigned long currentNtpTime = ntpUnixTime(udp);
	Serial.println(currentNtpTime);
    if (currentNtpTime != 0) {
        return setSystemTime(currentNtpTime);
    }
    return false;
}

unsigned long Clock::ntpUnixTime(UIPUDP &udp)
{
	static int udpInited = udp.begin(123); // open socket on arbitrary port

	const char timeServer[] = "pool.ntp.org";  // NTP server

											   // Only the first four bytes of an outgoing NTP packet need to be set
											   // appropriately, the rest can be whatever.
	const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

	if (!udpInited) {
		return 0;
	}
	// Clear received data from possible stray received packets
	udp.flush();

	// Send an NTP request
	if (!(udp.beginPacket(timeServer, 123) // 123 is the NTP port
		&& udp.write((byte *)&ntpFirstFourBytes, 48) == 48
		&& udp.endPacket())) {
		return 0;				// sending request failed

		}
								// Wait for response; check every pollIntv ms up to maxPoll times
	const int pollIntv = 150;		// poll every this many ms
	const byte maxPoll = 15;		// poll up to this many times
	int pktLen;				// received packet length
	for (byte i = 0; i<maxPoll; i++) {
		if ((pktLen = udp.parsePacket()) == 48)
			break;
		delay(pollIntv);
	}
	if (pktLen != 48) {
		return 0;				// no correct packet received
	}

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

bool Clock::setSystemHour(unsigned char newHour) {
	if (newHour >= 23) { // If trying to go past 23 hours...
		newHour = 0; // Loop back to 0
	}
	else if (newHour <= 0) { // If trying to go behind 0 hours...
		newHour = 23; // Loop back to 23 hours
	}
	setTime(newHour, minute(), 0, day(), month(), year()); // Set new time and reset seconds to 0
	return RTC.set(now()) == 0; // Set the RTC to the new time
}

bool Clock::setSystemMinute(unsigned char newMinute) {
	if (newMinute >= 59) { // If trying to go past 59 minutes...
		newMinute = 0; // Loop back to 0
	}
	else if (newMinute <= 0) { // If trying to go behind 0 minutes...
		newMinute = 59;	// Loop back to 59 minutes
	}
	setTime(hour(), newMinute, 0, day(), month(), year()); // Set new time and reset seconds to 0
	return RTC.set(now()) == 0; // Set the RTC to the new time
}


bool Clock::adjustTime(TimeComponent timeComponent, ValueAdjustment adjustment) {
	switch (timeComponent) {
		case TimeComponent::HOURS_TENS: {
			return adjustTime(&setSystemHour, hour(), adjustment, 10);
		}
		case TimeComponent::HOURS_ONES: {
			return adjustTime(&setSystemHour, hour(), adjustment, 1);
		}
		case TimeComponent::MINUTES_TENS: {
			return adjustTime(&setSystemMinute, minute(), adjustment, 10);
		}
		case TimeComponent::MINUTES_ONES: {
			return adjustTime(&setSystemMinute, minute(), adjustment, 1);
		}
	}
	return false;
}

bool Clock::adjustTime(bool (Clock::*updateFunction)(unsigned char), int currentValue, ValueAdjustment adjustmentDirection, int adjustmentValue) {
	switch (adjustmentDirection) {
		case ValueAdjustment::UP: {
			return (this->*updateFunction)(currentValue + adjustmentValue);
		}
		case ValueAdjustment::DOWN: {
			return (this->*updateFunction)(currentValue - adjustmentValue);
		}
	}
	return false;
}

unsigned short int Clock::timeDifferenceFromNowInMinutes(unsigned short int startMinutes) {
	return absoluteTimeDifferenceInMinutes(startMinutes, getTimeInMinutes());
}

unsigned short int Clock::absoluteTimeDifferenceInMinutes(unsigned short int startMinutes, unsigned short int endMinutes) {
	unsigned short int result = endMinutes - startMinutes;
	if (result > MINUTES_IN_A_DAY) {
		/* If start minutes is before tick over to 0 (before midnight) and end minutes is after tick over to 0 (after midnight)
		 * result will underflow. Adding MINUTES_IN_A_DAY will overflow result to the difference between startMinutes and 
		 * endMinutes.
		 * E.g. with unsigned short ints: startMinutes = 1400 minutes, endMinutes = 80 minutes
		 * 								  endMinutes - startMinutes = 64,216
		 * 								  64,216 + MINUTES_IN_A_DAY = 120
		*/
		result += MINUTES_IN_A_DAY;
	}
	return result;
}