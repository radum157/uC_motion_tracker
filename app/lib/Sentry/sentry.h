#pragma once

#include <stdint.h>
#include <ESP32Servo.h>

#include <util.h>

namespace msentry {

	class SentryServo {
	public:
		SentryServo() {};
		SentryServo(int8_t pin) : pin(pin) {}
		~SentryServo();

		void move();

		void smoothStep();

		Servo servo;
		int8_t pin{ -1 };

		int currentAngle{ 0 };
		int step{ 1 };

		volatile uint32_t waitTime{ 0 };
		volatile int motionIdx{ -1 };
	};

} // namespace msentry
