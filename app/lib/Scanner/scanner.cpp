#include <scanner.h>

using namespace msentry;

const int ScannerServo::angles[ScannerServo::nrAngles] = { 0, 45, 90, 135, 180 };

ScannerServo::~ScannerServo()
{
	if (pin > 0) {
		servo.detach();
	}
}

void ScannerServo::move()
{
	idx += step;

	if (idx == nrAngles) {
		step = -1;
		idx = nrAngles - 1;
	} else if (idx == -1) {
		step = 1;
		idx = 0;
	}

	servo.write(ScannerServo::angles[idx]);
}
