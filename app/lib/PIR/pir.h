#pragma once

#define CALIBRATION_TIME 1000	// 1s

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

		unsigned long debounceTime{ 3000 };
		volatile unsigned long lastDebounce{ 0 };

		volatile bool moving{ false };
	};

} // namespace msentry

