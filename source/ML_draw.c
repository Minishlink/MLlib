#include <MLlib.h>

/**
* \file
* \brief This file contains draw functions.
*/

GXColor RGBA_u32_u8(u32 color) 
{
	u8 r, g, b, a;

	r = (color >> 24) & 0xFF; 
	g = (color >> 16) & 0xFF; 
	b = (color >> 8) & 0xFF; 
	a = (color) & 0xFF; 

	return (GXColor){r, g, b, a};
}

u16 RGB_888_565(u8 r, u8 g, u8 b) 
{
	return ((r >> 3) | ((g >> 2)<<5) | ((b >> 3)<<11)); 
}

u16 BGR_RGB(u32 bgr)
{
	return (((bgr & 31)<<11) | (((bgr>>5) & 63)<<5) | ((bgr>>11) & 31));
}


u32 RGB_888_565_2(u32 colorin)
{
	u8 r,g,b;
	
	r = (((colorin >> 16) & 0xFF)*31)/255;
	g = (((colorin >> 8) & 0xFF)*63)/255;
	b = (((colorin) & 0xFF)*31)/255;

	return ((r) << 11 | (g) << 5 | (b));	
}

//---------------------------------------------

u32 RGB2GC(u32 rgb)
{
	u8 r1 = (rgb&0xff000000) >> 24;
	u8 g1 = (rgb&0xff0000) >> 16;
	u8 b1 = (rgb&0xff00) >> 8;     
	u8 r2 = r1;
	u8 g2 = g1;
	u8 b2 = b1;
	int y1, cb1, cr1, y2, cb2, cr2, cb, cr;

	y1 = (299 * r1 + 587 * g1 + 114 * b1) / 1000;
	cb1 = (-16874 * r1 - 33126 * g1 + 50000 * b1 + 12800000) / 100000;
	cr1 = (50000 * r1 - 41869 * g1 - 8131 * b1 + 12800000) / 100000;

	y2 = (299 * r2 + 587 * g2 + 114 * b2) / 1000;
	cb2 = (-16874 * r2 - 33126 * g2 + 50000 * b2 + 12800000) / 100000;
	cr2 = (50000 * r2 - 41869 * g2 - 8131 * b2 + 12800000) / 100000;

	cb = (cb1 + cb2) >> 1;
	cr = (cr1 + cr2) >> 1;

	return (y1 << 24) | (cb << 16) | (y2 << 8) | cr;
}

u32 RGB2GC_2(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2)
{
	int y1, cb1, cr1, y2, cb2, cr2, cb, cr;

	y1 = (299 * r1 + 587 * g1 + 114 * b1) / 1000;
	cb1 = (-16874 * r1 - 33126 * g1 + 50000 * b1 + 12800000) / 100000;
	cr1 = (50000 * r1 - 41869 * g1 - 8131 * b1 + 12800000) / 100000;

	y2 = (299 * r2 + 587 * g2 + 114 * b2) / 1000;
	cb2 = (-16874 * r2 - 33126 * g2 + 50000 * b2 + 12800000) / 100000;
	cr2 = (50000 * r2 - 41869 * g2 - 8131 * b2 + 12800000) / 100000;

	cb = (cb1 + cb2) >> 1;
	cr = (cr1 + cr2) >> 1;

	return (y1 << 24) | (cb << 16) | (y2 << 8) | cr;
}

u32 FLIP32(u32 value)
{
	u32 b;

	b = (value & 0xff) << 24;
	b |= (value & 0xff00) << 8;
	b |= (value & 0xff0000) >> 8;
	b |= (value & 0xff000000) >> 24;

	return b;
}

u16 FLIP16(u16 value)
{
	return ((value & 0xff) << 8) | ((value & 0xff00) >> 8);
}

// that's from PNGU lib
void _PNGU_YCbYCr_TO_RGB8(unsigned int ycbycr, unsigned char *r1, unsigned char *g1, unsigned char *b1, unsigned char *r2, unsigned char *g2, unsigned char *b2)
{
	unsigned char *val = (unsigned char *) &ycbycr;
	int r, g, b;

	r = 1.371f * (val[3] - 128);
	g = - 0.698f * (val[3] - 128) - 0.336f * (val[1] - 128);
	b = 1.732f * (val[1] - 128);

	*r1 = _pngu_clamp (val[0] + r, 0, 255);
	*g1 = _pngu_clamp (val[0] + g, 0, 255);
	*b1 = _pngu_clamp (val[0] + b, 0, 255);

	*r2 = _pngu_clamp (val[2] + r, 0, 255);
	*g2 = _pngu_clamp (val[2] + g, 0, 255);
	*b2 = _pngu_clamp (val[2] + b, 0, 255);
}

// that's from PNGU lib
int _pngu_clamp (int value, int min, int max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;

	return value;
}

