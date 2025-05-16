#pragma once

#define BAUD_RATE 115200		// serial baud rate

#define SWEEP_TIME 4000			// 4s
#define WAIT_TIME 1000			// 1s
#define MAX_ANGLE 180

#define SCAN_TIME 5000000		// 5s

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

#ifdef pdMS_TO_TICKS
#define MS_TO_PORTTICKS(ms) pdMS_TO_TICKS(ms)
#else
#define MS_TO_PORTTICKS(ms) (ms / 1000)
#endif

namespace msentry {

	const int scannerNrAngles = 5;
	// Scanner preset angles. Used in sentry to calculate turn angle
	const int scannerAngles[] = {
		25, 50, 90, 130, 155
	};

} // namespace msentry

