#include <MLlib.h>

/**
* \file
* \brief This file contains time functions and rand functions.
*/

//---------------------------------------------

time_t ML_GetTime()
{
	time_t NGCtime;
	return NGCtime = time(NULL);
}

//---------------------------------------------

char* ML_GetTimeChar()
{
	time_t time = ML_GetTime();
	return ctime(&time);
}

//---------------------------------------------

void ML_InitRand()
{	
	srand(time(NULL));
}

//---------------------------------------------

int ML_Rand(int min, int max)
{
	return (rand() % max) + min; 
}

//---------------------------------------------
s8 _fps = 0, // fps
_fps_n = 0, // counter
_fps_time = 0, // time
_fps_last_time = 0; // time-1

// Made by Hackotedelaplaque, thanks !
int ML_GetFPS()
{
	_fps_n++;
	_fps_time = ML_GetTime();
	
	if(_fps_last_time == 0) _fps_last_time = _fps_time;
	else if(_fps_time != _fps_last_time)
	{
		_fps_last_time = _fps_time;
		_fps = _fps_n;
		
		_fps_n = 0;
	}
	
	return _fps;
}

