#include <MLlib.h>

/**
* \file
* \brief This file contains (RTC) time functions.
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
