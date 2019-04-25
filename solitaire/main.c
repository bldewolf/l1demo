#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <math.h>

#define  FCY    16000000UL    // Instruction cycle frequency, Hz
#include <libpic30.h>

#include "texts.h"

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

#define HOR_RES 640UL
#define VER_RES 480UL
#define HOR_FRONT_PORCH 16
#define HOR_PULSE_WIDTH 96
#define HOR_BACK_PORCH  48
#define VER_FRONT_PORCH 10
#define VER_PULSE_WIDTH 2
#define VER_BACK_PORCH  33
#define BPP 2
#define CLOCKDIV 11
#define VENST_FUDGE 0
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
__eds__ uint8_t GFXDisplayBuffer[GFX_BUFFER_SIZE] __attribute__((eds, section("DISPLAY")));

void config_graphics(void) {
	_G1CLKSEL = 1;
	_GCLKDIV = CLOCKDIV;

	G1DPADRL = (unsigned long)(GFXDisplayBuffer);
	G1DPADRH = (unsigned long)(GFXDisplayBuffer);
//	G1W1ADRL = (unsigned long)(GFXDisplayBuffer);
//	G1W1ADRH = (unsigned long)(GFXDisplayBuffer);
	G1W2ADRL = (unsigned long)(GFXDisplayBuffer);
	G1W2ADRH = (unsigned long)(GFXDisplayBuffer);

	_GDBEN = 0xFFFF;

	// Using PIC24F manual section 43 page 37-38
	_DPMODE = 1;      /* TFT */
	_GDBEN = 0xFFFF;
	_DPW = _PUW = HOR_RES; // Work area and FB size so GPU works
	_DPH = _PUH = VER_RES;
	_DPWT = HOR_FRONT_PORCH + HOR_PULSE_WIDTH + HOR_BACK_PORCH + HOR_RES;
	_DPHT = VER_FRONT_PORCH + VER_PULSE_WIDTH + VER_BACK_PORCH + VER_RES + 45;
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
	_DPBPP = _PUBPP = 1;


//	_DPTEST = 2; /* Uncomment for test patterns */

	_G1EN = 1;
	__delay_ms(1);
}

void rcc_color(unsigned int color) {
	while(_CMDFUL) continue;
	G1CMDL = color;
	G1CMDH = RCC_COLOR;
	Nop();
}

#define bppcombo(r, g, b) ((r & 0x1F)<<11 | (g & 0x3F)<<5 | (b & 0x1F)<<0)
#define BLACK (bppcombo(0, 0, 0))
#define WHITE (bppcombo(0xFF, 0xFF, 0xFF))
#define GREEN (bppcombo(0, 0x20, 0))
#define GRAY (bppcombo(0x1F, 0x37, 0x1F))
#define BLUE (bppcombo(0, 0, 0x18))
void config_clut() {
	_CLUTRWEN = 1;

	G1CLUTWR = 0;
	_CLUTADR = BLACK;
	while(_CLUTBUSY) continue;

	_CLUTADR = 1;
	G1CLUTWR = WHITE;
	while(_CLUTBUSY) continue;

	_CLUTADR = 2;
	G1CLUTWR = GREEN;
	while(_CLUTBUSY) continue;

	_CLUTADR = 3;
	G1CLUTWR = GRAY;
	while(_CLUTBUSY) continue;

	_CLUTRWEN = 0;

	_CLUTEN = 1; // let 'er rip!
}

