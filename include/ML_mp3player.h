#ifndef DEF_ML_MP3PLAYER_H
#define DEF_ML_MP3PLAYER_H

/**
* \file
* \brief This file contains mp3 player headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */
   
/**
* \fn extern void ML_InitMP3()
* \brief This function inits the MP3 player.
*/
extern void ML_InitMP3();

/**
* \fn extern void ML_StopMP3()
* \brief This function stops MP3s.
*/
extern void ML_StopMP3();

/**
* \fn bool ML_IsPlayingMP3()
* \brief This function detects if any MP3 is playing.
* @return Return a bool. (TRUE if playing)
*/
extern bool ML_IsPlayingMP3();

/**
* \fn void ML_PlayMP3FromBuffer(const u8 *filename, u32 len)
* \brief This function plays a MP3 from a buffer.
* @param filename MP3 name like "music_mp3"
* @param len Lenght of the MP3 like "music_mp3_size" s
*/
void ML_PlayMP3FromBuffer(const u8 *filename, u32 len);

/**
* \fn void ML_PlayMP3FromFile(const char* filename);
* \brief This function plays a MP3 from a file.
* @param filename The name of the file located on a fat device. (SD Card)
*/
void ML_PlayMP3FromFile(const char* filename);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
