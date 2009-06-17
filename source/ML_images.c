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

