#include <Arduino.h>
#include <esp_bt.h>

#include <util.h>
#include <pir.h>
#include <sentry.h>
#include <scanner.h>
#include <tonePWM.h>
#include <wifi.h>

using namespace msentry;

// Peripherals
PIRSensor pir;
SentryServo sentry;
ScannerServo scanner;

// Interrupt variables
volatile bool motion{ false };
volatile bool scannerMove{ false };
volatile bool wifiPrint{ false };

// Timers + sync
hw_timer_t *scanTimer{ nullptr };
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

SemaphoreHandle_t serialMux;

// Helper functions
void printTime(unsigned long millis);

void initPIR();
void initServo();

void IRAM_ATTR scanISR();
void IRAM_ATTR pirISR();

void runScanner(void *param);

/**
 * Disables as much as possible to save power
 */
void powerSetup()
{
	Serial.println("Power setup...");

	btStop();
	esp_bt_controller_disable();
	esp_bt_controller_deinit();

	Serial.println("Power setup ended");
}

/**
 * Sets up serial, peripherals, interrupts, timers, wifi etc.
 */
void setup()
{
	Serial.begin(BAUD_RATE);
	Serial.println("Beginning Setup");

	// Pir first to calibrate
	initPIR();

	// Power and wifi
	powerSetup();
	wifiSetup();

	// Peripherals
	initServo();

	pinMode(BUZZER_PIN, OUTPUT);
	noTonePWM(BUZZER_PIN);

	serialMux = xSemaphoreCreateMutex();

	// Calibration period
	delay(CALIBRATION_TIME);

	// Start interrupts and vtasks
	scanTimer = timerBegin(0, 80, true); // timer 0, prescaler 80 (1 tick per 80us), count up
	timerAttachInterrupt(scanTimer, &scanISR, true);

	timerAlarmWrite(scanTimer, SCAN_TIME, true);

	// Create tasks
	xTaskCreatePinnedToCore(runScanner, "Scanner", TASK_STACK_SIZE, NULL, 1, NULL, 1);
	xTaskCreatePinnedToCore(runWifi, "WiFi", TASK_STACK_SIZE, NULL, 0, NULL, 1);

	// End of setup
	Serial.println("Ending Setup");
	tonePWM(BUZZER_PIN, BUZZER_FREQ, ALERT_TIME);

	timerAlarmEnable(scanTimer);
}

/**
 * Moves sentry and handles motion detection
 */
void loop()
{
	if (motion) {
		motion = false;

		xSemaphoreTake(serialMux, portMAX_DELAY);

		Serial.print("Motion detected at: ");
		printTime(pir.lastDebounce);

		xSemaphoreGive(serialMux);

		tonePWM(BUZZER_PIN, BUZZER_FREQ, ALERT_TIME);

		vTaskDelay(MS_TO_PORTTICKS(sentry.waitTime));
		sentry.waitTime = 0;
	}

	sentry.smoothStep();
}


/**
 * Moves the scanner between angles in @a scannerAngles
 */
void runScanner(void *param)
{
	while (true) {
		if (scannerMove) {
			scannerMove = false;

			pir.moving = true;
			scanner.move();

			vTaskDelay(MS_TO_PORTTICKS(10));
			pir.moving = false;
		}

		vTaskDelay(MS_TO_PORTTICKS(50));
	}
}

// Interrupts for @a scanTimer and PIR pin
void IRAM_ATTR scanISR()
{
	portENTER_CRITICAL_ISR(&mux);

	scannerMove = true;

	portEXIT_CRITICAL_ISR(&mux);
}

void IRAM_ATTR pirISR()
{
	portENTER_CRITICAL_ISR(&mux);

	// When debounce checks out, move sentry, update wifi message and stop the mechanical parts for 1s
	if (pir.detect()) {
		motion = true;
		wifiPrint = true;

		sentry.motionIdx = scanner.idx;
		sentry.waitTime = WAIT_TIME;

		scanner.idx -= scanner.step;
	}

	portEXIT_CRITICAL_ISR(&mux);
}

// Inits for PIR and servos
void initPIR()
{
	pinMode(PIR_PIN, INPUT_PULLDOWN);
	attachInterrupt(digitalPinToInterrupt(PIR_PIN), pirISR, RISING);
}

void initServo()
{
	sentry.servo.setPeriodHertz(SERVO_FREQ);
	sentry.servo.attach(SENTRY_PIN, SERVO_MIN_PW, SERVO_MAX_PW);

	sentry.servo.write(90);

	scanner.servo.setPeriodHertz(SERVO_FREQ);
	scanner.servo.attach(SCANNER_PIN, SERVO_MIN_PW, SERVO_MAX_PW);

	scanner.servo.write(scannerAngles[scanner.idx]);
}

/**
 * Prints elapsed time since start of program to serial
 */
void printTime(unsigned long millis)
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
