#include <MLlib.h>

/**
* \file
* \brief This file contains mp3 player functions.
*/

//---------------------------------------------

void ML_InitMP3()
{
	ASND_Init();
	setAsnd(TRUE);
	MP3Player_Init();
}

//---------------------------------------------

void ML_StopMP3()
{
	MP3Player_Stop();
}

//---------------------------------------------

bool ML_IsPlayingMP3()
{
	return MP3Player_IsPlaying();
}

//---------------------------------------------

void ML_PlayMP3FromBuffer(const u8 *filename, u32 len)
{
	void *mp3_buffer = (void *)filename;
	MP3Player_PlayBuffer(mp3_buffer, len, NULL);
}

//---------------------------------------------

void ML_PlayMP3FromFile(const char* filename)
{
	FILE *fp = fopen(filename,"rb");
	if(fp == NULL) return;
	
	fseek(fp, 0, SEEK_END);  // Obtient la taille du fichier
	long size = ftell(fp);
	rewind (fp);
	
	char *buffer = (char *)malloc(size);
	if(buffer ==  NULL) return;
	
	fread (buffer, 1, size, fp);
	fclose (fp);
	
	MP3Player_PlayBuffer(buffer, size, NULL);
	
	free(buffer);
}

