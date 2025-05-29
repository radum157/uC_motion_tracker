#include <pir.h>

#include <Arduino.h>

using namespace msentry;

bool PIRSensor::detect()
{
	unsigned long time = millis();
	if (time - lastMotion < debounceTime) {
		return false;
	}

	// Debounce
	detectCnt++;
	if (detectCnt < detectCntThold) {
		return false;
	}

	detectCnt = 0;
	lastMotion = time;
	return true;
}
