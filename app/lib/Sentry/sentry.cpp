#include <sentry.h>

using namespace msentry;

SentryServo::SentryServo(uint8_t pin)
{
	servo.setPeriodHertz(SERVO_FREQ);
	servo.attach(SENTRY_PIN, SERVO_MIN_PW, SERVO_MAX_PW);

	servo.write(90);
};

SentryServo::~SentryServo()
{
	if (servo.attached()) {
		servo.detach();
	}
}

namespace msentry {

	/**
	 * @brief
	 * Angle transformation in order for scanner and sentry to point to the same spot
	 */
	inline int scannerToSentryAngle(int angle)
	{
		int dist = angle - 90;
		return angle - dist / 3;
	}

} // namespace msentry

void SentryServo::track()
{
	int angle = scannerAngles[motionIdx];
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
		// Check for interrupts
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
