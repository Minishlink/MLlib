#include <MLlib.h>

/**
* \file
* \brief This file contains math functions.
*/

inline float ML_Distance(s32 x1, s32 y1, s32 x2, s32 y2)
{
	s64 h = x1 - x2;
	s64 v = y1 - y2;
	
	return(h*h + v*v);
}

inline float ML_TrueDistance(s32 x1, s32 y1, s32 x2, s32 y2)
{
	s64 h = x1 - x2;
	s64 v = y1 - y2;
	
	return(sqrt(h*h + v*v));
}

