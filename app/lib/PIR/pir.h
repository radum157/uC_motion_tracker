#pragma once

#include <utility>

namespace msentry {

	class PIRSensor {
	public:
		PIRSensor() {};

		bool detect();

		unsigned long debounceTime{ 3000 };
		volatile unsigned long lastDebounce{ 0 };

		volatile bool moving{ false };
	};

} // namespace msentry

