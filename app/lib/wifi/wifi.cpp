#include <wifi.h>

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <sys/time.h>

#define DATE_PREFIX "Last detection time: "

extern volatile bool wifiPrint;

namespace msentry {

	const char *ssid = "<ssid>";
	const char *password = "<password>";

	WebServer server(80);

	void serverRoot()
	{
		String html = R"rawliteral(
		<!DOCTYPE HTML><html>
		<head>
		  <title>ESP32 Event Log</title>
		</head>
		<body>
		  <h1>ESP32 Event Log</h1>
		  <div id='log'>Waiting for event...</div>
		  <script>
			setInterval(() => {
			  fetch('/detect')
				.then(res => res.text())
				.then(txt => document.getElementById('log').innerText = txt);
			}, 2000);
		  </script>
		</body>
		</html>
	  )rawliteral";

		server.send(200, "text/html", html);
	}

	String date{ "Idle..." };

	void serverDetect()
	{
		server.send(200, "plain/text", date);
	}

	void wifi_setup()
	{
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

		server.on("/", serverRoot);
		server.on("/detect", serverDetect);

		server.begin();
		Serial.println("WiFi setup ended");
	}

	void connect_loop()
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
		struct timeval tv;
		gettimeofday(&tv, nullptr);

		long seconds = tv.tv_sec;
		long millis = tv.tv_usec / 1000 - 500;

		if (millis < 0) {
			millis += 1000;
			seconds -= 1;
		}

		struct tm *timeinfo = localtime(&seconds);
		char buffer[64];
		strftime(buffer, sizeof(buffer), "Date: %Y-%m-%d\tTime: %H:%M:%S", timeinfo);

		char fullDate[128];
		snprintf(fullDate, sizeof(fullDate), DATE_PREFIX "%s.%03ld", buffer, millis);

		date = fullDate;
	}

	void wifi_loop(void *param)
	{
		WiFiClient client;

		while (true) {
			if (WiFi.status() != WL_CONNECTED) {
				date = "WiFi is down :(\nReconnecting...";
				connect_loop();
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
