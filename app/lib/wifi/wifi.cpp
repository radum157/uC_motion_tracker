#include <wifi.h>

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <util.h>

#include <time.h>

#define DATE_PREFIX "Last detection time\n"

#define NTP_SERVER "pool.ntp.org"		// Time sync server
#define GMT_OFFSET_SEC (3 * 60 * 60)	// GMT offset

extern bool wifiPrint;
extern SemaphoreHandle_t serialMux;

namespace msentry {

	// Credentials
	const char *ssid = "<ssid>";
	const char *password = "<password>";

	bool timeInitialized{ false };

	// Wifi variables
	WebServer server(80);
	String date{ "Idle..." };

	// Site HTML with some styling
	String html = R"rawliteral(
		<!DOCTYPE HTML><html>
		<head>
		  <title>ESP32 Event Log</title>
		  <meta name="viewport" content="width=device-width, initial-scale=1">
		  <style>
			body {
			  font-family: Arial, Helvetica, sans-serif;
			  margin: 0;
			  padding: 20px;
			  background-color: #f5f5f5;
			  color: #333;
			}
			h1 {
			  color: #0066cc;
			  text-align: center;
			  margin-bottom: 10px;
			}
			#log {
			  background-color: white;
			  border: 1px solid #ddd;
			  border-radius: 8px;
			  padding: 20px;
			  min-height: 60px;
			  font-size: 18px;
			  box-shadow: 0 2px 5px rgba(0,0,0,0.1);
			}
			.container {
			  max-width: 800px;
			  margin: 0 auto;
			}
			.status {
			  text-align: center;
			  margin-top: 20px;
			  font-size: 14px;
			  color: #666;
			}
			.messi-img {
			  display: block;
			  margin: 0 auto 20px auto;
			  max-width: 100%;
			  border-radius: 12px;
			  box-shadow: 0 2px 10px rgba(0,0,0,0.2);
			}
		  </style>
		</head>
		<body>
		  <div class="container">
			<h1>ESP32 Event Log</h1>
			<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/c/c1/Lionel_Messi_20180626.jpg/440px-Lionel_Messi_20180626.jpg" alt="Lionel Messi" class="messi-img">
			<div id='log'>Waiting for event...</div>
			<div class="status">Refreshing every 2 seconds...</div>
		  </div>
		  <script>
			setInterval(() => {
			  fetch('/detect')
				.then(res => res.text())
				.then(txt => {
				  document.getElementById('log').innerText = txt;
				  console.log('Updated: ' + new Date().toLocaleTimeString());
				})
				.catch(err => {
				  console.error('Error fetching data:', err);
				});
			}, 2000);
		  </script>
		</body>
		</html>
	  )rawliteral";

	/**
	 * Synchronizes datetime with NTP server
	 */
	bool initTime(bool log = true)
	{
		// NTP time synchronization
		configTime(GMT_OFFSET_SEC, 0, NTP_SERVER);

		// Timeout after 10 seconds
		unsigned long start = millis();

		while (millis() - start < CONNECT_TIMEOUT) {
			time_t now;
			struct tm timeinfo;

			if (time(&now) && getLocalTime(&timeinfo)) {
				return true;
			}

			vTaskDelay(MS_TO_PORTTICKS(500));
		}

		return false;
	}

	inline void startServer()
	{
		/**
		 * '/' endpoint
		 */
		server.on("/", [&]() {
			server.send(200, "text/html", html);
			});

		/**
		 * '/detect' endpoint
		 *
		 * @return The last detection datetime
		 */
		server.on("/detect", [&]() {
			server.send(200, "plain/text", date);
			});

		server.begin();
	}

	void wifiSetup()
	{
		// WiFi init
		Serial.println("WiFi setup...");

		WiFi.mode(WIFI_AP);

		// Connection retry loop
		WiFi.begin(ssid, password);
		Serial.print("Connecting to WiFi.");

		// Split delay for visual effect
		for (int j = 0; j < 5; j++) {
			delay(1000);
			Serial.print(".");
		}
		Serial.print("\n");

		if (WiFi.status() != WL_CONNECTED) {
			Serial.println("WiFi setup failed");
			return;
		}

		Serial.print("IP Adress: ");
		Serial.println(WiFi.localIP());

		// Datetime init
		Serial.println("NTP time sync...");
		timeInitialized = initTime();

		if (timeInitialized) {
			Serial.println("Time synchronized with NTP server");
		} else {
			Serial.println("Time synchronization failed");
		}

		// Server start
		startServer();
		Serial.println("WiFi setup ended");
	}

	/**
	 * Wifi (re)connect loop
	 */
	void wifiReconnect()
	{
		xSemaphoreTake(serialMux, portMAX_DELAY);

		Serial.println("WiFi down. Reconnecting...");

		xSemaphoreGive(serialMux);

		WiFi.mode(WIFI_AP);

		while (WiFi.status() != WL_CONNECTED) {
			WiFi.begin(ssid, password);
			vTaskDelay(MS_TO_PORTTICKS(CONNECT_TIMEOUT));
		}

		xSemaphoreTake(serialMux, portMAX_DELAY);

		Serial.print("New IP: ");
		Serial.println(WiFi.localIP());

		xSemaphoreGive(serialMux);

		startServer();
	}

	/**
	 * Updates the date upon detection interrupt
	 */
	void refreshDate()
	{
		struct tm timeinfo;
		char buffer[64];

		if (!getLocalTime(&timeinfo)) {
			date = "Error: Could not get current time";
			return;
		}

		// Format date and time
		strftime(buffer, sizeof(buffer), "Date: %Y-%m-%d\nTime: %H:%M:%S", &timeinfo);

		struct timeval tv;
		gettimeofday(&tv, nullptr);
		long millis = tv.tv_usec / 1000;

		// Combine everything
		char fullDate[128];
		snprintf(fullDate, sizeof(fullDate), "%s%s.%03ld", DATE_PREFIX, buffer, millis);

		date = fullDate;
	}

	void runWifi(void *param)
	{
		WiFiClient client;

		while (true) {
			// Check for wifi connection
			if (WiFi.status() != WL_CONNECTED) {
				wifiReconnect();
				continue;
			}

			// Check for datetime sync
			if (!timeInitialized) {
				xSemaphoreTake(serialMux, portMAX_DELAY);

				Serial.println("Running NTP sync...");

				xSemaphoreGive(serialMux);

				timeInitialized = initTime();

				xSemaphoreTake(serialMux, portMAX_DELAY);

				Serial.println("Sync done");

				xSemaphoreGive(serialMux);
				continue;
			}

			// Interrupt signaled
			if (wifiPrint) {
				refreshDate();
				wifiPrint = false;
			}

			// Handle requests
			server.handleClient();
			vTaskDelay(MS_TO_PORTTICKS(200));
		}
	}

} // namespace msentry
