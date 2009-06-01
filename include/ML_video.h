#ifndef DEF_ML_VIDEO_H
#define DEF_ML_VIDEO_H

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

#define DEFAULT_FIFO_SIZE	(1024*1024)
#define NUM_SPRITES 1024

// Sprite structure
typedef struct {
	GXTexObj texObj;
	int i;
	int x,y;			// screen co-ordinates
	int dx, dy;			// velocity
	bool visible;
	u32 width;
	u32 height;
	u8 *data;
	u16 clone_nb;
	u8 rotated;
	u8 alpha;
	float scaleX, scaleY;
	float angle;
	bool tiled;
	bool animated;
	u8 waitForXRefreshBetweenFrames;
	u16 currentFrame;
	u16 nbTiles;
	float tileWidth, tileHeight;
} Sprite;

// BMP structure
typedef struct
{
  u16 bfMagic;			/*** Always BM ***/
  u32 bfSize;			/*** Size of file ***/
  u32 bfReserved;		/*** Always 0 ***/
  u32 bfOffset;			/*** Offset to pixel data ***/
  u32 biImageHdrSize;		/*** Size of Image info header ***/
  u32 biWidth;			/*** Width of bitmap ***/
  u32 biHeight;			/*** Height of bitmap ***/
  u16 biPlanes;			/*** Number of planes ***/
  u16 biBitsPerPixel;		/*** Bits per pixel ***/
  u32 biCompression;		/*** Is this compressed? ***/
} __attribute__ ((__packed__)) WINBITMAP;

Sprite sprite[NUM_SPRITES];

GXRModeObj *screenMode;
int _screenWidth, _screenHeight;

/**
* \file
* \brief This file contains video headers.
*/


/*****************************************************
******************       GX         ******************
******************************************************/


/**
* \fn bool ML_LoadSpriteFromBuffer(u16 nb, const u8 *filename, int x, int y)
* \brief This function loads a PNG image in memory from the buffer (see tutorial/wiki).
* @param nb The sprite number you will use after.
* @param filename The path to the PNG file. (eg. sprite_png)
* @param x X position
* @param y Y position
* @return 0 if it failed, 1 else.
*/
extern bool ML_LoadSpriteFromBuffer(u16 nb, const u8 *filename, int x, int y);

/**
* \fn bool ML_LoadSpriteFromFile(u16 nb, char *filename, int x, int y)
* \brief This function loads a PNG image in memory from a file.
* @param nb The sprite number you will use after.
* @param filename The path to the PNG file. (eg. "sprite.png")
* @param x X position
* @param y Y position
* @return 0 if it failed, 1 else.
*/
extern bool ML_LoadSpriteFromFile(u16 nb, char *filename, int x, int y);

/**
* \fn void ML_DrawSprite(u16 nb)
* \brief This function shows the sprite which is already loaded, of course.
* @param nb Sprite number
*/
extern void ML_DrawSprite(u16 nb);

/**
* \fn void ML_DrawSpriteXY(u16 nb, int x, int y)
* \brief This function shows the sprite which is already loaded. At X and Y positions.
* @param nb Sprite number
* @param x X position
* @param y Y position
*/
extern void ML_DrawSpriteXY(u16 nb, int x, int y);

/**
* \fn void ML_DrawSpriteFull(u16 nb, int x, int y, float angle, float scaleX, float scaleY, u8 alpha)
* \brief This function shows the sprite which is already loaded. It can be faster when you have many things to do on the sprites in a time.
* @param nb Sprite number
* @param x X position
* @param y Y position
* @param angle Angle in degrees ( -180 -> +180 )
* @param scaleX Horizontal scale
* @param scaleY Vertical scale 
* @param alpha Transparency ( 0 -> 255 )
*/
extern void ML_DrawSpriteFull(u16 nb, int x, int y, float angle, float scaleX, float scaleY, u8 alpha);

