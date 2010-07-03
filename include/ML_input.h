#ifndef DEF_ML_INPUT_H
#define DEF_ML_INPUT_H

/**
* \file
* \brief This file contains input headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

// ---------------
//       Wii
// ---------------

typedef char byte;

/**
* \struct Wii_WPad
* \brief This structure is a part of the WPads struct.
*/
typedef struct {
	int A, /*!< A button */
	B, /*!< B button */
	Plus, /*!< Plus button */
	Minus, /*!< Minus button */
	Home, /*!< Home button */
	One, /*!< One button */
	Two, /*!< Two button */
	Up, /*!< Up button */
	Down, /*!< Down button */
	Left, /*!< Left button */
	Right; /*!< Right button */
} Wii_WPad;

/**
* \struct Wii_IR
* \brief This structure is a part of the WPads struct. (IR/Cursor)
*/
typedef struct {
	float X, /*!< X pos of Wiimote IR */
	Y, /*!< Y pos of Wiimote IR */
	Z, /*!< Distance from the Screen<->Wiimote */
	Angle,
	SmoothX, /*!< Not recommended X pos of Wiimote IR (smoothed) */
	SmoothY, /*!< Not recommended Y pos of Wiimote IR (smoothed) */
	JustX, /*!< Not recommended X pos of Wiimote IR (original) */
	JustY; /*!< Not recommended Y pos of Wiimote IR (original) */
	bool Valid; /*!< Is the value valid ? You don't have to use it normally. */
} Wii_IR;

/**
* \struct Wii_GForce
* \brief This structure is a part of the WPads, Nunchuks struct. (Gravity Force)
*/
typedef struct {
	float X,  /*!< Horizontal force (-3; 3) */
	Y, /*!< Vertical force (-3; 3) */
	Z; /*!< Depth force (-3; 3) */
} Wii_GForce;

/**
* \struct Wii_Orientation
* \brief This structure is a part of the WPads, Nunchuks structs. (Orientation)
*/
typedef struct {
	float Roll, /*!< Roll value from -180 to 180 degrees */
	Pitch, /*!< Pitch value from -180 to 180 degrees */
	JustRoll, /*!< Original roll value from -180 to 180 degrees */
	JustPitch, /*!< Original pitch value from -180 to 180 degrees */
	Yaw; /*!< Yaw value from -26 to 26 degrees */
} Wii_Orientation;

/**
* \struct WPads
* \brief This structure contains all the Wiimote features values.
*/
typedef struct {
	Wii_WPad Held, /*!< see struct Wii_WPad */
	Released, /*!< see struct Wii_WPad */
	Newpress; /*!< see struct Wii_WPad */
	Wii_IR IR; /*!< see struct Wii_IR */
	Wii_GForce GForce; /*!< see struct Wii_GForce */
	Wii_Orientation Orient; /*!< see struct Wii_Orientation */
	bool Enabled; /*!< Is the Nunchuk enabled ? */
} WPads;

extern WPads Wiimote[WPAD_MAX_WIIMOTES];

/**
* \struct Nunchuk_Buttons
* \brief This structure is a part of the Nunchuks struct. (Buttons)
*/
typedef struct {
	int C, /*!< Button C on the Nunchuk */
	Z; /*!< Button Z on the Nunchuk */
} Nunchuk_Buttons;

/**
* \struct Joystick
* \brief This structure is a part of the Nunchuks, CPads structs. (Joystick)
*
* Angle is relative to the positive y-axis into quadrant I and ranges from 0 to 360 degrees. So if the joystick is held straight upwards then angle is 0 degrees. If it is held to the right it is 90, down is 180, and left is 270.\n
* Magnitude is the distance from the center to where the joystick is being held. The magnitude ranges from 0 to 1. If the joystick is only slightly tilted from the center the magnitude will be low, but if it is closer to the outter edge the value will be higher. (from Wiiuse documentation) 
*/
typedef struct {
	float Angle, /*!< Angle from 0 to 360 */
	Magnitude; /*!< Magnitude from 0 to 1 */
} Joystick;

/**
* \struct Nunchuks
* \brief This structure contains all the Nunchuk features values.
*/
typedef struct {
	Nunchuk_Buttons Held, /*!< see struct Nunchuk_Buttons */
	Released, /*!< see struct Nunchuk_Buttons */
	Newpress; /*!< see struct Nunchuk_Buttons */
	Joystick Stick; /*!< see struct Joystick */
	Wii_GForce GForce; /*!< see struct Wii_GForce */
	Wii_Orientation Orient; /*!< see struct Wii_Orientation */
} Nunchuks;

extern Nunchuks Nunchuk[WPAD_MAX_WIIMOTES];

/**
* \struct CPad_Buttons
* \brief This structure is a part of the CPads struct.
*/
typedef struct {
	int A, /*!< A button */
	B, /*!< B button */
	X, /*!< X button */
	Y, /*!< Y button */
	L, /*!< Left Trigger */
	R, /*!< Right Trigger */
	ZL, /*!< Left Trigger Z */
	ZR, /*!< Right Trigger Z */
	Start, /*!< Start button */
	Select, /*!< Select button */
	Home, /*!< Home button */
	Up, /*!< Up button */
	Down, /*!< Down button */
	Left, /*!< Left button */
	Right; /*!< Right button */
} CPad_Buttons;

/**
* \struct CPads
* \brief This structure contains all the Classic Controler features values.
*/
typedef struct {
	CPad_Buttons Held, /*!< see struct CPad_Buttons */
	Released, /*!< see struct CPad_Buttons */
	Newpress; /*!< see struct CPad_Buttons */
	Joystick LeftStick, /*!< see struct Joystick */
	RightStick; /*!< see struct Joystick */
} CPads;

