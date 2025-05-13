#pragma once

#define TONE_CHANNEL 2		// be careful with this, as it may overlap other ledc channels
#define TONE_RESOLUTION 8	// 8-bit resolution

#include <stdint.h>

namespace msentry {

	void tonePWM(uint8_t pin, unsigned int frequency, unsigned long duration = 0);
	void noTonePWM(uint8_t pin);

} // namespace msentry
