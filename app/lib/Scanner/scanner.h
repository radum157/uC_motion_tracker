#pragma once

#include <stdint.h>
#include <ESP32Servo.h>
#include <util.h>

namespace msentry {

	/**
	 * Servo class for scanner
	 */
	class ScannerServo {
	public:
		ScannerServo() {};
		~ScannerServo();

		/**
		 * @brief
		 * Moves the scanner to the angle at @a idx in @a scannerAngles
		 */
		void move();

		Servo servo;

		int idx{ scannerNrAngles / 2 };
		int step{ 1 };
	};

} // namespace msentry