extern CPads CPad[WPAD_MAX_WIIMOTES];

/**
* \struct GH_Buttons
* \brief This structure is a part of the GH struct.
*/
typedef struct {
	int Plus, /*!< Plus button */
	Minus, /*!< Minus button */
	StrumUp, /*!< Strum Up button */
	StrumDown, /*!< Strum Down button */
	Yellow, /*!< Yellow button */
	Green, /*!< Green button */
	Blue, /*!< Blue button */
	Red, /*!< Red button */
	Orange; /*!< Orange button */
} GH_Buttons;

/**
* \struct GH
* \brief This structure contains all the Guitar Hero features values.
*/
typedef struct {
	GH_Buttons Held, /*!< see struct GH_Buttons */
	Released, /*!< see struct GH_Buttons */
	Newpress; /*!< see struct GH_Buttons */
	Joystick Stick;
} GH;

extern GH Guitar[WPAD_MAX_WIIMOTES];


// ---------------
//       GC
// ---------------

/**
* \struct GC_Pad
* \brief This structure is a part of the GC_Pads struct.
*/
typedef struct {
	int A, /*!< A button */
	B, /*!< B button */
	X, /*!< X button */
	Y, /*!< Y button */
	Up, /*!< Up button */
	Down, /*!< Down button */
	Left, /*!< Left button */
	Right, /*!< Right button */
	L, /*!< L button */
	R, /*!< R button */
	Z, /*!< Z button */
	Start; /*!< Start button */
} GC_Pad;

/**
* \struct GC_Stick
* \brief This structure is a part of the GC_Pads struct.
*/
typedef struct {
	u8 Up, /*!< Stick Up */
	Down, /*!< Stick Down */
	Left, /*!< Stick Left */
	Right; /*!< Stick Right */
} GC_Stick;

/**
* \struct GC_Sticks
* \brief This structure is a part of the GC_Pads struct.
*/
typedef struct {
	GC_Stick Held, /*!< see struct GC_Stick */
	Released, /*!< see struct GC_Stick */
	Newpress; /*!< see struct GC_Stick */
	int X, /*!< X angle */
	Y; /*!< Y angle */
} GC_Sticks;

/**
* \struct GC_Pads
* \brief This structure contains all the GC pads features values.
*/
typedef struct {
	GC_Pad Held, /*!< see struct GC_Pad */
	Released, /*!< see struct GC_Pad */
	Newpress; /*!< see struct GC_Pad */
	GC_Sticks Stick; /*!< see struct GC_Sticks */
} GC_Pads;

extern GC_Pads Pad[4];

/**
* \fn void ML_InitPad()
* \brief This function inits pads (Wii and GC).
*/
extern void ML_InitPad();
extern void ML_InitPadWii();
extern void ML_InitPadGC();

/**
* \fn void ML_SetNumberOfPads(u8 wiimotesNumber, u8 gc_padsNumber)
* \brief This function sets the number of Wiimotes and GCN pads.
* @param wiimotesNumber Number of Wiimotes
* @param gc_padsNumber Number of GCN pads
*/
extern void ML_SetNumberOfPads(u8 wiimotesNumber, u8 gc_padsNumber);


/**
* \fn void ML_GetPads()
* \brief This function receives the pads (Wii and GC) status.
*/
extern void ML_GetPads();
extern void ML_GetPadsWii();
extern void ML_GetNunchuk();
extern void ML_GetCPad();
extern void ML_GetGuitar();
extern void ML_GetPadsGC();

/**
* \fn int ML_GetWiimoteExtension(u8 wpad)
* \brief This function checks which Wiimote extension is connected.
* @param wpad Which wiimote
* @return WPAD_EXP_NONE if none; WPAD_EXP_NUNCHUK if nunchuk is connected; WPAD_EXP_CLASSIC if Classic Pad is connected; and so on...
*/
extern int ML_GetWiimoteExtension(u8 wpad);

/**
* \fn void ML_EnableNunchuk(u8 enabled)
* You can type 3 as "enabled", it will return if it's already enabled.
* \brief This function sets if the Nunchuk is enabled or not.
* @param enabled Nunchuk enabled (1) or not (0)
* @return 1 if enabled, 0 if disabled
*/
extern bool ML_EnableNunchuk(u8 enabled);

/**
* \fn void ML_EnableCPad(u8 enabled)
* You can type 3 as "enabled", it will return if it's already enabled.
* \brief This function sets if the Classic pad is enabled or not.
* @param enabled Classic pad enabled (1) or not (0)
* @return 1 if enabled, 0 if disabled.
*/
extern bool ML_EnableCPad(u8 enabled);

/**
* \fn void ML_EnableGuitar(u8 enabled)
* You can type 3 as "enabled", it will return if it's already enabled.
* \brief This function sets if the guitar of GH is enabled or not.
* @param enabled Guitar enabled (1) or not (0)
* @return 1 if enabled, 0 if disabled
*/
extern bool ML_EnableGuitar(u8 enabled);

/**
* \fn void ML_RumbleWiimote(s32 wpad, int enabled)
* \brief This function rumbles the Wiimote if enabled.
* @param wpad Wiimote number (All Wiimotes : WPAD_CHAN_ALL)
* @param enabled Enable rumble or not ? (Yes: 1 / No: 0)
*/
extern void ML_RumbleWiimote(s32 wpad, int enabled);

extern void _doSystemReset(void);
extern void _doPowerOff(void);
extern void _doPadPowerOff(s32 chan);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