/**
* \fn void ML_InitTile(u16 nb, u16 width, u16 height)
* \brief This function inits the tile system of the sprite.
* @param nb Sprite number
* @param width Width of the tile
* @param height Height of the tile
*/
extern void ML_InitTile(u16 nb, u16 width, u16 height);

/**
* \fn void ML_DrawTile(u16 nb, int x, int y, u16 frame)
* \brief This function draws a tile.
* @param nb Sprite number
* @param x X position
* @param y Y position
* @param frame Tile n°<frame>
*/
extern void ML_DrawTile(u16 nb, int x, int y, u16 frame);

/**
* \fn void ML_Brightness(u8 alpha)
* \brief This function changes the opacity of the entire screen.
* In fact, it draws a black rectangle which will be opacified.
* @param alpha Alpha of the screen.
*/
extern void ML_Brightness(u8 alpha);

/**
* \fn bool ML_FadeOut()
* \brief This function fades out.
* @return 1 if it's finished, 0 if not.
*/
extern bool ML_FadeOut();

/**
* \fn bool ML_FadeIn()
* \brief This function fades in (after a fade-out).
* @return 1 if it's finished, 0 if not.
*/
extern bool ML_FadeIn();

/**
* \fn void ML_Text(int nb, int x, int y, const char *text, ...)
* \brief This function draws some graphic text.
* @param nb Sprite number of the font (the font is in fact an image, a sprite)
* @param x X position
* @param y Y position
* @param text Text which can have arguments
*/
extern void ML_Text(int nb, int x, int y, const char *text, ...);

/**
* \fn void ML_Text(int nb, int x, int y, const char *text, ...)
* \brief This function draws some graphic text, but with no arguments and other funny thing : just faster !
* @param nb Sprite number of the font (the font is in fact an image, a sprite)
* @param x X position
* @param y Y position
* @param text Text
*/
extern void ML_SimpleText(int nb, int x, int y, const char *text);

/**
* \fn void ML_SetBackgroundColor(GXColor color)
* \brief This function clears the background with the color you want.
* @param color New background color
*/
extern void ML_SetBackgroundColor(GXColor color);

/**
* \fn void ML_SetPixelColor(int nb, int x, int y, u32 color)
* \brief This function sets the color value of a pixel on a sprite.
* @param nb Sprite number
* @param x X position of the pixel
* @param y Y position of the pixel
* @param color New color of the pixel
*/
extern void ML_SetPixelColor(int nb, int x, int y, u32 color);

/**
* \fn u32 ML_GetPixelColor(int nb, int x, int y)
* \brief This function returns the color value of the pixel on a sprite.
* @param nb Sprite number
* @param x X position of the pixel
* @param y Y position of the pixel
* @return Color of the pixel
*/
extern u32 ML_GetPixelColor(int nb, int x, int y);

/**
* \fn void ML_SplashScreen()
* \brief This function shows the official Splash Screen made by Zeblackos. :)
*/
extern void ML_SplashScreen();

extern void ML_GX_Init(); // Init GX system
extern void ML_GX_Refresh(); // Refresh GX system


/*****************************************************
******************       FB         ******************
******************************************************/

/**
* \fn void ML_CleanVideoBG()
* \brief This function cleans the BG in black.
*/
extern void ML_CleanVideoBG();

/**
* \fn void ML_CleanVideoBGColor(int color)
* \brief This function cleans the BG with a color.
* @param color The color you want.
*/
extern void ML_CleanVideoBGColor(int color);


/*****************************************************
******************      OTHERS      ******************
******************************************************/


/**
* \fn void ML_Refresh()
* \brief This function refresh the MLlib (Get PADS status, and refresh video)
*/
extern void ML_Refresh();

/**
* \fn void ML_RefreshConsoleMode()
* \brief CONSOLE MODE - This function refresh the MLlib (Get PADS status, and refresh video)
*/
extern void ML_RefreshConsoleMode();

extern void ML_InitVideo(); // Init video
extern void ML_VSync();  // Refresh video
extern void ML_GX_FreeMemory();

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

