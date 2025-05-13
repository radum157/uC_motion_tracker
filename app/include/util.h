#pragma once


#define BAUD_RATE 115200

#define SWEEP_TIME 4000		// 4s
#define WAIT_TIME 1000		// 1s
#define MAX_ANGLE 180

#define SCAN_TIME 4000		// 4s

#define SERVO_FREQ 50		// 50Hz
#define SERVO_MIN_PW 500
#define SERVO_MAX_PW 2400

#define BUZZER_FREQ 200		// 200Hz
#define ALERT_TIME 500		// 500ms

#define BUZZER_PIN 18		// D18
#define SCANNER_PIN 19		// D19
#define SENTRY_PIN 22		// D22
#define PIR_PIN 23			// D23

#define TASK_STACK_SIZE 800

namespace msentry {

	static constexpr int scannerNrAngles = 5;
	static const int scannerAngles[] = {
		0, 45, 90, 135, 180
	};

} // namespace msentry

