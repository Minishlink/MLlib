#include <MLlib.h>

/**
* \file
* \brief This file contains input functions.
*/

u8 nb_wiimote = 1, nb_pads_gc = 0;
u8 _nunchuk_enabled = 0, _cpad_enabled = 0, _guitar_enabled = 0;

void ML_InitPad()
{
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_ALL, _screenWidth, _screenHeight);

	PAD_Init();
}

void ML_GetPads()
{
	ML_GetPadsWii();
	ML_GetPadsGC();
}

void ML_SetNumberOfPads(u8 wiimotesNumber, u8 gc_padsNumber)
{
	if(wiimotesNumber <= WPAD_MAX_WIIMOTES)	nb_wiimote = wiimotesNumber;
	nb_pads_gc = gc_padsNumber;
}

// ---------------
//       Wii
// ---------------

void ML_GetPadsWii()
{
	WPAD_ScanPads();

	u8 i = 0;
	u32 pressed = 0, up = 0;
	ir_t ir;
	orient_t orient;
	gforce_t gforce;
	float theta;
	u32 type;
	
	for(i = 0; i < nb_wiimote; i++)
	{
		pressed = WPAD_ButtonsHeld(i);
		up = WPAD_ButtonsUp(i);

		// Is enabled
		if((WPAD_Probe(i, &type)) == WPAD_ERR_NONE) Wiimote[i].Enabled = 1;
		else Wiimote[i].Enabled = 0;

		if(Wiimote[i].Enabled)
		{		
			// Button A
			Wiimote[i].Released.A = (up & WPAD_BUTTON_A);
			Wiimote[i].Newpress.A = (pressed & WPAD_BUTTON_A) && (!Wiimote[i].Held.A);
			Wiimote[i].Held.A = (pressed & WPAD_BUTTON_A);
			// Button B
			Wiimote[i].Released.B = (up & WPAD_BUTTON_B);
			Wiimote[i].Newpress.B = (pressed & WPAD_BUTTON_B) && (!Wiimote[i].Held.B);
			Wiimote[i].Held.B = (pressed & WPAD_BUTTON_B);
			// Button +
			Wiimote[i].Released.Plus = (up & WPAD_BUTTON_PLUS);
			Wiimote[i].Newpress.Plus = (pressed & WPAD_BUTTON_PLUS) && (!Wiimote[i].Held.Plus);
			Wiimote[i].Held.Plus = (pressed & WPAD_BUTTON_PLUS);
			// Button -
			Wiimote[i].Released.Minus = (up & WPAD_BUTTON_MINUS);
			Wiimote[i].Newpress.Minus = (pressed & WPAD_BUTTON_MINUS) && (!Wiimote[i].Held.Minus);
			Wiimote[i].Held.Minus = (pressed & WPAD_BUTTON_MINUS);
			// Button Home
			Wiimote[i].Released.Home = (up & WPAD_BUTTON_HOME);
			Wiimote[i].Newpress.Home = (pressed & WPAD_BUTTON_HOME) && (!Wiimote[i].Held.Home);
			Wiimote[i].Held.Home = (pressed & WPAD_BUTTON_HOME);
			// Button 1
			Wiimote[i].Released.One = (up & WPAD_BUTTON_1);
			Wiimote[i].Newpress.One = (pressed & WPAD_BUTTON_1) && (!Wiimote[i].Held.One);
			Wiimote[i].Held.One = (pressed & WPAD_BUTTON_1);
			// Button 2
			Wiimote[i].Released.Two = (up & WPAD_BUTTON_2);
			Wiimote[i].Newpress.Two = (pressed & WPAD_BUTTON_2) && (!Wiimote[i].Held.Two);
			Wiimote[i].Held.Two = (pressed & WPAD_BUTTON_2);
			// Button Up
			Wiimote[i].Released.Up = (up & WPAD_BUTTON_UP);
			Wiimote[i].Newpress.Up = (pressed & WPAD_BUTTON_UP) && (!Wiimote[i].Held.Up);
			Wiimote[i].Held.Up = (pressed & WPAD_BUTTON_UP);
			// Button Down
			Wiimote[i].Released.Down = (up & WPAD_BUTTON_DOWN);
			Wiimote[i].Newpress.Down = (pressed & WPAD_BUTTON_DOWN) && (!Wiimote[i].Held.Down);
			Wiimote[i].Held.Down = (pressed & WPAD_BUTTON_DOWN);
			// Button Left
			Wiimote[i].Released.Left = (up & WPAD_BUTTON_LEFT);
			Wiimote[i].Newpress.Left = (pressed & WPAD_BUTTON_LEFT) && (!Wiimote[i].Held.Left);
			Wiimote[i].Held.Left = (pressed & WPAD_BUTTON_LEFT);
			// Button Right
			Wiimote[i].Released.Right = (up & WPAD_BUTTON_RIGHT);
			Wiimote[i].Newpress.Right = (pressed & WPAD_BUTTON_RIGHT) && (!Wiimote[i].Held.Right);
			Wiimote[i].Held.Right = (pressed & WPAD_BUTTON_RIGHT);

			// IR
			WPAD_IR(i, &ir);
			Wiimote[i].IR.Valid = ir.valid;
			if(ir.valid)
			{
				Wiimote[i].IR.JustX = ir.x;
				Wiimote[i].IR.JustY = ir.y;
				Wiimote[i].IR.Angle = ir.angle;
	
				theta = Wiimote[i].IR.Angle / 180.0 * M_PI;
				Wiimote[i].IR.X = ir.x + 10*sinf(theta);
				Wiimote[i].IR.Y = ir.y - 10*cosf(theta);
			}
			if(ir.smooth_valid)
			{
				Wiimote[i].IR.SmoothX = ir.sx;
				Wiimote[i].IR.SmoothY = ir.sy;
			}
			if(ir.raw_valid) Wiimote[i].IR.Z = ir.z;

			// Orientation
			WPAD_Orientation(i, &orient);
			Wiimote[i].Orient.Roll = orient.roll;
			Wiimote[i].Orient.Pitch = orient.pitch;
			Wiimote[i].Orient.JustRoll = orient.a_roll;
			Wiimote[i].Orient.JustPitch = orient.a_pitch;
			Wiimote[i].Orient.Yaw = orient.yaw;

			// Gravity Force
			WPAD_GForce(i, &gforce);
			Wiimote[i].GForce.X = gforce.x;
			Wiimote[i].GForce.Y = gforce.y;
			Wiimote[i].GForce.Z = gforce.z;
		}
	}

	if(_nunchuk_enabled) ML_GetNunchuk();
	if(_cpad_enabled) ML_GetCPad();
	if(_guitar_enabled) ML_GetGuitar();
}

