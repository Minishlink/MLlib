#include <MLlib.h>

int main(int argc, char **argv) 
{	ML_Init();
	ML_SplashScreen(); // You can delete this instruction, but please let it if you can :)
		
	while(1)
	{
		if(Wiimote[0].Newpress.Home) ML_Exit();
		
		ML_Refresh();
	}
	
	return 0;
}

