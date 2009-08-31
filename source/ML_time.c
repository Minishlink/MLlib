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


