#ifndef DEF_ML_FONT_H
#define DEF_ML_FONT_H

#include "uthash.h"

/**
* \file
* \brief This file contains font headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

typedef struct ftgxCharData_ {
	uint16_t glyphAdvanceX;	/**< Character glyph X coordinate advance in pixels. */
	uint16_t glyphIndex;	/**< Charachter glyph index in the font face. */

	uint16_t textureWidth;	/**< Texture width in pixels/bytes. */
	uint16_t textureHeight;	/**< Texture glyph height in pixels/bytes. */

	uint16_t renderOffsetY;	/**< Texture Y axis bearing offset. */
	uint16_t renderOffsetMax;	/**< Texture Y axis bearing maximum value. */
	uint16_t renderOffsetMin;	/**< Texture Y axis bearing minimum value. */

	uint32_t* glyphDataTexture;	/**< Glyph texture bitmap data buffer. */
} ftgxCharData;

typedef struct ftgxDataOffset_ {
	uint16_t max;   /**< Maximum data offset. */
	uint16_t min;   /**< Minimum data offset. */
} ftgxDataOffset;

typedef struct ftMap_ {
    wchar_t id;					/* key */
    ftgxCharData charData;		/* value */
    UT_hash_handle hh;			/* makes this structure hashable */
} ftMap;

// Font structure
typedef struct {
	ftMap *fontData;
	GXColor color;
	long x, 
	y;
	bool visible;
	u8 rotated;
	u8 alpha;
	float angle;
	bool flipX,
	flipY;
	u16 style;
	FT_Face ftFace;
	FT_GlyphSlot ftSlot;
	FT_UInt ftPointSize;
	bool ftKerningEnabled;
	bool cacheAll;
} ML_Font;

#define _TEXT(t) L ## t

#define FONT_NULL					0x0000

#define FONT_JUSTIFY_LEFT			0x0001
#define FONT_JUSTIFY_CENTER			0x0002
#define FONT_JUSTIFY_RIGHT			0x0004

#define FONT_ALIGN_TOP				0x0010
#define FONT_ALIGN_MIDDLE			0x0020
#define FONT_ALIGN_BOTTOM			0x0040

#define FONT_UNDERLINE				0x0100
#define FONT_STRIKE					0x0200

#define FONT_DEFAULT				FONT_JUSTIFY_LEFT | FONT_ALIGN_TOP

/**
* \fn void ML_InitFont()
* \brief This function initializes the font system. You do no hat to call it because it does it automatically.
*/
extern bool ML_InitFont();

/**
* \fn void ML_QuitFont()
* \brief This function quits the font system. You do no hat to call it because it does it automatically.
*/
extern void ML_QuitFont();

/**
* \fn void ML_DeleteFont(ML_Font *font)
* \brief This function frees and deletes the font memory. You have to call it.
* @param font Font structure
*/
extern void ML_DeleteFont(ML_Font *font);

/**
* \fn bool ML_LoadFontFromBuffer(ML_Font *font, const uint8_t *filename, FT_Long bufferSize, FT_UInt pointSize)
* \brief This function loads a font from a buffer.
* @param font Font structure
* @param filename Buffer variable (normally nameOfYourFont_ttf)
* @param bufferSize Size of the buffer (normally nameOfYourFont_ttf_size)
* @param pointSize Size of the police (in pt)
* @return 1 if success, 0 if not
*/
extern bool ML_LoadFontFromBuffer(ML_Font *font, const uint8_t *filename, FT_Long bufferSize, FT_UInt pointSize);

/**
* \fn bool ML_LoadFontFromFile(ML_Font *font, const char *filename, FT_UInt pointSize)
* \brief This function loads a font from a file.
* @param font Font structure
* @param filename Path to the .ttf ( "/apps/yourapp/data/yourfont.ttf" generally)
* @param pointSize Size of the police (in pt)
* @return 1 if success, 0 if not
*/
extern bool ML_LoadFontFromFile(ML_Font *font, const char *filename, FT_UInt pointSize);

/**
* \fn u16 ML_DrawText(ML_Font *font, int x, int y, char *text, ...)
* \brief This function draws some text.
* @param font Font structure
* @param x X position
* @param y Y position
* @param text Text
* @return Number of caracters printed
*/
extern u16 ML_DrawText(ML_Font *font, int x, int y, char *text, ...);

inline bool _loadFont(ML_Font *font, const char *filename, const uint8_t* buffer, FT_Long bufferSize, FT_UInt pointSize, bool fat);

bool FreeTypeGX_cacheGlyphData(ML_Font *font, wchar_t charCode);
uint16_t FreeTypeGX_cacheGlyphDataComplete(ML_Font *font);
void FreeTypeGX_loadGlyphData(FT_Bitmap *bmp, ftgxCharData *charData);
void FreeTypeGX_drawTextFeature(ML_Font *font, int16_t x, int16_t y, uint16_t width, ftgxDataOffset offsetData, uint16_t format);
uint16_t FreeTypeGX_adjustTextureWidth(uint16_t textureWidth);
uint16_t FreeTypeGX_adjustTextureHeight(uint16_t textureHeight);
void FreeTypeGX_charToWideChar(char* strChar, wchar_t *strWChar);

uint32_t* Metaphrasis_convertBufferToRGBA8(uint32_t* rgbaBuffer, uint16_t bufferWidth, uint16_t bufferHeight);
uint16_t FreeTypeGX_getWidth(ML_Font *font, const wchar_t *text);
uint16_t FreeTypeGX_getWidthEx(ML_Font *font, const wchar_t *text, uint16_t i);
uint16_t FreeTypeGX_getHeight(ML_Font *font, const wchar_t *text);
ftgxDataOffset FreeTypeGX_getOffset(ML_Font *font, const wchar_t *text);
uint16_t FreeTypeGX_getStyleOffsetWidth(uint16_t width, uint16_t format);
uint16_t FreeTypeGX_getStyleOffsetHeight(ftgxDataOffset offset, uint16_t format);

ftMap *_findTexInFtMap(ML_Font *font, wchar_t charCode);
void _addTexToFtMap(ML_Font *font, wchar_t charCode, ftgxCharData charData);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

