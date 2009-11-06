#ifndef DEF_ML_DEFINE_H
#define DEF_ML_DEFINE_H

/**
* \file
* \brief This file contains some defines.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

#define PAD_ANY_BUTTON 0
#define PAD_BUTTON_L	PAD_TRIGGER_L
#define PAD_BUTTON_R PAD_TRIGGER_R
#define PAD_BUTTON_Z PAD_TRIGGER_Z

#define MAGENTA 0xf01e
#define BLACK   0x0000

/**
* \def GX_COLOR_WHITE
* \brief White color in a GXColor var.
*/
#define GX_COLOR_WHITE (GXColor){0xff, 0xff, 0xff, 0xff}

/**
* \def GX_COLOR_BLACK
* \brief Black color in a GXColor var.
*/
#define GX_COLOR_BLACK (GXColor){0x00, 0x00, 0x00, 0x00}

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
