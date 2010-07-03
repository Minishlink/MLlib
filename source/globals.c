#include <MLlib.h>

bool _asnd_enabled; // Sound enabled
bool bPowerOff;	// Poweroff flag.
bool bReset;	// Reset flag.
bool _ttf_enabled; // Font system enabled
bool _aa_enabled; // Anti-aliasing for textures enabled

void setAsnd(bool value)
{
	_asnd_enabled = value;
}

bool getAsnd() 
{
	return _asnd_enabled;
}

void setPowerOff(bool value)
{
	bPowerOff = value;
}
 
void setReset(bool value)
{
	bReset = value;
}

bool getTTF()
{
	return _ttf_enabled;
}

void setTTF(bool value)
{
	_ttf_enabled = value;
}

bool getAA()
{
	return _aa_enabled;
}

void setAA(bool value)
{
	_aa_enabled = value;
}

