#ifndef DEF_ML_MATH_H
#define DEF_ML_MATH_H

/**
* \file
* \brief This file contains math headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/**
* \fn float ML_Distance(s32 x1, s32 y1, s32 x2, s32 y2)
* \brief This function calculates the squared distance between two points.
* @param x1 X position of the first point
* @param y1 Y position of the first point
* @param x2 X position of the second point
* @param y2 Y position of the second point
* @return squared distance between the two points
*/
extern float ML_Distance(s32 x1, s32 y1, s32 x2, s32 y2);

/**
* \fn float ML_TrueDistance(s32 x1, s32 y1, s32 x2, s32 y2)
* \brief This function calculates the distance between two points. (much slower than ML_Distance)
* @param x1 X position of the first point
* @param y1 Y position of the first point
* @param x2 X position of the second point
* @param y2 Y position of the second point
* @return distance between the two points
*/
extern float ML_TrueDistance(s32 x1, s32 y1, s32 x2, s32 y2);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

