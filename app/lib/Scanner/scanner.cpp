#include <scanner.h>
#include <util.h>

using namespace msentry;

ScannerServo::ScannerServo(uint8_t pin)
{
	servo.setPeriodHertz(SERVO_FREQ);
	servo.attach(SENTRY_PIN, SERVO_MIN_PW, SERVO_MAX_PW);

	servo.write(scannerAngles[idx]);
};

ScannerServo::~ScannerServo()
{
	if (servo.attached()) {
		servo.detach();
	}
}

void ScannerServo::move()
{
	idx += step;

	if (idx == scannerNrAngles) {
		step = -1;
		idx = scannerNrAngles - 1;
	} else if (idx == -1) {
		step = 1;
		idx = 0;
	}

	servo.write(scannerAngles[idx]);
}
