#pragma once

#define CONNECT_RETRIES 10
#define SEM_WAIT_TIME 500

namespace msentry {

	void wifiSetup();
	void runWifi(void *param);

} // namespace msentry

