#ifndef DEF_ML_GLOBALS_H
#define DEF_ML_GLOBALS_H

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

bool _asnd_enabled;
bool bPowerOff;	// Poweroff flag.
bool bReset;	// Reset flag.

extern void setAsnd(bool value);
extern bool getAsnd();
extern void setPowerOff(bool value);
extern void setReset(bool value);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

