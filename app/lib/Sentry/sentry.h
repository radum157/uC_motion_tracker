#pragma once

#include <stdint.h>
#include <ESP32Servo.h>

#include <util.h>

namespace msentry {

	class SentryServo {
	public:
		SentryServo() {};
		~SentryServo();

		/**
		 * @brief
		 * Moves the scanner according to the angle at @a motionIdx in @a scannerAngles
		 */
		void track();

		/**
		 * @brief
		 * Angle sweep from 0 to 180 and back
		 */
		void smoothStep();

		Servo servo;

		int currentAngle{ 90 };
		int step{ 1 };

		volatile uint32_t waitTime{ 0 };
		volatile int motionIdx{ -1 };
	};

} // namespace msentry
