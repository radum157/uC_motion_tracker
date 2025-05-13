#include <pir.h>

#include <Arduino.h>

using namespace msentry;

bool PIRSensor::detect()
{
	unsigned long time = millis();

	// Ignore PIR movement
	if (moving) {
		lastDebounce = time;
		return false;
	}

	// Debounce
	if (time - lastDebounce > debounceTime) {
		lastDebounce = time;
		return true;
	}

	return false;
}
