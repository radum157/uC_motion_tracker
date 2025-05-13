#include <wifi.h>

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <time.h>

#define DATE_PREFIX "Last detection time\n"

#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC (3 * 60 * 60)

extern volatile bool wifiPrint;

namespace msentry {

	const char *ssid = "messi";
	const char *password = "WaHo2859";

	bool timeInitialized{ false };

	WebServer server(80);

	String date{ "Idle..." };

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
              margin-bottom: 30px;
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
          </style>
        </head>
        <body>
          <div class="container">
            <h1>ESP32 Event Log</h1>
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

	void serverRoot()
	{
		server.send(200, "text/html", html);
	}

	void serverDetect()
	{
		server.send(200, "plain/text", date);
	}

	bool initTime(bool log = true)
	{
		// NTP time synchronization
		configTime(GMT_OFFSET_SEC, 0, NTP_SERVER);

		// Timeout after 10 seconds
		unsigned long start = millis();

		while (millis() - start < 10000) {
			time_t now;
			struct tm timeinfo;

			if (time(&now) && getLocalTime(&timeinfo)) {
				return true;
			}

			vTaskDelay(500 / portTICK_PERIOD_MS);
		}

		return false;
	}

	void wifiSetup()
	{
		// WiFi init
		Serial.println("WiFi setup...");
		WiFi.mode(WIFI_AP);

		while (WiFi.status() != WL_CONNECTED) {
			WiFi.begin(ssid, password);
			Serial.print("Connecting to WiFi.");

			for (int i = 0; i < CONNECT_RETRIES && WiFi.status() != WL_CONNECTED; i++) {
				delay(1000);
				Serial.print(".");
			}
			Serial.print("\n");
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
		server.on("/", serverRoot);
		server.on("/detect", serverDetect);

		server.begin();
		Serial.println("WiFi setup ended");
	}

	void wifiReconnect()
	{
		while (WiFi.status() != WL_CONNECTED) {
			WiFi.begin(ssid, password);

			for (int i = 0; i < CONNECT_RETRIES && WiFi.status() != WL_CONNECTED; i++) {
				delay(1000);
			}

			server.handleClient();
		}
	}

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
			if (WiFi.status() != WL_CONNECTED) {
				date = "WiFi is down :(\nReconnecting...";
				wifiReconnect();
				continue;
			}

			if (!timeInitialized) {
				date = "Time not synchronized :<\nRetrying...";
				timeInitialized = initTime();
				continue;
			}

			if (wifiPrint) {
				refreshDate();
				wifiPrint = false;
			}

			server.handleClient();
			vTaskDelay(50 / portTICK_PERIOD_MS);
		}
	}

} // namespace msentry
