#ifndef DEF_ML_TIME_H
#define DEF_ML_TIME_H

/**
* \file
* \brief This file contains time and random headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/**
* \fn time_t ML_GetTime()
* \brief This function gets RTC time.
* @return Return time in a time_t struct.
*/
extern time_t ML_GetTime();

/**
* \fn char* ML_GetTimeChar()
* \brief This function gets RTC time in char.
* @return Return time in char.
*/
extern char* ML_GetTimeChar();

/**
* \fn int ML_InitRand()
* \brief This function initializes the random system. Call this once.
*/
extern void ML_InitRand();

/**
* \fn int ML_Rand(int min, int max)
* \brief This function returns a random number between "min" and "max". Call this function after ML_InitRand().
* @param min Minimum number
* @param max Maximum number
* @return Return the random number.
*/
extern int ML_Rand(int min, int max);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
