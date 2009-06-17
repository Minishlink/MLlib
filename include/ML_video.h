#ifndef DEF_ML_VIDEO_H
#define DEF_ML_VIDEO_H

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

#define DEFAULT_FIFO_SIZE	(1024*1024)

// Image structure
typedef struct { 
	GXTexObj texObj;
	u8 *data;
	u16 width, height;
} ML_Image;

// Sprite structure
typedef struct {
	ML_Image *image;
	long i;
	s16 x, y;
	s8 dx, dy;
	bool visible;
	u16 width;
	u16 height;
	u8 rotated;
	u8 alpha;
	float scaleX, scaleY;
	float angle;
	bool tiled;
	bool animated;
	u8 waitForXRefreshBetweenFrames;
	u8 anime_from, anime_to;
	u16 currentFrame;
	u16 nbTiles;
	u16 tileWidth, tileHeight;
} ML_Sprite;

// Background structure
typedef struct { 
	s16 x, y;
	u16 width, height;
	float scaleX, scaleY;
	bool visible;
	u8 rotated;
	u8 alpha;
	float angle;
} ML_Background;

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
* \fn bool ML_LoadSpriteFromBuffer(ML_Image *image, ML_Sprite *sprite, const u8 *filename, int x, int y)
* \brief This function loads a PNG image in memory from the buffer (see tutorial/wiki).
* @param image The image you're loading
* @param sprite The sprite structure you will use after
* @param filename The path to the PNG file. (eg. sprite_png)
* @param x X position
* @param y Y position
* @return 0 if it failed, 1 else.
*/
extern bool ML_LoadSpriteFromBuffer(ML_Image *image, ML_Sprite *sprite, const u8 *filename, int x, int y);

extern void ML_DrawSpriteXY2(ML_Sprite *sprite, int x, int y);

/**
* \fn bool ML_LoadSpriteFromFile(ML_Image *image, ML_Sprite *sprite, char *filename, int x, int y)
* \brief This function loads a PNG image in memory from a file.
* @param image The image you're loading
* @param sprite The sprite structure you will use after
* @param filename The path to the PNG file. (eg. "sprite.png")
* @param x X position
* @param y Y position
* @return 0 if it failed, 1 else.
*/
extern bool ML_LoadSpriteFromFile(ML_Image *image, ML_Sprite *sprite, char *filename, int x, int y);

extern bool ML_LoadBackgroundFromBuffer(ML_Image *image, ML_Sprite *sprite, ML_Background *background, const u8 *filename, int x, int y);
extern bool ML_LoadBackgroundFromFile(ML_Image *image, ML_Sprite *sprite, ML_Background *background, char *filename, int x, int y);

inline bool _loadImage(ML_Image *image, ML_Sprite *sprite, ML_Background *background, char *filename, const u8 *buffer, int x, int y, bool fat);

/**
* \fn void ML_DrawSprite(ML_Sprite *sprite)
* \brief This function shows the sprite which is already loaded, of course.
* @param sprite Sprite
*/
extern void ML_DrawSprite(ML_Sprite *sprite);

/**
* \fn void ML_DrawSpriteXY(ML_Sprite *sprite, int x, int y)
* \brief This function shows the sprite which is already loaded. At X and Y positions.
* @param sprite Sprite
* @param x X position
* @param y Y position
*/
extern void ML_DrawSpriteXY(ML_Sprite *sprite, int x, int y);

/**
* \fn void ML_DrawSpriteFull(ML_Sprite *sprite, int x, int y, float angle, float scaleX, float scaleY, u8 alpha)
* \brief This function shows the sprite which is already loaded. It can be faster when you have many things to do on the sprites in a time.
* @param sprite Sprite
* @param x X position
* @param y Y position
* @param angle Angle in degrees ( -180 -> +180 )
* @param scaleX Horizontal scale
* @param scaleY Vertical scale 
* @param alpha Transparency ( 0 -> 255 )
*/
extern void ML_DrawSpriteFull(ML_Sprite *sprite, int x, int y, float angle, float scaleX, float scaleY, u8 alpha);

