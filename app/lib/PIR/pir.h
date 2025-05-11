#pragma once

#include <utility>

#include <pinned.h>

namespace msentry {

	class PIRSensor : PinComponent<INPUT> {
	public:
		PIRSensor() : PinComponent<INPUT>() {};
		PIRSensor(int8_t pin) : PinComponent<INPUT>(pin) {}

		bool detect();

		unsigned long debounceTime{ 3000 };
		volatile unsigned long lastDebounce{ 0 };

		volatile bool moving{ false };
	};

} // namespace msentry

