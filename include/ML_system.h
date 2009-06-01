#ifndef DEF_ML_SYSTEM_H
#define DEF_ML_SYSTEM_H

/**
* \file
* \brief This file contains system headers.
*/

enum T_WiiPower
{
    RESET   	= 0x2001,
    RESET_PD 	= 0x2002,
    SHUTDOWN   = 0x2003,
    IDLE       = 0x2004,
    WAKEUP     = 0x2005
};

/** 
* \def ML_ReturnToWii();
* \brief Returns to the Wii Menu.
*/
#define ML_ReturnToWii(); SYS_ResetSystem(SYS_RETURNTOMENU,0,0); 

/** 
* \def ML_ShutdownWii();
* \brief Shutdowns the Wii.
*/
#define ML_ShutdownWii(); SYS_ResetSystem(SYS_POWEROFF,0,0);

/**
* \fn extern void ML_Exit()
* \brief This function exits the app/game. If it was launched from HBC/Wiiload, it will return to the HBC; if not, it will return to the Wii menu. :)
*/
extern void ML_Exit();

/**
* \fn extern void ML_ExitConsoleMode()
* \brief CONSOLE MODE - This function exits the app/game. If it was launched from HBC/Wiiload, it will return to the HBC; if not, it will return to the Wii menu. :)
*/
extern void ML_ExitConsoleMode();

/**
* \fn extern void ML_SetPowerMode(int value)
* \brief NOT RECOMMENDED - This function set the wii power mode to the value (see enum T_WiiPower). It's better to use ML_ReturnToWii(); and ML_ShutdownWii();. ML_Exit(); is the best though !
* @param value The Wii will set itself to this value power mode.
*/
extern void ML_SetPowerMode(int value);

void ML_CallbackForPowerAndReset(bool GXyes);

void _flushAndClean(bool GXyes);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
