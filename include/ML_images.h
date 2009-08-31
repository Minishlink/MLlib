#ifndef DEF_ML_IMAGES_H
#define DEF_ML_IMAGES_H

/**
* \file
* \brief This file contains images headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

inline void _initImage(ML_Image *image);

/**
* \fn void ML_CloneImage(ML_Image *image1, ML_Image *image2)
* \brief This function clones two images.
* @param image1 The original image
* @param image2 The image which will be the same as image1
*/
extern void ML_CloneImage(ML_Image *image1, ML_Image *image2);

/**
* \fn void ML_DeleteImage(ML_Image *image)
* \brief This function deletes an image. Call it when exiting the app/game or if you don't use it anymore.
* @param image The image which will be deleted
*/
extern void ML_DeleteImage(ML_Image *image);

/**
* \fn void ML_FlushImage(ML_Image *image)
* \brief This function refreshes the image when you have done some modifications with it.
* @param image Image
*/
extern void ML_FlushImage(ML_Image *image);

/**
* \fn void ML_SetPixelColor(ML_Image *image, int x, int y, u32 color)
* \brief This function sets the color value of a pixel on a sprite. You need to call ML_FlushImage() after doing your modifications ! ;)
* @param image Image
* @param x X position of the pixel
* @param y Y position of the pixel
* @param color New color of the pixel
* @see ML_FlushImage
*/
extern void ML_SetPixelColor(ML_Image *image,  int x, int y, u32 color);

/**
* \fn u32 ML_GetPixelColor(ML_Image *image, int x, int y)
* \brief This function returns the color value of the pixel on a sprite.
* @param image Image
* @param x X position of the pixel
* @param y Y position of the pixel
* @return Color of the pixel
*/
extern u32 ML_GetPixelColor(ML_Image *image, int x, int y);

/**
* \fn void ML_InvertImageColors(ML_Image *image)
* \brief This function inverts the image colors. You need to call ML_FlushImage() after doing your modifications ! ;)
* @param image Image
* @see ML_FlushImage
*/
extern void ML_InvertImageColors(ML_Image *image);

/**
* \fn void ML_ApplyGrayscaleToImage(ML_Image *image)
* \brief This function applies grayscale onto the image. You need to call ML_FlushImage() after doing your modifications ! ;)
* @param image Image
* @see ML_FlushImage
*/
extern void ML_ApplyGrayscaleToImage(ML_Image *image);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

