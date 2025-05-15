#include <sentry.h>

using namespace msentry;

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
		vTaskDelay(MS_TO_PORTTICKS(delayTime));
	}

	vTaskDelay(MS_TO_PORTTICKS(delayTime));
	step = -step;
}
