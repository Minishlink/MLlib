#ifndef DEF_ML_GLOBALS_H
#define DEF_ML_GLOBALS_H

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

extern bool _asnd_enabled; // Sound enabled
extern bool bPowerOff;	// Poweroff flag.
extern bool bReset;	// Reset flag.
extern bool _ttf_enabled; // Font system enabled
extern bool _aa_enabled; // Anti-aliasing for textures enabled

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

