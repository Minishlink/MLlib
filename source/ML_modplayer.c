#include <MLlib.h>

/**
* \file
* \brief This file contains mod player functions.
*/

//---------------------------------------------

void ML_InitMOD(MODPlay *play)
{	
	if(!getAsnd())
	{
		ASND_Init();
		setAsnd(TRUE);
	}
	MODPlay_Init(play);
}

//---------------------------------------------

void ML_PlayMODFromBuffer(MODPlay *play, const u8 *mod)
{
	void *mod_buffer = (void *)mod;
	MODPlay_SetMOD(play, mod_buffer);
	MODPlay_Start(play);
}

//---------------------------------------------

void ML_PlayMODFromFile(MODPlay *play, const char *filename)
{	

	FILE *fp = fopen(filename,"rb");
	if(fp == NULL) return;
	
	fseek(fp, 0 , SEEK_END);  // Obtient la taille du fichier
	long size = ftell(fp);
	rewind (fp);
	
	char *buffer = (char *)malloc(size);
	if(buffer ==  NULL) return;
	
	fread (buffer, 1, size, fp);
	fclose (fp);
	
	MODPlay_SetMOD(play, buffer);
	MODPlay_Start(play);
	
	free(buffer);
}

//---------------------------------------------

bool ML_IsPlayingMOD()
{
	if(ASND_StatusVoice(0) == SND_UNUSED) return 0;
	else return 1;
}

//---------------------------------------------

void ML_LoadMOD(MODPlay *play, const void *mod)
{
	MODPlay_SetMOD(play, mod);
}

//---------------------------------------------

void ML_PlayMOD(MODPlay *play)
{
	MODPlay_Start(play);
}

//---------------------------------------------

void ML_StopMOD(MODPlay *play)
{
	MODPlay_Stop(play);
}

//---------------------------------------------

void ML_PauseMOD(MODPlay *play, bool pause)
{
	MODPlay_Pause(play, pause);
}

