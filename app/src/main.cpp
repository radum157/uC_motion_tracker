#include <Arduino.h>
#include <esp_bt.h>

#include <util.h>
#include <pir.h>
#include <sentry.h>
#include <scanner.h>
#include <tonePWM.h>
#include <wifi.h>

using namespace msentry;

PIRSensor pir;
SentryServo sentry;
ScannerServo scanner;

volatile bool motion{ false };
volatile bool scannerMove{ false };
volatile bool wifiPrint{ false };

hw_timer_t *scanTimer{ nullptr };
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void print_time(unsigned long millis);

void init_pir();
void init_servo();

void IRAM_ATTR scanISR();
void IRAM_ATTR pirISR();

void scanner_loop(void *param);


void power_setup()
{
	Serial.println("Power setup...");

	btStop();
	esp_bt_controller_disable();

	Serial.println("Power setup ended");
}

void setup()
{
	Serial.begin(BAUD_RATE);
	Serial.println("Beginning Setup");

	init_pir();

	power_setup();
	wifi_setup();

	init_servo();

	pinMode(BUZZER_PIN, OUTPUT);
	noTonePWM(BUZZER_PIN);

	delay(10000);

	scanTimer = timerBegin(0, 80, true); // timer 0, prescaler 80 (1 tick per 80us), count up
	timerAttachInterrupt(scanTimer, &scanISR, true);

	timerAlarmWrite(scanTimer, 2000000, true);

	xTaskCreatePinnedToCore(scanner_loop, "Scanner", TASK_STACK_SIZE, NULL, 1, NULL, 1);
	xTaskCreatePinnedToCore(wifi_loop, "WiFi", TASK_STACK_SIZE, NULL, 0, NULL, 1);

	Serial.println("Ending Setup");
	tonePWM(BUZZER_PIN, BUZZER_FREQ, ALERT_TIME);

	timerAlarmEnable(scanTimer);
}

void loop()
{
	if (motion) {
		motion = false;

		Serial.print("Motion detected at: ");
		print_time(pir.lastDebounce);

		tonePWM(BUZZER_PIN, BUZZER_FREQ, ALERT_TIME);

		vTaskDelay(sentry.waitTime / portTICK_PERIOD_MS);
		sentry.waitTime = 0;
	}

	sentry.smoothStep();
}

void scanner_loop(void *param)
{
	while (true) {
		if (scannerMove) {
			scannerMove = false;

			pir.moving = true;
			scanner.move();

			vTaskDelay(10 / portTICK_PERIOD_MS);
			pir.moving = false;
		}

		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
}

void IRAM_ATTR scanISR()
{
	portENTER_CRITICAL_ISR(&mux);

	scannerMove = true;

	portEXIT_CRITICAL_ISR(&mux);
}

void IRAM_ATTR pirISR()
{
	portENTER_CRITICAL_ISR(&mux);

	if (pir.detect()) {
		motion = true;
		wifiPrint = true;

		sentry.motionIdx = scanner.idx;
		sentry.waitTime = WAIT_TIME;

		scanner.idx -= scanner.step;
	}

	portEXIT_CRITICAL_ISR(&mux);
}

void init_pir()
{
	pinMode(PIR_PIN, INPUT_PULLDOWN);
	attachInterrupt(digitalPinToInterrupt(PIR_PIN), pirISR, RISING);
}

void init_servo()
{
	sentry.servo.setPeriodHertz(SERVO_FREQ);
	sentry.servo.attach(SENTRY_PIN, SERVO_MIN_PW, SERVO_MAX_PW);

	sentry.servo.write(90);

	scanner.servo.setPeriodHertz(SERVO_FREQ);
	scanner.servo.attach(SCANNER_PIN, SERVO_MIN_PW, SERVO_MAX_PW);

	scanner.servo.write(scannerAngles[scanner.idx]);
}

void print_time(unsigned long millis)
{
	unsigned long div = 1000 * 60 * 60;

	unsigned long hours = millis / div;
	millis %= div;
	div /= 60;

	unsigned long minutes = millis / div;
	millis %= div;
	div /= 60;

	unsigned long seconds = millis / 1000;
	millis %= 1000;

	unsigned long ms = millis / 10;

	Serial.printf("%02lu:%02lu:%02lu.%02lu\n", hours, minutes, seconds, ms);
}
