#include <MLlib.h>

/**
* \file
* \brief This file contains init functions.
*/

//---------------------------------------------

void ML_Init()
{
	ML_InitVideo();
	ML_GX_Init();
	ML_InitPad();
	
	setPowerOff(FALSE);
	setReset(FALSE);
	setAsnd(FALSE);
	setTTF(FALSE);
	setAA(FALSE);
	
	SYS_SetResetCallback(_doSystemReset);
	SYS_SetPowerCallback(_doPowerOff);
	WPAD_SetPowerButtonCallback(_doPadPowerOff);
	
	VIDEO_WaitVSync();
	
	setStartTime(gettime() / TB_TIMER_CLOCK);
}

//---------------------------------------------

void ML_InitConsoleMode()
{
	ML_InitVideo();
	VIDEO_SetBlack(FALSE);
	ML_InitPad();
	
	setPowerOff(FALSE);
	setReset(FALSE);
	setAsnd(FALSE);
	setTTF(FALSE);
	setAA(FALSE);
	
	SYS_SetResetCallback(_doSystemReset);
	SYS_SetPowerCallback(_doPowerOff);
	WPAD_SetPowerButtonCallback(_doPadPowerOff);
	
	VIDEO_WaitVSync();
	
	setStartTime(gettime() / TB_TIMER_CLOCK);
}

//---------------------------------------------

void ML_InitFAT()
{
	fatInitDefault();
}