int ML_GetWiimoteExtension(u8 wpad)
{
	WPADData* data = WPAD_Data(wpad);
	return data->exp.type;
}

void ML_GetNunchuk()
{
	u32 pressed = 0, up = 0;
	u8 i = 0;
	expansion_t data;
	
	for(i = 0; i < nb_wiimote; i++)
	{
		pressed = WPAD_ButtonsHeld(i);
		up = WPAD_ButtonsUp(i);

		// Nunchuk C Button
		Nunchuk[i].Released.C = (up & WPAD_NUNCHUK_BUTTON_C);
		Nunchuk[i].Newpress.C = (pressed & WPAD_NUNCHUK_BUTTON_C) && (!Nunchuk[i].Held.C);
		Nunchuk[i].Held.C = (pressed & WPAD_NUNCHUK_BUTTON_C);

		// Nunchuk Z Button
		Nunchuk[i].Released.Z = (up & WPAD_NUNCHUK_BUTTON_Z);
		Nunchuk[i].Newpress.Z = (pressed & WPAD_NUNCHUK_BUTTON_Z) && (!Nunchuk[i].Held.Z);
		Nunchuk[i].Held.Z = (pressed & WPAD_NUNCHUK_BUTTON_Z);

		// Nunchuk Stick
		WPAD_Expansion(i, &data);
		Nunchuk[i].Stick.Angle = data.nunchuk.js.ang;    
		Nunchuk[i].Stick.Magnitude = data.nunchuk.js.mag; 
		Nunchuk[i].GForce.X = data.nunchuk.gforce.x;
		Nunchuk[i].GForce.Y = data.nunchuk.gforce.y;
		Nunchuk[i].GForce.Z = data.nunchuk.gforce.z;
		Nunchuk[i].Orient.Roll = data.nunchuk.orient.roll;
		Nunchuk[i].Orient.JustRoll = data.nunchuk.orient.a_roll;
		Nunchuk[i].Orient.Pitch = data.nunchuk.orient.pitch;
		Nunchuk[i].Orient.JustPitch = data.nunchuk.orient.a_pitch;
		Nunchuk[i].Orient.Yaw = data.nunchuk.orient.yaw;
	}

}

