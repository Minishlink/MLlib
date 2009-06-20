#include <MLlib.h>

/**
* \file
* \brief This file contains images functions.
*/

void _initImage(ML_Image *image)
{	
	image->width = 0;
	image->height = 0;
	image->data = NULL;
}

void ML_CloneImage(ML_Image *image1, ML_Image *image2)
{
	*image2 = *image1;
}

void ML_DeleteImage(ML_Image *image)
{
	if(image->data)
	{
		free(image->data);
		image->data = NULL;
	}
}

void ML_FlushImage(ML_Image *image)
{
	DCFlushRange(image->data, image->width * image->height * 4);
	GX_InitTexObj(&image->texObj, image->data, image->width, image->height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);	
	GX_InitTexObjLOD(&image->texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
}

// These two next functions are mainly inspired from GRRLIB, thank you for these equation/algorithm !
void ML_SetPixelColor(ML_Image *image, int x, int y, u32 color) 
{
	u8 *truc = (u8*)image->data;
	u32 offset;

	offset = (((y >> 2)<<4)*image->width) + ((x >> 2)<<6) + (((y%4 << 2) + x%4 ) <<1); // Fuckin equation found by NoNameNo ;)

	*(truc+offset)=(color) & 0xFF;
	*(truc+offset+1)=(color>>24) & 0xFF;
	*(truc+offset+32)=(color>>16) & 0xFF;
	*(truc+offset+33)=(color>>8) & 0xFF;
	
	image->data = truc;
}

u32 ML_GetPixelColor(ML_Image *image, int x, int y) 
{
	u8 *truc = (u8*)image->data;
	u8 r, g, b, a;
	u32 offset;

	offset = (((y >> 2)<<4)*image->width) + ((x >> 2)<<6) + (((y%4 << 2) + x%4 ) << 1); // Fuckin equation found by NoNameNo ;)

	a=*(truc+offset);
	r=*(truc+offset+1);
	g=*(truc+offset+32);
	b=*(truc+offset+33);

	return((r<<24) | (g<<16) | (b<<8) | a);
}

void ML_FlipImageX(ML_Image *image)
{
	unsigned int x, y, texWidth = image->width - 1;

	for (y = 0; y < image->height; y++) {
		for (x = 0; x < image->width; x++) {
			ML_SetPixelColor(image, texWidth - x, y, ML_GetPixelColor(image, x, y));
		}
	}
}

void ML_FlipImageY(ML_Image *image)
{
	unsigned int x, y, texHeight = image->height - 1;

	for (y = 0; y < image->height; y++) {
		for (x = 0; x < image->width; x++) {
			ML_SetPixelColor(image, x, texHeight - y, ML_GetPixelColor(image, x, y));
		}
	}
}

void ML_InvertImageColors(ML_Image *image)
{
	unsigned int x, y;
	u32 color;

	for (y = 0; y < image->height; y++) {
		for (x = 0; x < image->width; x++) {
			color = ML_GetPixelColor(image, x, y);
			ML_SetPixelColor(image, x, y, ((0xFFFFFF - (color >> 8 & 0xFFFFFF)) << 8)  | (color & 0xFF));
		}
	}
}

void ML_ApplyGrayscaleToImage(ML_Image *image)
{
	unsigned int x, y;
	u32 color;
	u8 gray;

	for (y = 0; y < image->height; y++) {
		for (x = 0; x < image->width; x++) {
			color = ML_GetPixelColor(image, x, y);
			gray = (((color >> 24 & 0xFF)*77 + (color >> 16 & 0xFF)*150 + (color >> 8 & 0xFF)*28) >> 8);
			ML_SetPixelColor(image, x, y,  ((gray << 24) | (gray << 16) | (gray << 8) | (color & 0xFF)));
		}
	}
}