/**
* \fn void ML_DrawTexture(GXTexObj *texObj, int x, int y, u16 width, u16 height, float angle, float scaleX, float scaleY, u8 alpha)
* \brief This function shows the sprite which is already loaded. It can be faster when you have many things to do on the sprites in a time.
* @param texObj GX Texture (it's sprite.texObj)
* @param x X position
* @param y Y position
* @param width Width
* @param height Height
* @param angle Angle in degrees ( -180 -> +180 )
* @param scaleX Horizontal scale
* @param scaleY Vertical scale 
* @param alpha Transparency ( 0 -> 255 )
*/
extern void ML_DrawTexture(GXTexObj *texObj, int x, int y, u16 width, u16 height, float angle, float scaleX, float scaleY, u8 alpha);

/**
* \fn void ML_InitTile(ML_Sprite *sprite, u16 width, u16 height)
* \brief This function inits the tile system of the sprite.
* @param sprite Sprite
* @param width Width of the tile
* @param height Height of the tile
*/
extern void ML_InitTile(ML_Sprite *sprite, u16 width, u16 height);

/**
* \fn void ML_DrawTile(ML_Sprite *sprite, int x, int y, u16 frame)
* \brief This function draws a tile.
* @param sprite Sprite
* @param x X position
* @param y Y position
* @param frame Tile n°<frame>
*/
extern void ML_DrawTile(ML_Sprite *sprite, int x, int y, u16 frame);

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
* \fn void ML_DrawSpriteText(ML_Sprite *sprite, int x, int y, const char *text, ...)
* \brief This function draws some graphic text.
* @param sprite Sprite for extra-features like transparency, etc...
* @param x X position
* @param y Y position
* @param text Text which can have arguments
*/
extern void ML_DrawSpriteText(ML_Sprite *sprite, int x, int y, const char *text, ...);

extern void ML_DrawSpriteTextBox(ML_Sprite *sprite, int x, int y, int x2, int y2, const char *text, ...);

/**
* \fn void ML_DrawSpriteSimpleText(ML_Sprite *sprite, int x, int y, const char *text, ...)
* \brief This function draws some graphic text, but with no arguments and other funny thing : just faster !
* @param image Image of the font
* @param sprite Sprite for extra-features like transparency, etc...
* @param x X position
* @param y Y position
* @param text Text
*/
extern void ML_DrawSpriteSimpleText(ML_Sprite *sprite, int x, int y, const char *text);

/**
* \fn void ML_SetBackgroundColor(GXColor color)
* \brief This function clears the background with the color you want.
* @param color New background color
*/
extern void ML_SetBackgroundColor(GXColor color);

/**
* \fn void ML_SetPixelColor(ML_Image *image, int x, int y, u32 color)
* \brief This function sets the color value of a pixel on a sprite.
* @param image Image
* @param x X position of the pixel
* @param y Y position of the pixel
* @param color New color of the pixel
*/
extern void ML_SetPixelColor(ML_Image *image,  int x, int y, u32 color);

/**
* \fn u32 ML_GetPixelColor(ML_Image *image, int x, int y)
* \brief This function returns the color value of the pixel on a sprite.
* @param image Image
* @param x X position of the pixel
* @param y Y position of the pixel
* @return Color of the pixel
*/
extern u32 ML_GetPixelColor(ML_Image *image, int x, int y);

/**
* \fn void ML_SplashScreen()
* \brief This function shows the official Splash Screen made by Zeblackos. :)
*/
extern void ML_SplashScreen();

extern bool ML_Screenshot(const char *filename);

void _drawImage(GXTexObj *texObj, int x, int y, u16 _width, u16 _height, float scaleX, float scaleY, float angle, u8 alpha, bool tiled, u16 frame, u16 tileWidth, u16 tileHeight);
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
* \brief This function refresh the MLlib (Get pads status, and refresh video)
*/
extern void ML_Refresh();

/**
* \fn void ML_RefreshConsoleMode()
* \brief CONSOLE MODE - This function refresh the MLlib (Get pads status, and refresh video)
*/
extern void ML_RefreshConsoleMode();

extern void ML_InitVideo(); // Init video
extern void ML_VSync();  // Refresh video
extern void ML_GX_FreeMemory();

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

