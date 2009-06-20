#ifndef DEF_ML_VIDEO_H
#define DEF_ML_VIDEO_H

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

#define DEFAULT_FIFO_SIZE	(1024*1024)

/**
* \struct ML_Image
* \brief This is the structure for images.
*/
typedef struct { 
	GXTexObj texObj; /*!< Texture */
	u8 *data; /*!< Data (ARGB) */
	u16 width, /*!< Width */
	height; /*!< Height */
} ML_Image;

/**
* \struct ML_Sprite
* \brief This is the structure for sprites.
*/
typedef struct {
	ML_Image *image;  /*!< Pointer to the image associated with */
	long i;  /*!< Used for the animation */
	s16 x,  /*!< X position */
	y; /*!< Y position */
	s8 dx, /*!< X velocity */
	dy; /*!< Y velocity */
	bool visible; /*!< Member for setting the sprite visible or not */
	u16 width; /*!< Width */
	u16 height; /*!< Height */
	u8 rotated; /*!< Is rotation enabled */
	u8 alpha; /*!< Transparency (0-255) */
	float scaleX, /*!< X scaling */
	scaleY; /*!< Y scaling */
	float angle; /*!< Angle in degrees */
	bool tiled; /*!< Is tiling enabled */
	bool animated; /*!< Is animation enabled */
	u8 waitForXRefreshBetweenFrames;  /*!< Number of cycles between each frame changes */
	u8 anime_from, /*!< Animation start at... */
	anime_to; /*!< Animation end at... */
	u16 currentFrame; /*!< Current frame */
	u16 nbTiles; /*!< Number of tiles */
	u16 tileWidth, /*!< Width of one tile */
	tileHeight; /*!< Height of one tile */
} ML_Sprite;

/**
* \struct ML_Background
* \brief This is the structure for backgrounds.
*/
typedef struct {
	ML_Image *image;  /*!< Pointer to the image associated with */
	u16 width, /*!< Width */
	height; /*!< Height */
	float scaleX, /*!< X scaling */
	scaleY; /*!< Y scaling */
	u8 rotated; /*!< Is rotation enabled */
	u8 alpha; /*!< Transparency (0-255) */
	float angle; /*!< Angle in degrees */
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

extern bool ML_LoadBackgroundFromBuffer(ML_Image *image, ML_Background *background, const u8 *filename, int x, int y);
extern bool ML_LoadBackgroundFromFile(ML_Image *image, ML_Background *background, char *filename, int x, int y);

inline bool _loadImage(ML_Image *image, ML_Sprite *sprite, ML_Background *background, char *filename, const u8 *buffer, int x, int y, bool fat);

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
* \fn void ML_DrawRect(int x, int y, u16 width, u16 height, u32 rgba)
* \brief This function shows a rectangle with the color you want.
* @param x X position
* @param y Y position
* @param width Width
* @param height Height
* @param rgba RGBA Color (HTML Color) like 0xFF0016FF (0xRED+GREEN+BLUE+ALPHA)
*/
extern void ML_DrawRect(int x, int y, u16 width, u16 height, u32 rgba);

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
* \fn void ML_SetBackgroundColor(GXColor color)
* \brief This function clears the background with the color you want.
* @param color New background color
*/
extern void ML_SetBackgroundColor(GXColor color);

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

