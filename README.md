# MLlib
The *MLlib* (for Minishlink's Library) is a simple library to develop on Nintendo Wii.
Technically, it's a wrapper of libogc and others librairies. Coding with this lib on Wii is way easier now ! :-)

More info [on Wiibrew](http://wiibrew.org/MLlib).

## Template
Here's the simple code to have the program running on Wii :
```C
#include <MLlib.h>

int main(int argc, char **argv) 
{
	ML_Init();
	ML_SplashScreen(); // You can delete this instruction, but please let it if you can :)
		
	while(1)
	{
		if(Wiimote[0].Newpress.Home) ML_Exit();
		
		ML_Refresh();
	}
	
	return 0;
}
```

## Authors :
 * Minishlink

## Special Thanks :
 * Chaosteil
 * Cid2Mizard
 * Crayon
 * Dhewg
 * Dykam
 * EvilSpoon
 * Feesh
 * Mollusk
 * Morukutsu
 * NoNameNo
 * Svpe
 * T4ils
 * WntrMute
 * Zeblackos
and these communities :
 * www.dev-fr.org + IRC Channel #dev-fr
 * www.nintendomax.com
 * www.wiibrew.org + IRC Channel #wiidev
 * www.wiigen.fr
 
This project was moved from https://code.google.com/p/mllib/
