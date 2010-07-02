#ifndef DEF_ML_GLOBALS_H
#define DEF_ML_GLOBALS_H

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

bool _asnd_enabled; // Sound enabled
bool bPowerOff;	// Poweroff flag.
bool bReset;	// Reset flag.
bool _ttf_enabled; // Font system enabled
bool _aa_enabled; // anti-alisaing for textures enabled

extern void setAsnd(bool value);
extern bool getAsnd();
extern void setPowerOff(bool value);
extern void setReset(bool value);
extern void setTTF(bool value);
extern bool getTTF();
extern void setAA(bool value);
extern bool getAA();

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