void clut_set(int index, int color) {
	_CLUTADR = index;
	_CLUTRWEN = 1;
	G1CLUTWR = color;
	while(_CLUTBUSY) continue;
	_CLUTRWEN = 0;
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

void rcc_copy(int oper_rop, int dtst, __eds__ uint8_t *src, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t w, uint16_t h) {
	// source
	while(_CMDFUL) continue;
	G1CMDL = x1 + y1*HOR_RES;
	G1CMDH = RCC_SRCADDR | (x1 + y1*HOR_RES)>>16;
	Nop();

	// destination
	while(_CMDFUL) continue;
	G1CMDL = x2 + y2*HOR_RES;
	G1CMDH = RCC_DESTADDR | (x2 + y2*HOR_RES)>>16;
	Nop();

	// size
	while(_CMDFUL) continue;
	G1CMDL = (w<<12) | h;
	G1CMDH = RCC_RECTSIZE | (w>>4);
	Nop();

	// go!
	while(_CMDFUL) continue;
	G1CMDL = (oper_rop & 0xFF)<<3 | (dtst & 0x3) << 1;
	G1CMDH = RCC_STARTCOPY;
	Nop();
}

void clear_field() {
// green field
	rcc_color(2);
	rcc_draw(0, 38, HOR_RES - 1, VER_RES - (38 + 17));
}

void draw_background() {
	int i;
	rcc_color(0);
	rcc_draw(0, 0, HOR_RES, VER_RES);

// win 3.1 fun
	// blue bar
	rcc_color(2);
	rcc_draw(19, 0, HOR_RES - 1 - 19 - 38, 18);

	// left corner
	rcc_color(3);
	rcc_draw(0, 0, 18, 18);
	rcc_color(0);
	rcc_draw(2, 7, 13, 3);
	rcc_color(1);
	rcc_draw(3, 8, 11, 1);

	// right corner
	rcc_color(3);
	rcc_draw(HOR_RES - 1 - 37, 0, 18, 18);
	rcc_draw(HOR_RES - 1 - 18, 0, 18, 18);
	rcc_color(0);
	for(i = 0; i < 4; i++ ) {
		// first triangle
		rcc_draw(HOR_RES - 1 - 37 + 6 + i, 8 + i, 7 - 2*i, 1);
		// top triangle
		rcc_draw(HOR_RES - 1 - 18 + 6 + i, 8 - i, 7 - 2*i, 1);
		// bottom triangle
		rcc_draw(HOR_RES - 1 - 18 + 6 + i, 11 + i, 7 - 2*i, 1);
	}

	// options bar
	rcc_color(3);
	rcc_draw(0, 19, HOR_RES - 1, 18);
	rcc_color(1);
	rcc_draw(0, VER_RES - 16, HOR_RES - 1, 16);

	while(!_CMDMPT) continue;
	G1W1ADRL = (unsigned long)(&menu_text);
	G1W1ADRH = (unsigned long)(&menu_text);
	rcc_color(1);
	rcc_copy(0x6C, 0x1, menu_text, 0, 0, 10, 23, 83, 13);
	while(!_CMDMPT) continue;
	G1W1ADRL = (unsigned long)(title_text);
	G1W1ADRH = (unsigned long)(title_text);
	rcc_copy(0x1C, 0x1, title_text, 0, 0, 285, 5, 52, 10);
	while(!_CMDMPT) continue;
	G1W1ADRL = (unsigned long)(score_time_text);
	G1W1ADRH = (unsigned long)(score_time_text);
	rcc_copy(0x1C, 0x1, score_time_text, 0, 0, HOR_RES - 1 - 108, VER_RES - 13, 103, 10);
}

volatile char synced = 0;
void __attribute__((interrupt, auto_psv))_GFX1Interrupt(void) {
	static int lines = 0;
	if(_VMRGNIF) {
		synced = 1;
		lines = 0;
		_VMRGNIF = 0;
	} else if(_HMRGNIF) { /* on each horizontal sync, change clut */
		if(lines == 0) {
			clut_set(2, BLUE);
		} else if(lines == 120) {
			clut_set(2, GREEN);
		}
		lines++;
		_HMRGNIF = 0;
	}
	_GFX1IF = 0;
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
	config_clut();

	/* enable vertical blanking interrupts so _VMRGNIF works */
	_VMRGNIF = 0;
	_VMRGNIE = 1;
	_HMRGNIF = 0;
	_HMRGNIE = 1;
	_GFX1IE = 1;

	draw_background();
	clear_field();

	__delay_ms(5000);

	srand(1);

	int16_t frames = 0;
	int16_t x = 120, y = 120;
	int16_t xv = 10, yv = 10;
	int16_t xa = 0, ya = 4;
	int16_t w = 50, h = 70;
	int cards = 0;
	int blx = 2;
	int bhx = HOR_RES - 1 - 2 - w;
	int bly = 40;
	int bhy = VER_RES - 2 - 19 - 1 - h;
	while (1) {
		if(cards == 52) {
			cards = 0;
			clear_field();
		}
		if(y > (bhy - h) && abs(yv) < 10) { // reset it
			do {
				x = abs((rand()<<8 | rand()) % bhx);
			} while(x < blx || x > bhx);
			do {
				y =(rand() % 100) + bly + 1;
			} while(y < bly || y > bhy);
			do {
				xv = 3 * ((rand() % 8) - 4);
			} while(xv == 0);
			do {
				yv = 3 * ((rand() % 8) - 2);
			} while(yv == 0);
			cards++;
		}
		frames++;
		if(frames % 1 == 0) {
			x += xv;
			y += yv;
			xv += xa;
			yv += ya;
			if(x < blx) {
				x = 2*blx - x;
				xv = -xv;
			} else
			if(x > bhx) {
				x = 2*bhx - x;
				xv = -xv;
			}

			if(y < bly) {
				y = 2*bly - y;
				yv = abs(yv);
			} else
			if(y > bhy) {
				y = 2*bhy - y;
				yv = -yv/2;
			}

			// draw card with logo on it
			rcc_color(0);
//			rcc_draw(x, y, w, h);
			rcc_draw(x, y+2, w, h-4);
			rcc_draw(x+1, y+1, w-2, h-2);
			rcc_draw(x+2, y, w-4, h);
			rcc_color(1);
			rcc_draw(x+2, y+1, w-4, h-2);
			rcc_draw(x+1, y+2, w-2, h-4);
			// left L
			rcc_color(0);
			rcc_draw(x+6, y+7, 15, 42);
			rcc_draw(x+21, y+35, 8, 14);
			// right L outline
			rcc_draw(x+21, y+22, 23, 1);
			rcc_draw(x+43, y+22, 1, 42);
			rcc_draw(x+28, y+49, 1, 14);
			rcc_draw(x+28, y+63, 15, 1);
//			rcc_color(3);
//			rcc_draw(x+23, y+18, 11, 30);
//			rcc_draw(x+17, y+18, 6, 10);
		}

//		_VMRGNIF = 0;
//		while(!_VMRGNIF) continue; // wait for vsync
		synced = 0;
		while(!synced) continue; // wait for vsync
	}

	return 0;
}
