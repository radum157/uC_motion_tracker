#include <sentry.h>

using namespace msentry;

SentryServo::~SentryServo()
{
	if (pin > 0) {
		servo.detach();
	}
}

static inline int scannerToSentryAngle(int angle)
{
	int dist = angle - 90;
	return angle - dist / 3;
}

void SentryServo::track()
{
	int angle = msentry::scannerAngles[motionIdx];
	angle = scannerToSentryAngle(angle);

	servo.write(angle);
	currentAngle = angle;

	motionIdx = -1;
}

void SentryServo::smoothStep()
{
	int target = (step == -1) ? 0 : MAX_ANGLE;
	int delayTime = SWEEP_TIME / MAX_ANGLE;

	for (; currentAngle != target; currentAngle += step) {
		if (waitTime != 0) {
			track();
			return;
		}

		servo.write(currentAngle);
		vTaskDelay(delayTime / portTICK_PERIOD_MS);
	}

	vTaskDelay(delayTime / portTICK_PERIOD_MS);
	step = -step;
}