void ML_GetCPad()
{
	u32 pressed = 0, up = 0;
	u8 i = 0;
	expansion_t data;
	
	for(i = 0; i < nb_wiimote; i++)
	{
		pressed = WPAD_ButtonsHeld(i);
		up = WPAD_ButtonsUp(i);

		// Button A
		CPad[i].Released.A = (up & WPAD_CLASSIC_BUTTON_A);
		CPad[i].Newpress.A = (pressed & WPAD_CLASSIC_BUTTON_A) && (!CPad[i].Held.A);
		CPad[i].Held.A = (pressed & WPAD_CLASSIC_BUTTON_A);
		// Button B
		CPad[i].Released.B = (up & WPAD_CLASSIC_BUTTON_B);
		CPad[i].Newpress.B = (pressed & WPAD_CLASSIC_BUTTON_B) && (!CPad[i].Held.B);
		CPad[i].Held.B = (pressed & WPAD_CLASSIC_BUTTON_B);
		// Button X
		CPad[i].Released.X = (up & WPAD_CLASSIC_BUTTON_X);
		CPad[i].Newpress.X = (pressed & WPAD_CLASSIC_BUTTON_X) && (!CPad[i].Held.X);
		CPad[i].Held.X = (pressed & WPAD_CLASSIC_BUTTON_X);
		// Button Y
		CPad[i].Released.Y = (up & WPAD_CLASSIC_BUTTON_Y);
		CPad[i].Newpress.Y = (pressed & WPAD_CLASSIC_BUTTON_Y) && (!CPad[i].Held.Y);
		CPad[i].Held.Y = (pressed & WPAD_CLASSIC_BUTTON_Y);
		// Button Start
		CPad[i].Released.Start = (up & WPAD_CLASSIC_BUTTON_PLUS);
		CPad[i].Newpress.Start = (pressed & WPAD_CLASSIC_BUTTON_PLUS) && (!CPad[i].Held.Start);
		CPad[i].Held.Start = (pressed & WPAD_CLASSIC_BUTTON_PLUS);
		// Button Select
		CPad[i].Released.Select = (up & WPAD_CLASSIC_BUTTON_MINUS);
		CPad[i].Newpress.Select = (pressed & WPAD_CLASSIC_BUTTON_MINUS) && (!CPad[i].Held.Select);
		CPad[i].Held.Select = (pressed & WPAD_CLASSIC_BUTTON_MINUS);
		// Button Home
		CPad[i].Released.Home = (up & WPAD_CLASSIC_BUTTON_HOME);
		CPad[i].Newpress.Home = (pressed & WPAD_CLASSIC_BUTTON_HOME) && (!CPad[i].Held.Home);
		CPad[i].Held.Home = (pressed & WPAD_CLASSIC_BUTTON_HOME);
		// Button Up
		CPad[i].Released.Up = (up & WPAD_CLASSIC_BUTTON_UP);
		CPad[i].Newpress.Up = (pressed & WPAD_CLASSIC_BUTTON_UP) && (!CPad[i].Held.Up);
		CPad[i].Held.Up = (pressed & WPAD_CLASSIC_BUTTON_UP);
		// Button Down
		CPad[i].Released.Down = (up & WPAD_CLASSIC_BUTTON_DOWN);
		CPad[i].Newpress.Down = (pressed & WPAD_CLASSIC_BUTTON_DOWN) && (!CPad[i].Held.Down);
		CPad[i].Held.Down = (pressed & WPAD_CLASSIC_BUTTON_DOWN);
		// Button Left
		CPad[i].Released.Left = (up & WPAD_CLASSIC_BUTTON_LEFT);
		CPad[i].Newpress.Left = (pressed & WPAD_CLASSIC_BUTTON_LEFT) && (!CPad[i].Held.Left);
		CPad[i].Held.Left = (pressed & WPAD_CLASSIC_BUTTON_LEFT);
		// Button Right
		CPad[i].Released.Right = (up & WPAD_CLASSIC_BUTTON_RIGHT);
		CPad[i].Newpress.Right = (pressed & WPAD_CLASSIC_BUTTON_RIGHT) && (!CPad[i].Held.Right);
		CPad[i].Held.Right = (pressed & WPAD_CLASSIC_BUTTON_RIGHT);
		// Button L
		CPad[i].Released.L = (up & WPAD_CLASSIC_BUTTON_FULL_L);
		CPad[i].Newpress.L = (pressed & WPAD_CLASSIC_BUTTON_FULL_L) && (!CPad[i].Held.L);
		CPad[i].Held.L = (pressed & WPAD_CLASSIC_BUTTON_FULL_L);
		// Button R
		CPad[i].Released.R = (up & WPAD_CLASSIC_BUTTON_FULL_R);
		CPad[i].Newpress.R = (pressed & WPAD_CLASSIC_BUTTON_FULL_R) && (!CPad[i].Held.R);
		CPad[i].Held.R = (pressed & WPAD_CLASSIC_BUTTON_FULL_R);
		// Button ZL
		CPad[i].Released.ZL = (up & WPAD_CLASSIC_BUTTON_ZL);
		CPad[i].Newpress.ZL = (pressed & WPAD_CLASSIC_BUTTON_ZL) && (!CPad[i].Held.ZL);
		CPad[i].Held.ZL = (pressed & WPAD_CLASSIC_BUTTON_ZL);
		// Button ZR
		CPad[i].Released.ZR = (up & WPAD_CLASSIC_BUTTON_ZR);
		CPad[i].Newpress.ZR = (pressed & WPAD_CLASSIC_BUTTON_ZR) && (!CPad[i].Held.ZR);
		CPad[i].Held.ZR = (pressed & WPAD_CLASSIC_BUTTON_ZR);
		
		// Sticks
		WPAD_Expansion(i, &data);
		CPad[i].LeftStick.Angle = data.classic.ljs.ang;    
		CPad[i].LeftStick.Magnitude = data.classic.ljs.mag; 	
		CPad[i].RightStick.Angle = data.classic.rjs.ang;    
		CPad[i].RightStick.Magnitude = data.classic.rjs.mag; 
	}
}

