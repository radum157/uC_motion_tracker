#include <pir.h>

#include <Arduino.h>

using namespace msentry;

bool PIRSensor::detect()
{
	unsigned long time = millis();

	if (time - lastDebounce > debounceTime) {
		lastDebounce = time;
		return true;
	}

	return false;
}

// std::pair<int8_t, unsigned long> PIRSensor::detect()
// {
// 	int value = digitalRead(pin);
// 	unsigned long time = millis();

// 	if (value == HIGH && !motion
// 		&& (time - lastDebounce > debounceTime)) {
// 		motion = true;
// 		lastDebounce = time;

// 		return { 1, time };
// 	} else if (value == LOW && motion
// 		&& (time - lastDebounce > debounceTime)) {
// 		motion = false;
// 		lastDebounce = time;

// 		return { -1, time };
// 	}

// 	return { 0, time };
// }
