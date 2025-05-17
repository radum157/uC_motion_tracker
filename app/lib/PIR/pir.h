#pragma once

#define CALIBRATION_TIME 30000	// 30s
#define PIR_DELAY 3200			// 3.2s

namespace msentry {

	/**
	 * @brief
	 * PIR Sensor class
	 */
	class PIRSensor {
	public:
		PIRSensor() {};

		/**
		 * @return True if motion is detected after a custom debounce period
		 */
		bool detect();

		unsigned long lastMotion{ 0 };
		unsigned long debounceTime{ PIR_DELAY };

		unsigned long detectCnt{ 0 };
		unsigned long detectCntThold{ 1 };
	};

} // namespace msentry

