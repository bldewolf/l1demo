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
#define CYAN (bppcombo(0, 0xFF, 0xFF))
#define RED (bppcombo(0xFF, 0, 0))
void config_clut() {
	_CLUTRWEN = 1;

	G1CLUTWR = 0;
	_CLUTADR = BLACK;
	while(_CLUTBUSY) continue;

	_CLUTADR = 1;
	G1CLUTWR = WHITE;
	while(_CLUTBUSY) continue;

	_CLUTADR = 2;
	G1CLUTWR = CYAN;
	while(_CLUTBUSY) continue;

	_CLUTADR = 3;
	G1CLUTWR = RED;
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

void draw_background() {
	int x = 50, y = 50, w = 50, h = 70;
	rcc_color(0);
	rcc_draw(0, 0, HOR_RES, VER_RES);

}

volatile char synced = 0;
void __attribute__((interrupt, auto_psv))_GFX1Interrupt(void) {
	static int lines = 0;
	if(_VMRGNIF) {
		synced = 1;
		lines = 0;
		_VMRGNIF = 0;
	} else if(_HMRGNIF) {
		lines++;
		_HMRGNIF = 0;
	}
	_GFX1IF = 0;
}

inline int getp(int x, int y) {
	char p = GFXDisplayBuffer[(x + y*HOR_RES)/4];
	return 0x3 & (p >> 2*(x % 4));
}

inline int in_boundary(int x, int y) {
	if(x < 1 || x > HOR_RES - 3 ||
		y < 1 || y > VER_RES - 3) {
		return 0;
	}
	return 1;
}

inline int has_neighbors(int x, int y) {
	int i;
	int nx, ny;
	for(i = 0; i < 4; i++) {
		nx = x;
		ny = y;
		switch(i) {
			case 0:
				nx = x + 2;
				break;
			case 1: ny = y + 2;
				break;
			case 2: nx = x - 2;
				break;
			case 3: ny = y - 2;
				break;
		}
		if(in_boundary(nx, ny) && getp(nx, ny) == 1) {
			return 1;
		}
	}
	return 0;
}

// generated by permy.pl
inline int best_random() {
	switch(rand() % 24) {
		case 0: return 27;
		case 1: return 30;
		case 2: return 39;
		case 3: return 45;
		case 4: return 54;
		case 5: return 57;
		case 6: return 75;
		case 7: return 78;
		case 8: return 99;
		case 9: return 108;
		case 10: return 114;
		case 11: return 120;
		case 12: return 135;
		case 13: return 141;
		case 14: return 147;
		case 15: return 156;
		case 16: return 177;
		case 17: return 180;
		case 18: return 198;
		case 19: return 201;
		case 20: return 210;
		case 21: return 216;
		case 22: return 225;
		case 23: return 228;
		default: return 0;
	}
}

inline void draw_maze() {
	int hx = 1, hy = 1;
	int x = 1;
	int y = 1;
	int nx;
	int ny;
	int wx;
	int wy;
	int i;
	int selected;
	int order;
	int tally;

	rcc_color(1);
	while(1) {
		rcc_draw(x, y, 1, 1);
		selected = 0;
		order = best_random();
		for(i = 0; i < 8; i+=2) {
			wx = nx = x;
			wy = ny = y;
			switch(0x3 & (order >> i)) {
				case 0:
					nx = x + 2;
					wx = x + 1;
					break;
				case 1: ny = y + 2;
					wy = y + 1;
					break;
				case 2: nx = x - 2;
					wx = x - 1;
					break;
				case 3: ny = y - 2;
					wy = y - 1;
					break;
			}
			if(!in_boundary(nx, ny) ||
				getp(nx, ny) != 0) {
				continue;
			}
//			rcc_draw(nx, ny, 1, 1);
			rcc_draw(wx, wy, 1, 1);
			x = nx;
			y = ny;

			selected = 1;
			break;
		}
		if(!selected) {
			tally = 1;
			nx = hx;
			ny = hy;

			while(1) {
				int gp = getp(nx, ny);
				int hn = has_neighbors(nx, ny);
				if(tally && gp) {
					hx = nx;
					hy = ny;
				}
				if(tally && gp == 0) {
					tally = 0;
				}
				if(gp == 0 && hn) {
					break;
				}
				nx += 2;
				ny += 2*(nx / (HOR_RES - 2));
				nx = nx % (HOR_RES - 2);
				if(ny >= VER_RES - 2) {
					return;
				}
			}
			x = nx;
			y = ny;
			order = best_random();
			for(i = 0; i < 8; i+=2) {
				wx = nx = x;
				wy = ny = y;
				switch(0x3 & (order >> i)) {
					case 0:
						nx = x + 2;
						wx = x + 1;
						break;
					case 1: ny = y + 2;
						wy = y + 1;
						break;
					case 2: nx = x - 2;
						wx = x - 1;
						break;
					case 3: ny = y - 2;
						wy = y - 1;
						break;
				}
				if(in_boundary(nx, ny) && getp(nx, ny) == 1) {
					rcc_draw(wx, wy, 1, 1);
					break;
				}
			}
		}
	}
}

inline void lep(int c, int x, int y, int cx, int cy) {
	int i, nx, ny;
	rcc_color(c);
	while(getp(x,y) == 2 && (x != cx || y != cy)) {
//		__delay_ms(100);
		rcc_draw(x, y, 1, 1);
		for(i = 0; i < 4; i++) {
			nx = x;
			ny = y;
			switch(i) {
				case 0:
					nx = x + 1;
					break;
				case 1: ny = y + 1;
					break;
				case 2: nx = x - 1;
					break;
				case 3: ny = y - 1;
					break;
			}
			char p = getp(nx, ny);
			if(p == 2) {
				x = nx;
				y = ny;
				break;
			}
		}
	}
}

inline void le_walk(int x, int y, int lim) {
	int wx, nx, wy, ny, p, order, i, counter = 0;

	rcc_color(2);
	rcc_draw(x, y, 1, 1);
	while(1) {
		if(lim != 0 && counter++ > lim)
			break;
//		__delay_ms(100);
		order = best_random();
		for(i = 0; i < 8; i+=2) {
			wx = nx = x;
			wy = ny = y;
			switch(0x3 & (order >> i)) {
				case 0:
					nx = x + 2;
					wx = x + 1;
					break;
				case 1: ny = y + 2;
					wy = y + 1;
					break;
				case 2: nx = x - 2;
					wx = x - 1;
					break;
				case 3: ny = y - 2;
					wy = y - 1;
					break;
			}
			if(!in_boundary(nx, ny) || getp(wx,wy) == 2) {
				continue;
			}
			break;
		}
		p = getp(nx, ny);
		if(p == 2) {
			lep(0, x, y, nx, ny);
			rcc_color(2);
			x = nx;
			y = ny;
			continue;
		}
		rcc_draw(nx, ny, 1, 1);
		rcc_draw(wx, wy, 1, 1);
		x = nx;
		y = ny;
		if(p == 1) {
			break;
		}
	}
	lep(1, x, y, -1, -1);
}

// Wilson's algorithm
inline void draw_maze2() {
	int x, y;
	rcc_color(1);
//	rcc_draw(3, 1, 1, 1);
//	__delay_ms(1000);

	le_walk(HOR_RES/2 - 1, VER_RES/2 - 1, 1000);
	// iterate over the whole board
	for(y = 1; y < VER_RES - 2; y+=2) {
		for(x = 1; x < HOR_RES - 2; x+=2) {
			int p = getp(x, y);
			if(p != 1) { // let's random walk from here
				le_walk(x, y, 0);
			}
		}
	}
}

inline void walk_maze() {
	int x = 1;
	int y = 1;
	int dir = 0;
	int ndir;
	int nx, ny;
	int i;
	int order;

	while(x != HOR_RES - 3 || y != VER_RES - 3) {
		rcc_color(3);
		rcc_draw(x, y, 1, 1);
		__delay_ms(1);
//		order = best_random();
		for(i = 0; i < 4; i++) {
			nx = x;
			ny = y;
			ndir = (4 + i + dir - 1) % 4;
			switch(ndir) {
//			switch(0x3 & (order >> (2*i))) {
				case 0:
					nx = x + 1;
					break;
				case 1: ny = y + 1;
					break;
				case 2: nx = x - 1;
					break;
				case 3: ny = y - 1;
					break;
			}
			if(in_boundary(nx, ny) &&
				getp(nx, ny) != 0) {
				break;
			}
		}
		rcc_color(2);
		rcc_draw(x, y, 1, 1);
		x = nx;
		y = ny;
		dir = ndir;
	}
	rcc_draw(x, y, 1, 1);
}

inline int walk_maze2(int x, int y, int dir) {
	int ndir;
	int nx, ny;
	int i;
	int order;

	if(x != HOR_RES - 3 || y != VER_RES - 3) {
		rcc_color(3);
		rcc_draw(x, y, 1, 1);
		__delay_ms(1);
		order = best_random();
		for(i = 0; i < 4; i++) {
			nx = x;
			ny = y;
			ndir = (4 + i + dir - 1) % 4;
//			switch(ndir) {
			switch(0x3 & (order >> (2*i))) {
				case 0:
					nx = x + 1;
					break;
				case 1: ny = y + 1;
					break;
				case 2: nx = x - 1;
					break;
				case 3: ny = y - 1;
					break;
			}
			if(in_boundary(nx, ny) &&
			   getp(nx, ny) == 1) {
				rcc_color(2);
				rcc_draw(x, y, 1, 1);
				__delay_ms(1);
				if(walk_maze2(nx, ny, ndir)) {
					return 1;
				}
				rcc_color(3);
				rcc_draw(x, y, 1, 1);
				__delay_ms(1);
			}
		}
	} else {
		rcc_color(2);
		rcc_draw(x, y, 1, 1);
		__delay_ms(1);
		return 1;
	}
	return 0;
}

inline void walk_maze3() {
	int x = 1, y = 1, dir;
	int ndir;
	int nx, ny;
	int i;
	int order;
#define fmax 15
	static short stack[fmax][2];
	int f = 0;

	stack[f][0] = 1;
	stack[f][1] = 1;

	NEXTPLEASE:
	while(x != HOR_RES - 3 || y != VER_RES - 3) {
		if(f >= fmax) return;
		x = stack[f][0];
		y = stack[f][1];
		rcc_color(3);
		rcc_draw(x, y, 1, 1);
		__delay_ms(1);
		order = best_random();
		for(i = 0; i < 4; i++) {
			nx = x;
			ny = y;
			ndir = (4 + i + order - 1) % 4;
			switch(ndir) {
//			switch(0x3 & (order >> (2*i))) {
				case 0:
					nx = x + 1;
					break;
				case 1: ny = y + 1;
					break;
				case 2: nx = x - 1;
					break;
				case 3: ny = y - 1;
					break;
			}
			if(in_boundary(nx, ny) &&
			   getp(nx, ny) == 1) {
				rcc_color(2);
				rcc_draw(x, y, 1, 1);
				__delay_ms(1);
				f++;
				stack[f][0] = nx;
				stack[f][1] = ny;
				goto NEXTPLEASE;
			}
		}
		f--;
	}
}

inline void walk_maze4() {
	int x = 1, y = 1, dir;
	int ndir;
	int nx, ny;
	int i;
	int order;
	int back[2];

	NEXTPLEASE:
	while(x != HOR_RES - 3 || y != VER_RES - 3) {
		rcc_color(3);
		rcc_draw(x, y, 1, 1);
//		__delay_ms(1);
		order = best_random();
		for(i = 0; i < 4; i++) {
			nx = x;
			ny = y;
//			ndir = (4 + i + order - 1) % 4;
//			switch(ndir) {
			switch(0x3 & (order >> (2*i))) {
				case 0:
					nx = x + 1;
					break;
				case 1: ny = y + 1;
					break;
				case 2: nx = x - 1;
					break;
				case 3: ny = y - 1;
					break;
			}
			char p = getp(nx, ny);
			if(p == 2) {
				back[0] = nx;
				back[1] = ny;
			}
			if(in_boundary(nx, ny) && p == 1) {
				rcc_color(2);
				rcc_draw(x, y, 1, 1);
//				__delay_ms(1);
				x = nx;
				y = ny;
				goto NEXTPLEASE;
			}
		}
		x = back[0];
		y = back[1];
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

	config_graphics();
	config_clut();

	/* enable vertical blanking interrupts so _VMRGNIF works */
	_VMRGNIF = 0;
	_VMRGNIE = 1;
	_HMRGNIF = 0;
	_HMRGNIE = 1;
	_GFX1IE = 1;

	draw_background();

	__delay_ms(5000);

	srand(0);

	int16_t frames = 0;

	while (1) {
//			rcc_color(2);
//			rcc_draw(100, 100, 100, 100);
		draw_maze2();
		walk_maze4();
		__delay_ms(5000);
		draw_background();

		synced = 0;
		while(!synced) continue; // wait for vsync
		frames++;
	}

	return 0;
}
