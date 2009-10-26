#include <MLlib.h>

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

