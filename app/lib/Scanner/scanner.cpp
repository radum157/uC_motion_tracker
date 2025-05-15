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
	idx += step;

	if (idx == scannerNrAngles) {
		step = -1;
		idx = scannerNrAngles - 1;
	} else if (idx == -1) {
		step = 1;
		idx = 0;
	}

	servo.write(scannerAngles[idx]);
}
