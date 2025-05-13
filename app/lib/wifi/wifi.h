#pragma once

#define CONNECT_RETRIES 10
#define SEM_WAIT_TIME 500

namespace msentry {

	void wifi_setup();
	void wifi_loop(void *param);

} // namespace msentry

