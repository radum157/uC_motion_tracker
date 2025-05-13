#pragma once

#include <stdint.h>
#include <ESP32Servo.h>

#include <util.h>

namespace msentry {

	class SentryServo {
	public:
		SentryServo() {};
		~SentryServo();

		void track();
		void smoothStep();

		Servo servo;

		int currentAngle{ 90 };
		int step{ 1 };

		volatile uint32_t waitTime{ 0 };
		volatile int motionIdx{ -1 };
	};

} // namespace msentry