void ML_GetGuitar()
{
	u32 pressed = 0, up = 0;
	u8 i = 0;
	expansion_t data;

	for(i = 0; i < nb_wiimote; i++)
	{
		pressed = WPAD_ButtonsHeld(i);
		up = WPAD_ButtonsUp(i);

		// Button +
		Guitar[i].Released.Plus = (up & WPAD_GUITAR_HERO_3_BUTTON_PLUS);
		Guitar[i].Newpress.Plus = (pressed & WPAD_GUITAR_HERO_3_BUTTON_PLUS) && (!Guitar[i].Held.Plus);
		Guitar[i].Held.Plus = (pressed & WPAD_GUITAR_HERO_3_BUTTON_PLUS);
		// Button -
		Guitar[i].Released.Minus = (up & WPAD_GUITAR_HERO_3_BUTTON_MINUS);
		Guitar[i].Newpress.Minus = (pressed & WPAD_GUITAR_HERO_3_BUTTON_MINUS) && (!Guitar[i].Held.Minus);
		Guitar[i].Held.Minus = (pressed & WPAD_GUITAR_HERO_3_BUTTON_MINUS);
		// Button Red
		Guitar[i].Released.Red = (up & WPAD_GUITAR_HERO_3_BUTTON_RED);
		Guitar[i].Newpress.Red = (pressed & WPAD_GUITAR_HERO_3_BUTTON_RED) && (!Guitar[i].Held.Red);
		Guitar[i].Held.Red = (pressed & WPAD_GUITAR_HERO_3_BUTTON_RED);
		// Button Yellow
		Guitar[i].Released.Yellow = (up & WPAD_GUITAR_HERO_3_BUTTON_YELLOW);
		Guitar[i].Newpress.Yellow = (pressed & WPAD_GUITAR_HERO_3_BUTTON_YELLOW) && (!Guitar[i].Held.Yellow);
		Guitar[i].Held.Yellow = (pressed & WPAD_GUITAR_HERO_3_BUTTON_YELLOW);
		// Button Green
		Guitar[i].Released.Green = (up & WPAD_GUITAR_HERO_3_BUTTON_GREEN);
		Guitar[i].Newpress.Green = (pressed & WPAD_GUITAR_HERO_3_BUTTON_GREEN) && (!Guitar[i].Held.Green);
		Guitar[i].Held.Green = (pressed & WPAD_GUITAR_HERO_3_BUTTON_GREEN);
		// Button Orange
		Guitar[i].Released.Orange = (up & WPAD_GUITAR_HERO_3_BUTTON_ORANGE);
		Guitar[i].Newpress.Orange = (pressed & WPAD_GUITAR_HERO_3_BUTTON_ORANGE) && (!Guitar[i].Held.Orange);
		Guitar[i].Held.Orange = (pressed & WPAD_GUITAR_HERO_3_BUTTON_ORANGE);
		// Button Blue
		Guitar[i].Released.Blue = (up & WPAD_GUITAR_HERO_3_BUTTON_BLUE);
		Guitar[i].Newpress.Blue = (pressed & WPAD_GUITAR_HERO_3_BUTTON_BLUE) && (!Guitar[i].Held.Blue);
		Guitar[i].Held.Blue = (pressed & WPAD_GUITAR_HERO_3_BUTTON_BLUE);
		// Button Strum Up
		Guitar[i].Released.StrumUp = (up & WPAD_GUITAR_HERO_3_BUTTON_STRUM_UP);
		Guitar[i].Newpress.StrumUp = (pressed & WPAD_GUITAR_HERO_3_BUTTON_STRUM_UP) && (!Guitar[i].Held.StrumUp);
		Guitar[i].Held.StrumUp = (pressed & WPAD_GUITAR_HERO_3_BUTTON_STRUM_UP);
		// Button Strum Down
		Guitar[i].Released.StrumDown = (up & WPAD_GUITAR_HERO_3_BUTTON_STRUM_DOWN);
		Guitar[i].Newpress.StrumDown = (pressed & WPAD_GUITAR_HERO_3_BUTTON_STRUM_DOWN) && (!Guitar[i].Held.StrumDown);
		Guitar[i].Held.StrumDown = (pressed & WPAD_GUITAR_HERO_3_BUTTON_STRUM_DOWN);
		
		// Stick
		WPAD_Expansion(i, &data);
		Guitar[i].Stick.Angle = data.gh3.js.ang;    
		Guitar[i].Stick.Magnitude = data.gh3.js.mag; 	
	}
}

