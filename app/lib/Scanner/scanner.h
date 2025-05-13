#pragma once

#include <stdint.h>
#include <ESP32Servo.h>
#include <util.h>

namespace msentry {

	class ScannerServo {
	public:
		ScannerServo() {};
		~ScannerServo();

		void move();

		Servo servo;

		int idx{ msentry::scannerNrAngles / 2 };
		int step{ 1 };
	};

} // namespace msentry
