#include <tonePWM.h>

#include <Arduino.h>

void msentry::tonePWM(uint8_t pin, unsigned int frequency, unsigned long duration)
{
	ledcSetup(TONE_CHANNEL, frequency, TONE_RESOLUTION);

	ledcAttachPin(pin, TONE_CHANNEL);
	ledcWrite(TONE_CHANNEL, 128);  // 128 out of 255 (50% duty cycle for 8-bit resolution)

	if (duration > 0) {
		vTaskDelay(duration / portTICK_PERIOD_MS);
		noTone(pin);
	}
}

void msentry::noTonePWM(uint8_t pin)
{
	ledcWrite(TONE_CHANNEL, 0); // 0 duty cycle
	ledcDetachPin(pin);
}
