#include <MLlib.h>

/**
* \file
* \brief This file contains backgrounds functions.
*/

void _initBackground(ML_Background *background)
{	
	*background = (ML_Background){0};
	background->visible = TRUE;
	background->scaleX = 1;
	background->scaleY = 1;
	background->alpha = 255;
}

void ML_CloneBackground(ML_Background *background1, ML_Background *background2)
{
	*background2 = *background1;
}

