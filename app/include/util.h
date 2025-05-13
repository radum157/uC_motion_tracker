#pragma once

#define BAUD_RATE 115200		// serial baud rate

#define SWEEP_TIME 4000			// 4s
#define WAIT_TIME 1000			// 1s
#define MAX_ANGLE 180

#define SCAN_TIME 4000			// 4s

#define SERVO_FREQ 50			// 50Hz
#define SERVO_MIN_PW 500
#define SERVO_MAX_PW 2400

#define BUZZER_FREQ 200			// 200Hz
#define ALERT_TIME 500			// 500ms

#define BUZZER_PIN 18			// D18
#define SCANNER_PIN 19			// D19
#define SENTRY_PIN 22			// D22
#define PIR_PIN 23				// D23

#define TASK_STACK_SIZE 2048	// vtask stack size

namespace msentry {

	const int scannerNrAngles = 5;
	// Scanner preset angles. Used in sentry to calculate turn angle
	const int scannerAngles[] = {
		0, 45, 90, 135, 180
	};

} // namespace msentry

