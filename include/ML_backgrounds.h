#ifndef DEF_ML_BACKGROUND_H
#define DEF_ML_BACKGROUND_H

/**
* \file
* \brief This file contains backgrounds headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

inline void _initBackground(ML_Background *background);

/**
* \fn void ML_CloneBackground(ML_Background *background1, ML_Background *background2)
* \brief This function clones two backgrounds.
* @param background1 The original background
* @param background2 The background which will be the same as background1
*/
extern void ML_CloneBackground(ML_Background *background1, ML_Background *background2);

/**
* \fn void ML_RotateBackground(ML_Background *background, float angle, u8 autoRotate)
* \brief This function rotates the background.
* @param background Background
* @param angle Angle of rotation (degrees)
* @param autoRotate If TRUE, the background will rotate of <angle> degrees each frame. If it's FALSE, the background will rotate to the angle indicated.
*/
extern void ML_RotateBackground(ML_Background *background, float angle, u8 autoRotate);

/**
* \fn void ML_SetBackgroundScale(ML_Background *background, float scaleX, float scaleY)
* \brief This function changes the Scale X and Scale Y of the background.
* @param background Background
* @param scaleX New scale for the width of the background
* @param scaleY New scale for the height of the background
*/
extern void ML_SetBackgroundScale(ML_Background *background, float scaleX, float scaleY);

/**
* \fn void ML_SetBackgroundSize(ML_Background *background, u16 width, u16 height)
* \brief This function changes the size of your background.
* @param background Background
* @param width New width
* @param height New height
*/
extern void ML_SetBackgroundSize(ML_Background *background, u16 width, u16 height);

/**
* \fn void ML_SetBackgroundAlpha(ML_Background *background, u8 alpha)
* \brief This function set the transparency of the background.
* @param background Background
* @param alpha Transparency ( 0 -> 255 )
*/
extern void ML_SetBackgroundAlpha(ML_Background *background, u8 alpha);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

