#include <wifi.h>

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

extern volatile bool wifiPrint;
extern SemaphoreHandle_t serialMux;

const char *ssid = "<ssid>";
const char *password = "<password>";

const char *server = "http://192.168.1.100:8000";

void connect_loop()
{
	Serial.print("Connecting to wifi");

	for (int i = 0; i < CONNECT_RETRIES && WiFi.status() != WL_CONNECTED; i++) {
		vTaskDelay(100 / portTICK_PERIOD_MS);
		Serial.print(".");
	}
	Serial.print("\n");

	Serial.println("Connected");
}

void msentry::wifi_setup()
{
	Serial.println("WiFi setup...");

	WiFi.begin(ssid, password);

	Serial.print("IP Adress: ");
	Serial.println(WiFi.localIP());

	Serial.println("WiFi setup ended");
}

bool sendHTTP()
{
	HTTPClient http;

	http.begin(server);
	http.addHeader("Content-Type", "application/json");

	String jsonPayload = "{\"message\":\"" + String("fmm") + "\"}";
	int responseCode = http.POST(jsonPayload);

	http.end();

	return responseCode >= 200 && responseCode < 300;
}

bool printDate()
{
	if (!xSemaphoreTake(serialMux, SEM_WAIT_TIME)) {
		return false;
	}

	if (WiFi.status() != WL_CONNECTED) {
		connect_loop();
	}

	xSemaphoreGive(serialMux);

	if (WiFi.status() != WL_CONNECTED) {
		return false;
	}

	if (!sendHTTP()) {
		return false;
	}

	xSemaphoreTake(serialMux, portMAX_DELAY);

	Serial.println("Message posted through wifi");

	xSemaphoreGive(serialMux);

	return true;
}

void msentry::wifi_loop(void *param)
{
	while (true) {
		if (wifiPrint) {
			while (!printDate()) {
				vTaskDelay(10 / portTICK_PERIOD_MS);
			}

			wifiPrint = false;
		}

		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
}
