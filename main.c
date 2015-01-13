#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include "fonts.h"

#define  FCY    16000000UL    // Instruction cycle frequency, Hz
#include <libpic30.h>

_CONFIG1(FWDTEN_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2)
_CONFIG3(ALTPMP_ALTPMPEN & SOSCSEL_EC)

#define HOR_RES 640UL
#define VER_RES 480UL
#define HOR_FRONT_PORCH 16
#define HOR_PULSE_WIDTH  96
#define HOR_BACK_PORCH 48
#define VER_FRONT_PORCH 11
#define VER_PULSE_WIDTH 2
#define VER_BACK_PORCH 31
#define BPP 2

#define CHR_FGCOLOR		0x5000
#define CHR_BGCOLOR		0x5100
#define CHR_FONTBASE		0x5200
#define CHR_PRINTCHAR		0x5300
#define CHR_TXTAREASTART	0x5800
#define CHR_TXTAREAEND		0x5900
#define CHR_PRINTPOS		0x5A00
#define RCC_SRCADDR	 	0x6200
#define RCC_DESTADDR	 	0x6300
#define RCC_RECTSIZE	 	0x6400
#define RCC_COLOR	 	0x6600
#define RCC_STARTCOPY	 	0x6700

#define GFX_BUFFER_SIZE (HOR_RES * VER_RES / (8/BPP))
__eds__ uint8_t GFXDisplayBuffer[GFX_BUFFER_SIZE] __attribute__((eds, section("DISPLAY"), address(0x1000)));

void config_graphics(void) {
	_G1CLKSEL = 1;
	_GCLKDIV = 11;

	G1DPADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1DPADRH = 0;
	G1W1ADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1W1ADRH = 0;
	G1W2ADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1W2ADRH = 0;

	_GDBEN = 0xFFFF;

	// Using PIC24F manual section 43 page 37-38
	_DPMODE = 1;      /* TFT */
	_GDBEN = 0xFFFF;
	_DPW = _PUW = HOR_RES; // Work area and FB size so GPU works
	_DPH = _PUH = VER_RES;
	_DPWT = HOR_FRONT_PORCH + HOR_PULSE_WIDTH + HOR_BACK_PORCH + HOR_RES;
	_DPHT = VER_FRONT_PORCH + VER_PULSE_WIDTH + VER_BACK_PORCH + VER_RES;
	_DPCLKPOL = 0;
	_DPENOE = 0;
	_DPENPOL = 0;
	_DPVSOE = 1;      /* use VSYNC */
	_DPHSOE = 1;      /* use HSYNC */
	_DPVSPOL = 0;     /* VSYNC negative polarity */
	_DPHSPOL = 0;     /* HSYNC negative polarity */
	_ACTLINE = _VENST = VER_FRONT_PORCH + VER_PULSE_WIDTH + VER_BACK_PORCH;
	_ACTPIX = _HENST = HOR_FRONT_PORCH + HOR_PULSE_WIDTH + HOR_BACK_PORCH;
	_VSST = VER_FRONT_PORCH;
	_HSST = HOR_FRONT_PORCH;
	_VSLEN = VER_PULSE_WIDTH;
	_HSLEN = HOR_PULSE_WIDTH;
	_DPPWROE = 0;
	_DPPINOE = 1;
	_DPPOWER = 1;
	_DPBPP = _PUBPP = BPP / 2;


//	_DPTEST = 2;

	_G1EN = 1;
	__delay_ms(1);
}

void config_chr(void) {
	while(_CMDFUL) continue;
	G1CMDL = 0xFFFF;
	G1CMDH = CHR_FGCOLOR;
	Nop();

	while(_CMDFUL) continue;
	G1CMDL = 2;
	G1CMDH = CHR_BGCOLOR;
	Nop();

	while(_CMDFUL) continue;
	G1CMDL = (uint16_t)(FontStart) & 0xFFFF;
	G1CMDH = CHR_FONTBASE;
	Nop();

	while(_CMDFUL) continue;
	G1CMDL = 0;
	G1CMDH = CHR_TXTAREASTART;
	Nop();

	while(_CMDFUL) continue;
	G1CMDL = (HOR_RES & 0xF)<<12 | VER_RES;
	G1CMDH = CHR_TXTAREAEND | (HOR_RES >>4);
	Nop();
}

