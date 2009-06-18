#include <MLlib.h>

/**
* \file
* \brief This file contains backgrounds functions.
*/

void _initBackground(ML_Background *background)
{	
	*background = (ML_Background){0};
	background->scaleX = 1;
	background->scaleY = 1;
	background->alpha = 255;
}

void ML_CloneBackground(ML_Background *background1, ML_Background *background2)
{
	*background2 = *background1;
}

void ML_RotateBackground(ML_Background *background, float angle, u8 autoRotate)
{
	background->rotated = autoRotate;	
	if(background->rotated)
	{
		background->angle += angle;
	} else background->angle = angle;
}

void ML_SetBackgroundSize(ML_Background *background, u16 width, u16 height) { background->width = width; background->height = height; }
void ML_SetBackgroundScale(ML_Background *background, float scaleX, float scaleY) { background->scaleX = scaleX; background->scaleY = scaleY; }
void ML_SetBackgroundAlpha(ML_Background *background, u8 alpha) { background->alpha = alpha; }

