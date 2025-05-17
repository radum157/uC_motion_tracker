#include <scanner.h>
#include <util.h>

using namespace msentry;

ScannerServo::~ScannerServo()
{
	if (servo.attached()) {
		servo.detach();
	}
}

void ScannerServo::move()
{
	if (ignore) {
		ignore = false;
		return;
	}

	idx += step;

	if (idx == scannerNrAngles) {
		step = -1;
		idx = scannerNrAngles - 2;
	} else if (idx == -1) {
		step = 1;
		idx = 1;
	}

	servo.write(scannerAngles[idx]);
}
