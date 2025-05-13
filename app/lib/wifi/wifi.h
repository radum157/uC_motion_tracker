#pragma once

#define CONNECT_RETRIES 10 // wifi connection retry periods

namespace msentry {

	/**
	 * @brief
	 * Connects to wifi and sets the time zone
	 */
	void wifiSetup();

	/**
	 * @brief
	 * Loop function for wifi handling
	 */
	void runWifi(void *param);

} // namespace msentry

