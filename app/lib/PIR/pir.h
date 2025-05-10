#pragma once

#include <utility>

#include <stdint.h>

namespace msentry {

	class PIRSensor {
	public:
		PIRSensor() {};
		PIRSensor(int8_t pin) : pin(pin) {}

		bool detect();

		unsigned long debounceTime{ 3000 };
		volatile unsigned long lastDebounce{ 0 };

		int8_t pin{ -1 };
	};

} // namespace msentry

