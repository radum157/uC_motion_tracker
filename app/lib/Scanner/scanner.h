#pragma once

#include <stdint.h>
#include <ESP32Servo.h>

namespace msentry {

	class ScannerServo {
	public:
		ScannerServo() {};
		ScannerServo(int8_t pin) : pin(pin) {}
		~ScannerServo();

		void move();

		Servo servo;
		int8_t pin{ -1 };

		int idx{ 0 };
		int step{ 1 };

		static constexpr int nrAngles = 5;
		static const int angles[nrAngles];
	};

} // namespace msentry
