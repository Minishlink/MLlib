#include <MLlib.h>

/**
* \file
* \brief This file contains system functions.
*/

//---------------------------------------------

void ML_SetPowerMode(int value)
{
	int fd = -1;

	_flushAndClean(1, 1);

	if (!((value > 0x2000) && (value < 0x2006))) { return; }
	fd = IOS_Open("/dev/stm/immediate", 0);
	IOS_Ioctl(fd, value, NULL, 0, NULL, 0);
	IOS_Close(fd);
}

//---------------------------------------------

void ML_Exit()
{
	_flushAndClean(1, 1);
	
	char* sig = (char *)0x80001804;
	if( sig[0] == 'S' &&
		sig[1] == 'T' &&
		sig[2] == 'U' &&
		sig[3] == 'B' &&
		sig[4] == 'H' &&
		sig[5] == 'A' &&
		sig[6] == 'X' &&
		sig[7] == 'X' )
	exit(0);
	else SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

//---------------------------------------------

void ML_ExitConsoleMode()
{
	_flushAndClean(0, 1);
	
	char * sig = (char *)0x80001804;
	if( sig[0] == 'S' &&
		sig[1] == 'T' &&
		sig[2] == 'U' &&
		sig[3] == 'B' &&
		sig[4] == 'H' &&
		sig[5] == 'A' &&
		sig[6] == 'X' &&
		sig[7] == 'X' )
	exit(0);
	else SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

//---------------------------------------------

void ML_ReturnToWii()
{
	_flushAndClean(1, 0);
	SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

void ML_ShutdownWii()
{
	_flushAndClean(1, 0);
	SYS_ResetSystem(SYS_POWEROFF, 0, 0);
}

//---------------------------------------------

void ML_CallbackForPowerAndReset(bool GXyes) // Thanks Botskiz !
{	
	if(bReset) // Reset the system.
	{
		_flushAndClean(GXyes, 0);
		SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
	}
	else if(bPowerOff) // Shut the system down.
	{
		_flushAndClean(GXyes, 0);
		SYS_ResetSystem(SYS_POWEROFF, 0, 0);
	}
}


// ----------------------------------------

void _flushAndClean(bool GXyes, bool refresh)
{	
	if(refresh)
	{
		if(GXyes) ML_Refresh();
		else ML_RefreshConsoleMode();
	}
	
	if(getAsnd()) { ASND_Pause(1); ASND_End(); setAsnd(FALSE); }
	if(getTTF()) { ML_QuitFont(); setTTF(FALSE); }
	
	VIDEO_SetBlack(TRUE);
	
	if(GXyes)
	{
		ML_DisableClipping();
		GX_DrawDone();
		GX_AbortFrame();
		GX_Flush();   
		ML_GX_FreeMemory();
	} 
    
    VIDEO_Flush();
}

// ----------------------------------------

void ML_Debug(const char* filename)
{
	if(fatInitDefault())
	{
		chdir("/");
		mkdir("logs", 0755);
		chdir("/logs/");
		FILE *f;
		f = fopen(filename, "wb");
		if(f) fclose(f);
	}
}

