#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include "fonts.h"
#include "music.h"
#include "testgfx.h"

#define  FCY    16000000UL    // Instruction cycle frequency, Hz
#include <libpic30.h>

_CONFIG1(FWDTEN_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2)
_CONFIG3(ALTPMP_ALTPMPEN & SOSCSEL_EC)

/*
   To change resolutions, use an X modeline generator like:
   http://xtiming.sourceforge.net/cgi-bin/xtiming.pl
   For example, 80x480@60Hz produces
   Modeline "80x480@61" 4.93 80 112 128 160 480 490 495 505
   Then we convert it to the values we need:

   4.93 is the pixel clock in MHz, we need to set CLOCKDIV to match.  To pick a
   divisor, use page 147 of the data sheet or, uh, this perl one-liner to
   generate all of them and pick one that's close:
   perl -e '$d = 1;  $x = 0; while($x < 128) { printf "%s %.2f\n",$x,96/$d;if($x < 64) { $d += 0.25; } elsif($x < 96) { $d += 0.50 } else { $d += 1 }; $x++}'

   The differences between: 80 112 128 160 480
   are H-porch, H-Pulse, H-width
   The differences between: 480 490 495 505
   are V-porch, V-Pulse, V-width

   Once these values are in, the monitor will display it but it will probably
   be misaligned (especially vertically).  Play with VENST_FUDGE and
   HENST_FUDGE to fix this.
*/

#define HOR_RES 80UL
#define VER_RES 480UL
#define HOR_FRONT_PORCH 32
#define HOR_PULSE_WIDTH 16
#define HOR_BACK_PORCH  32
#define VER_FRONT_PORCH 10
#define VER_PULSE_WIDTH 5
#define VER_BACK_PORCH  10
#define BPP 2
#define CLOCKDIV 69
#define VENST_FUDGE 1
#define HENST_FUDGE 0
#define VSPOL 0 /* sync polarities */
#define HSPOL 0

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
#define IPU_SRCADDR		0x7100
#define IPU_DESTADDR	 	0x7200
#define IPU_DECOMPRESS	 	0x7400

#define GFX_BUFFER_SIZE (HOR_RES * VER_RES / (8/BPP))
__eds__ uint8_t GFXDisplayBuffer[GFX_BUFFER_SIZE] __attribute__((eds, section("DISPLAY"), address(0x1000)));

void config_graphics(void) {
	_G1CLKSEL = 1;
	_GCLKDIV = CLOCKDIV;

	G1DPADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1DPADRH = (unsigned long)(GFXDisplayBuffer) >>16 & 0xFF;
	G1W1ADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1W1ADRH = (unsigned long)(GFXDisplayBuffer) >>16 & 0xFF;
	G1W2ADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1W2ADRH = (unsigned long)(GFXDisplayBuffer) >>16 & 0xFF;

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
	_DPVSOE = 1;
	_DPHSOE = 1;
	_DPVSPOL = VSPOL;
	_DPHSPOL = HSPOL;
	_ACTLINE = _VENST = VER_FRONT_PORCH + VER_PULSE_WIDTH + VER_BACK_PORCH - VENST_FUDGE;
	_ACTPIX = _HENST = HOR_FRONT_PORCH + HOR_PULSE_WIDTH + HOR_BACK_PORCH - HENST_FUDGE;
	_VSST = VER_FRONT_PORCH;
	_HSST = HOR_FRONT_PORCH;
	_VSLEN = VER_PULSE_WIDTH;
	_HSLEN = HOR_PULSE_WIDTH;
	_DPPWROE = 0;
	_DPPINOE = 1;
	_DPPOWER = 1;
	_DPBPP = _PUBPP = BPP / 2;


//	_DPTEST = 2; /* Uncomment for test patterns */

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
	G1CMDL = (unsigned long)(FontStart) & 0xFFFF;
	G1CMDH = CHR_FONTBASE | (unsigned long)(FontStart) >> 16;
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
	G1W1ADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1W1ADRH = (unsigned long)(GFXDisplayBuffer) >>16 & 0xFF;

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
	G1W1ADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1W1ADRH = (unsigned long)(GFXDisplayBuffer) >>16 & 0xFF;
	G1W2ADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1W2ADRH = (unsigned long)(GFXDisplayBuffer) >>16 & 0xFF;

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

void ipu_decomp(__eds__ uint8_t *src, __eds__ uint8_t *dst, unsigned long size) {
	G1W1ADRL = (unsigned long)(src) & 0xFFFF;
	G1W1ADRH = (unsigned long)(src) >>16 & 0xFF;
	G1W2ADRL = (unsigned long)(dst) & 0xFFFF;
	G1W2ADRH = (unsigned long)(dst) >>16 & 0xFF;

	// source addr
	while(_CMDFUL) continue;
	G1CMDL = 0;
	G1CMDH = IPU_SRCADDR;
	Nop();

	// dest addr
	while(_CMDFUL) continue;
	G1CMDL = 0;
	G1CMDH = IPU_DESTADDR;
	Nop();

	// dest addr
	while(_CMDFUL) continue;
	G1CMDL = size & 0xFFFF;
	G1CMDH = IPU_DECOMPRESS	| (size >> 16);
	Nop();
}

/* In-A-Gadda-Da-Vida Tab
D[--------3-2-------\7-6-5------[
A[---5-5-------3-5----------3---[
*/

unsigned short song[] = {
	NOTES_D5,
	0,
	NOTES_D5,
	0,
	NOTES_F5,
	NOTES_E5,
	NOTES_C5,
	NOTES_D5,
	0,
	NOTES_A5,
	0,
	NOTES_Ab5,
	0,
	NOTES_G5,
	0,
	NOTES_C5,
	};

void config_timer() {
	PR1 = 0;
	_T1IP = 5;	// set interrupt priority
	_TON  = 1;	// turn on the timer
	_T1IF = 0;	// reset interrupt flag
	_T1IE = 1;	// turn on the timer1 interrupt

	/* set up timer for stepping through song */
	PR2 = 0x3d09; // reaaal sloooow
	_T2IP = 6;
	_T2IF = 0;
	/* no nice macros for T2CON :( */
	T2CON = 0b1000000000110000; // set T2 on with max prescaler (256)
	_T2IE = 1;
}
//_T1Interrupt() is the T1 interrupt service routine (ISR).
void __attribute__((__interrupt__)) _T1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
	static unsigned char idx = 0;
	PORTB = sinetable[idx] << 8;
	idx += 1;
	_T1IF = 0;
}

//_T2Interrupt() is the T2 interrupt service routine (ISR).
void __attribute__((__interrupt__)) _T2Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void)
{
	static unsigned short idx = 0;
	PR1 = song[idx];

	idx++;
	if(idx == sizeof(song) / sizeof(song[0])) /* loop it! */
		idx = 0;
	_T2IF = 0;
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
	__delay_ms(1);
	ipu_decomp(gfx_compressed, GFXDisplayBuffer, GFX_BUFFER_SIZE);
	chr_print("Hello\nHello\nHello\nHello\n");
	char buf[20];
	sprintf(buf, "%x\n", G1IPU);
	chr_print(buf);
	rcc_draw(79, 0, 1, 480); /* Weird things occur if the right column isn't 0 */

	uint8_t c = 0;
	while (1) {
		rcc_color(c++ % 4);
		rcc_draw(20, 0, 20, 200);
//		rcc_draw(0, 0, HOR_RES, VER_RES);
//		rcc_draw(638, 0, 1, 200);
		__delay_ms(1000);
	}

	return 0;
}
