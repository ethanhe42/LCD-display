#include <msp430g2553.h>
#include "TFT\TFT_Driver.h"

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

	TFT_Init();

	for (;;) {
		Color_Test();//ç®€å�•åˆ·å±�æµ‹è¯•
	}

	return 0;
}
