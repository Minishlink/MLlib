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
* \fn void ML_FlipImageX(ML_Image *image)
* \brief This function flips the image horizontally.
* @param image Image
*/
extern void ML_FlipImageX(ML_Image *image);

/**
* \fn void ML_FlipImageY(ML_Image *image)
* \brief This function flips the image vertically.
* @param image Image
*/
extern void ML_FlipImageY(ML_Image *image);

/**
* \fn void ML_InvertImageColors(ML_Image *image)
* \brief This function inverts the image colors.
* @param image Image
*/
extern void ML_InvertImageColors(ML_Image *image);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

