#pragma once

#include <stdint.h>
#include <Arduino.h>

namespace msentry {

	template<uint8_t mode>
	class PinComponent {
	public:
		PinComponent() {}
		PinComponent(int8_t pin) : pin(pin)
		{
			pinMode(pin, mode);
		}

		int8_t pin{ -1 };
	};

} // namespace msentry

