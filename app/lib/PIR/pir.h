#pragma once

#include <utility>

#include <pinned.h>

namespace msentry {

	class PIRSensor : PinComponent<INPUT_PULLDOWN> {
	public:
		PIRSensor() : PinComponent<INPUT_PULLDOWN>() {};
		PIRSensor(int8_t pin) : PinComponent<INPUT_PULLDOWN>(pin) {}

		bool detect();

		unsigned long debounceTime{ 3000 };
		volatile unsigned long lastDebounce{ 0 };

		volatile bool moving{ false };
	};

} // namespace msentry

