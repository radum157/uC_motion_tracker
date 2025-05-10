#include <sentry.h>

using namespace msentry;

#include <ESP32Servo.h>

SentryServo::~SentryServo()
{
	if (pin > 0) {
		servo.detach();
	}
}

void SentryServo::move()
{
	if (motionIdx < 0) {
		waitTime = 0;
		return;
	}

	servo.write(90);
	currentAngle = 90;
}

void SentryServo::smoothStep()
{
	int target = (step == -1) ? 0 : MAX_ANGLE;
	int delayTime = SWEEP_TIME / MAX_ANGLE;

	for (; currentAngle != target; currentAngle += step) {
		if (waitTime != 0) {
			move();
			return;
		}

		servo.write(currentAngle);
		delay(delayTime);
	}

	delay(delayTime);
	step = -step;
}
