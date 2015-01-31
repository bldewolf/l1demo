#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <math.h>

#define  FCY    16000000UL    // Instruction cycle frequency, Hz
#include <libpic30.h>

_CONFIG1(FWDTEN_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2)
_CONFIG3(ALTPMP_ALTPMPEN & SOSCSEL_EC)

__eds__ unsigned char sbuf[2][8000] __attribute__((eds, section("SBUF")));
volatile int sbuf_swap = 1; /* use this to enable/disable swapping */

void config_timer() {
	PR1 = 2000;	// we are gunning for 8khz
	_T1IP = 5;	// set interrupt priority
	_TON  = 1;	// turn on the timer
	_T1IF = 0;	// reset interrupt flag
	_T1IE = 1;	// turn on the timer1 interrupt
}
//_T1Interrupt() is the T1 interrupt service routine (ISR).
void __attribute__((__interrupt__)) _T1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
	static int active = 0;
	static unsigned short idx = 0;
	PORTB = sbuf[active][idx] << 8;
	idx += 1;
	if(idx == sizeof(sbuf[0])) {
		if(sbuf_swap)
			active = !active;
		idx = 0;
	}
	_T1IF = 0;
}

void sine_wave(__eds__ unsigned char *buf, int len, double freq) {
	int x;
	double scaler = 1.0 * len / freq / (2*3.14159);

	for(x = 0; x < len; x++) {
		buf[x] = (sin(x / scaler) + 1) * 128;
	}
}

int main(void) {
	//OSCCON = 0x0000;

	ANSB = 0x0000;
	ANSC = 0x0000;
	ANSD = 0x0000;
	ANSF = 0x0000;
	ANSG = 0x0000;
	//TRISE = 0x0000;

	TRISB = 0x0000;

	sine_wave(sbuf[0], sizeof(sbuf[0]), 440);
	sine_wave(sbuf[1], sizeof(sbuf[0]), 220);
	config_timer();

	while (1) {
		__delay_ms(1000);
	}

	return 0;
}