void chr_print(char *c) {
	while(_CMDFUL) continue;
	G1CMDL = 0;
	G1CMDH = CHR_PRINTPOS;
	Nop();

	while(*c != NULL) {
		while(_CMDFUL) continue;
		G1CMDL = *c;
		G1CMDH = CHR_PRINTCHAR;
		Nop();

		c++;
	}
}

void rcc_color(char color) {
	while(_CMDFUL) continue;
	G1CMDL = color;
	G1CMDH = RCC_COLOR;
	Nop();
}

void rcc_draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	// destination
	while(_CMDFUL) continue;
	G1CMDL = x + y*HOR_RES;
	G1CMDH = RCC_DESTADDR | (x + y*HOR_RES)>>16;
	Nop();

	// size
	while(_CMDFUL) continue;
	G1CMDL = (w<<12) | h;
	G1CMDH = RCC_RECTSIZE | (w>>4);
	Nop();

	// go!
	while(_CMDFUL) continue;
	G1CMDL = 0xC<<3;
	G1CMDH = RCC_STARTCOPY;
	Nop();
}

void blank_background() {
	rcc_color(0);
	rcc_draw(0, 0, HOR_RES, VER_RES);
}

/* For close to A440: 16Mhz / (440Hz*256 samples) =~ 0x8E */
void config_timer() {
	PR1 = 0x8E;
	_T1IP = 5;	// set interrupt priority
	_TON  = 1;	// turn on the timer
	_T1IF = 0;	// reset interrupt flag
	_T1IE = 1;	// turn on the timer1 interrupt
}
const unsigned char sinetable[] = {
0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,
0x98,0x9b,0x9e,0xa2,0xa5,0xa7,0xaa,0xad,
0xb0,0xb3,0xb6,0xb9,0xbc,0xbe,0xc1,0xc4,
0xc6,0xc9,0xcb,0xce,0xd0,0xd3,0xd5,0xd7,
0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,
0xea,0xeb,0xed,0xee,0xf0,0xf1,0xf3,0xf4,
0xf5,0xf6,0xf8,0xf9,0xfa,0xfa,0xfb,0xfc,
0xfd,0xfd,0xfe,0xfe,0xfe,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,0xfd,
0xfd,0xfc,0xfb,0xfa,0xfa,0xf9,0xf8,0xf6,
0xf5,0xf4,0xf3,0xf1,0xf0,0xee,0xed,0xeb,
0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc,
0xda,0xd7,0xd5,0xd3,0xd0,0xce,0xcb,0xc9,
0xc6,0xc4,0xc1,0xbe,0xbc,0xb9,0xb6,0xb3,
0xb0,0xad,0xaa,0xa7,0xa5,0xa2,0x9e,0x9b,
0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,
0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,
0x67,0x64,0x61,0x5d,0x5a,0x58,0x55,0x52,
0x4f,0x4c,0x49,0x46,0x43,0x41,0x3e,0x3b,
0x39,0x36,0x34,0x31,0x2f,0x2c,0x2a,0x28,
0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17,
0x15,0x14,0x12,0x11,0xf,0xe,0xc,0xb,
0xa,0x9,0x7,0x6,0x5,0x5,0x4,0x3,
0x2,0x2,0x1,0x1,0x1,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x1,0x1,0x1,0x2,
0x2,0x3,0x4,0x5,0x5,0x6,0x7,0x9,
0xa,0xb,0xc,0xe,0xf,0x11,0x12,0x14,
0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,
0x25,0x28,0x2a,0x2c,0x2f,0x31,0x34,0x36,
0x39,0x3b,0x3e,0x41,0x43,0x46,0x49,0x4c,
0x4f,0x52,0x55,0x58,0x5a,0x5d,0x61,0x64,
0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c,
};
//_T1Interrupt() is the T1 interrupt service routine (ISR).
void __attribute__((__interrupt__)) _T1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
	static unsigned char idx = 0;
	PORTB = sinetable[idx] << 8;
	idx += 1;
	_T1IF = 0;
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

	config_graphics();
	config_chr();
	config_timer();

	blank_background();
	chr_print("Hello");

	uint8_t c = 0;
	while (1) {
		rcc_color(c++ % 4);
		rcc_draw(100, 0, 200, 200);
//		rcc_draw(0, 0, HOR_RES, VER_RES);
		__delay_ms(1000);
	}

	return 0;
}
