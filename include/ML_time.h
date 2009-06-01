#ifndef DEF_ML_TIME_H
#define DEF_ML_TIME_H

/**
* \file
* \brief This file contains time headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/**
* \fn extern time_t ML_GetTime()
* \brief This function gets RTC time.
* @return Return time in a time_t struct.
*/
extern time_t ML_GetTime();

/**
* \fn extern char* ML_GetTimeChar()
* \brief This function gets RTC time in char.
* @return Return time in char.
*/
extern char* ML_GetTimeChar();

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
