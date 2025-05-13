#pragma once

#define TONE_CHANNEL 2		//! may overlap other ledc channels
#define TONE_RESOLUTION 8	// 8-bit resolution

#include <stdint.h>

namespace msentry {

	/**
	 * @brief
	 * Plays a tone with PWM on a given output pin
	 */
	void tonePWM(uint8_t pin, unsigned int frequency, unsigned long duration = 0);

	/**
	 * @brief
	 * Stops playing a tone
	 */
	void noTonePWM(uint8_t pin);

} // namespace msentry
