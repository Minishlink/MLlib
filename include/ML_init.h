#ifndef DEF_ML_INIT_H
#define DEF_ML_INIT_H

/**
* \file
* \brief This file contains init headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/**
* \fn extern void ML_Init()
* \brief This function inits the MLlib.
*/
extern void ML_Init();

/**
* \fn extern void ML_InitConsoleMode()
* \brief CONSOLE MODE - This function inits the MLlib.
*/
extern void ML_InitConsoleMode();

/**
* \fn extern void ML_InitFAT()
* \brief This function initializes FAT (access to SD card and so on)
*/
extern void ML_InitFAT();

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