bool ML_EnableNunchuk(u8 enabled) { if(enabled == 0 || enabled == 1) { _nunchuk_enabled = enabled; } return _nunchuk_enabled; }
bool ML_EnableCPad(u8 enabled) { if(enabled == 0 || enabled == 1) { _cpad_enabled = enabled; } return _cpad_enabled; }
bool ML_EnableGuitar(u8 enabled) { if(enabled == 0 || enabled == 1) { _guitar_enabled = enabled; } return _guitar_enabled; }

void ML_RumbleWiimote(s32 wpad, int enabled)
{
	WPAD_Rumble(wpad, enabled);
}

// ---------------
//       GC
// ---------------

void ML_GetPadsGC()
{
	PAD_ScanPads();

	u8 i = 0;
	u32 pressed = 0, up = 0;

	for(i = 0; i < nb_pads_gc; i++)
	{
		pressed = PAD_ButtonsHeld(i);
		up = PAD_ButtonsUp(i);

		// Button Start
		Pad[i].Released.Start = (up & PAD_BUTTON_START);
		Pad[i].Newpress.Start = (pressed & PAD_BUTTON_START) && (!Pad[i].Held.Start);
		Pad[i].Held.Start = (pressed & PAD_BUTTON_START);
		// Button A
		Pad[i].Released.A = (up & PAD_BUTTON_A);
		Pad[i].Newpress.A = (pressed & PAD_BUTTON_A) && (!Pad[i].Held.A);
		Pad[i].Held.A = (pressed & PAD_BUTTON_A);
		// Button B
		Pad[i].Released.B = (up & PAD_BUTTON_B);
		Pad[i].Newpress.B = (pressed & PAD_BUTTON_B) && (!Pad[i].Held.B);
		Pad[i].Held.B = (pressed & PAD_BUTTON_B);
		// Button X
		Pad[i].Released.X = (up & PAD_BUTTON_X);
		Pad[i].Newpress.X = (pressed & PAD_BUTTON_X) && (!Pad[i].Held.X);
		Pad[i].Held.X = (pressed & PAD_BUTTON_X);
		// Button Y
		Pad[i].Released.Y = (up & PAD_BUTTON_Y);
		Pad[i].Newpress.Y = (pressed & PAD_BUTTON_Y) && (!Pad[i].Held.Y);
		Pad[i].Held.Y = (pressed & PAD_BUTTON_Y);

		// Button Up
		Pad[i].Released.Up = (up & PAD_BUTTON_UP);
		Pad[i].Newpress.Up = (pressed & PAD_BUTTON_UP) && (!Pad[i].Held.Up);
		Pad[i].Held.Up = (pressed & PAD_BUTTON_UP);
		// Button Down
		Pad[i].Released.Down = (up & PAD_BUTTON_DOWN);
		Pad[i].Newpress.Down = (pressed & PAD_BUTTON_DOWN) && (!Pad[i].Held.Down);
		Pad[i].Held.Down = (pressed & PAD_BUTTON_DOWN);
		// Button Left
		Pad[i].Released.Left = (up &  PAD_BUTTON_LEFT);
		Pad[i].Newpress.Left = (pressed & PAD_BUTTON_LEFT) && (!Pad[i].Held.Left);
		Pad[i].Held.Left = (pressed & PAD_BUTTON_LEFT);
		// Button Right
		Pad[i].Released.Right = (up & PAD_BUTTON_RIGHT);
		Pad[i].Newpress.Right = (pressed & PAD_BUTTON_RIGHT) && (!Pad[i].Held.Right);
		Pad[i].Held.Right = (pressed & PAD_BUTTON_RIGHT);

		// Trigger L
		Pad[i].Released.L = (up & PAD_TRIGGER_L);
		Pad[i].Newpress.L = (pressed & PAD_TRIGGER_L) && (!Pad[i].Held.L);
		Pad[i].Held.L = (pressed & PAD_TRIGGER_L);
		// Trigger R
		Pad[i].Released.R = (up & PAD_TRIGGER_R);
		Pad[i].Newpress.R = (pressed & PAD_TRIGGER_R) && (!Pad[i].Held.R);
		Pad[i].Held.R = (pressed & PAD_TRIGGER_R);
		// Trigger Z
		Pad[i].Released.Z = (up & PAD_TRIGGER_Z);
		Pad[i].Newpress.Z = (pressed & PAD_TRIGGER_Z) && (!Pad[i].Held.Z);
		Pad[i].Held.Z = (pressed & PAD_TRIGGER_Z);

		// Stick X
		Pad[i].Stick.X = PAD_StickX(i);
		// Stick Y
		Pad[i].Stick.Y = PAD_StickY(i);

		// Stick Up
		Pad[i].Stick.Released.Up = ((!Pad[i].Stick.Y > 65) && (Pad[i].Stick.Held.Up));
		Pad[i].Stick.Newpress.Up = ((Pad[i].Stick.Y > 65) && (!Pad[i].Stick.Held.Up));
		Pad[i].Stick.Held.Up = (Pad[i].Stick.Y > 65);
		// Stick Down
		Pad[i].Stick.Released.Down = ((!Pad[i].Stick.Y < -65) && (Pad[i].Stick.Held.Down));
		Pad[i].Stick.Newpress.Down = ((Pad[i].Stick.Y < -65) && (!Pad[i].Stick.Held.Down));
		Pad[i].Stick.Held.Down = (Pad[i].Stick.Y < -65);
		// Stick Left
		Pad[i].Stick.Released.Left = ((!Pad[i].Stick.X < -65) && (Pad[i].Stick.Held.Left));
		Pad[i].Stick.Newpress.Left = ((Pad[i].Stick.X < -65) && (!Pad[i].Stick.Held.Left));
		Pad[i].Stick.Held.Left = (Pad[i].Stick.X < -65);
		// Stick Right
		Pad[i].Stick.Released.Right = ((!Pad[i].Stick.X > 65) && (Pad[i].Stick.Held.Right));
		Pad[i].Stick.Newpress.Right = ((Pad[i].Stick.X > 65) && (!Pad[i].Stick.Held.Right));
		Pad[i].Stick.Held.Right = (Pad[i].Stick.X > 65);
	}
}

// #### Thanks Botskiz for these lines ! :)  ####
//====================================================================
//	Reset callback function.
//====================================================================
void _doSystemReset(void)
{
	bReset = true;
}

//====================================================================
//	PowerOff callback function.
//====================================================================
void _doPowerOff(void)
{
	bPowerOff = true;
}

//====================================================================
//	PowerOff callback function for the Wii Remote power button.
//====================================================================
void _doPadPowerOff(s32 chan)
{
	if(chan == WPAD_CHAN_0)
		bPowerOff = true;
}


