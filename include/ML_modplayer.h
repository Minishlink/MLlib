#ifndef DEF_ML_MODPLAYER_H
#define DEF_ML_MODPLAYER_H

/**
* \file
* \brief This file contains mod player headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/**
* \fn void ML_InitMOD(MODPlay *play)
* \brief This function inits the mod's buffer.
* @param *play MOD's buffer
*/
extern void ML_InitMOD(MODPlay *play);

/**
* \fn void ML_PlayMODFromBuffer(MODPlay *play, const u8 *mod)
* \brief This function loads and plays a MOD.
* @param play MOD's buffer
* @param mod MOD
*/
extern void ML_PlayMODFromBuffer(MODPlay *play, const u8 *mod);

/**
* \fn void ML_PlayMODFromFile(MODPlay *play, const char *filename)
* \brief This function loads and plays a MOD located on a device such as a SD card.
* @param play MOD's buffer
* @param filename Name of your MOD located on a device such as a SD card
*/
extern void ML_PlayMODFromFile(MODPlay *play, const char *filename);

/**
* \fn bool ML_IsPlayingMOD()
* \brief This function checks if a MOD is currently playing.
* @return 1 if playing, 0 if not
*/
extern bool ML_IsPlayingMOD();

/**
* \fn void ML_LoadMOD(MODPlay *play, const void *mod)
* \brief This function loads the mod.
* @param play MOD's buffer
* @param mod MOD
*/
extern void ML_LoadMOD(MODPlay *play, const void *mod);

/**
* \fn void ML_PlayMOD(MODPlay *play)
* \brief This function plays the mod.
* @param play MOD's buffer
*/
extern void ML_PlayMOD(MODPlay *play);

/**
* \fn void ML_StopMOD(MODPlay *play)
* \brief This function stops playing the mod.
* @param play MOD's buffer
*/
extern void ML_StopMOD(MODPlay *play);

/**
* \fn void ML_PauseMOD(MODPlay *play, bool pause)
* \brief This function set to pause the playing mod.
* @param play MOD's buffer
* @param pause Set to 0 to set to pause and 1 to unset.
*/
extern void ML_PauseMOD(MODPlay *play, bool pause);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
