#include <MLlib.h>

/**
* \file
* \brief This file contains font functions.
*/

/*				CREDITS
 * This file is mainly the work of Armin Tamzarian who made FreeTypeGX and Metaphrasis.
 * I just ported his library from C++ to C and added some things here and there. :)
 * I adapted it to the MLlib.
 */

static FT_Library ftLibrary;

bool ML_InitFont()
{
	int status = 1;

	if(!getTTF())
	{
		FT_Error error = FT_Init_FreeType(&ftLibrary);
		if(error)
		{
			status--;
		}
	}

	if(status == 1)
	{
		setTTF(TRUE);
	}

	return status;
}

void ML_QuitFont()
{
	if(getTTF())
	{
		FT_Done_FreeType(ftLibrary);
		setTTF(FALSE);
	}
}

void ML_DeleteFont(ML_Font *font)
{
	ftMap *s;

	for(s = font->fontData; s != NULL; s = s->hh.next)
	{
    	if(s->charData.glyphDataTexture) { free(s->charData.glyphDataTexture); s->charData.glyphDataTexture = NULL; }
	}

	if(font->fontData) { HASH_CLEAR(hh, font->fontData); font->fontData = NULL; }

	FT_Done_Face(font->ftFace);
}

bool ML_LoadFontFromFile(ML_Font *font, const char *filename, FT_UInt pointSize)
{
	return _loadFont(font, filename, NULL, 0, pointSize, 1);
}

bool ML_LoadFontFromBuffer(ML_Font *font, const uint8_t *filename, FT_Long bufferSize, FT_UInt pointSize)
{
	return _loadFont(font, NULL, filename, bufferSize, pointSize, 0);
}

bool _loadFont(ML_Font *font, const char *filename, const uint8_t* buffer, FT_Long bufferSize, FT_UInt pointSize, bool fat)
{
	if(!getTTF())
	{
		ML_InitFont();
	}
	font->fontData = NULL;

	font->ftPointSize = pointSize;

	if(!fat) // from buffer
		FT_New_Memory_Face(ftLibrary, (FT_Byte *)buffer, bufferSize, 0, &font->ftFace);
	else // from file
	{
		FILE *fp = fopen(filename, "rb");
		if(fp == NULL) return 0;

		fseek(fp, 0, SEEK_END);  // Obtient la taille du fichier
		long size = ftell(fp);
		rewind(fp);

		char *ftBuffer = (char *)malloc(size);
		if(ftBuffer ==  NULL) return 0;

		fread(ftBuffer, 1, size, fp);
		fclose(fp);

		FT_New_Memory_Face(ftLibrary, (FT_Byte *)ftBuffer, size, 0, &font->ftFace);

		free(ftBuffer);
	}

	FT_Set_Pixel_Sizes(font->ftFace, 0, font->ftPointSize);

	font->ftSlot = font->ftFace->glyph;
	font->ftKerningEnabled = FT_HAS_KERNING(font->ftFace);

	font->alpha = 255;
	font->color = (GXColor){0xff, 0xff, 0xff, 0xff};
	font->angle = 0;
	font->flipX = 0;
	font->flipY = 0;
	font->style = FONT_DEFAULT;

	FreeTypeGX_cacheGlyphDataComplete(font);

	return true;
}

// originally FreeTypeGX::drawText();
u16 ML_DrawText(ML_Font *font, int x, int y, char *text, ...)
{
	/*char *buffer;
	va_list argp;
    va_start(argp, text);
	vasprintf(&buffer, text, argp);
    va_end(argp);
    if(!buffer) SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);

	wchar_t *wd = malloc(sizeof(wchar_t) * (strlen(buffer) + 1));
	FreeTypeGX_charToWideChar(buffer, wd);
	free(buffer);
	free(wd);*/
	
	/*wchar_t *wText = malloc(sizeof(wchar_t) * (strlen(text) + 1));
	FreeTypeGX_charToWideChar(text, wText);*/
	
	/*char buffer[1024];
	va_list argp;
    va_start(argp, text);
	int size = vsprintf(buffer, text, argp);
    va_end(argp);
    
    wchar_t *wText = malloc(sizeof(wchar_t) * (size + 1));
	FreeTypeGX_charToWideChar(buffer, wText);*/	
	
	wchar_t *wText = malloc(sizeof(wchar_t) * (strlen(text) + 1));
	FreeTypeGX_charToWideChar(text, wText);

	uint16_t strLength = wcslen(wText);
	uint16_t x_pos = x, y_pos = y, printed = 0;
	uint16_t x_offset = 0, y_offset = 0;
	uint16_t i = 0;
	GXTexObj glyphTexture;
	FT_Vector pairDelta;

	GXColor colorTemp = font->color;

	if(font->style & 0x000F)
	{
		x_offset = FreeTypeGX_getStyleOffsetWidth(FreeTypeGX_getWidth(font, wText), font->style);
	}
	if(font->style & 0x00F0)
	{
		y_offset = FreeTypeGX_getStyleOffsetHeight(FreeTypeGX_getOffset(font, wText), font->style);
	}

	for (i = 0; i < strLength; i++)
	{
		if(wText[i] == L'%') // is there a special key ?
		{
			if(wText[i-1] != L'\\')
			{
				switch(wText[i+1])
				{
					case L't':
						x_pos += 12*3;	
						i++;
						break;

					case L'n':
						x_pos = x;
						y_pos += FreeTypeGX_getHeight(font, wText);
						i++;
						break;

					case L'c':
						switch(wText[i+2])
						{
							case L'0': // white
								colorTemp = (GXColor){0xff, 0xff, 0xff, 0xff};
								i++;
								break;
							case L'1': // red
								colorTemp = (GXColor){0xff, 0x00, 0x00, 0xff};
								i++;
								break;
							case L'2': // green
								colorTemp = (GXColor){0x00, 0xff, 0x00, 0xff};
								i++;
								break;
							case L'3': // deep-blue
								colorTemp = (GXColor){0x00, 0x00, 0xa0, 0xff};
								i++;
								break;
							case L'4': // purple
								colorTemp = (GXColor){0x80, 0x00, 0x80, 0xff};
								i++;
								break;
							case L'5': // light-blue
								colorTemp = (GXColor){0x00, 0x00, 0xff, 0xff};
								i++;
								break;
							case L'6': // yellow
								colorTemp = (GXColor){0xff, 0xff, 0x00, 0xff};
								i++;
								break;
							case L'7': // grey
								colorTemp = (GXColor){0xc0, 0xc0, 0xc0, 0xff};
								i++;
								break;
							case L'8': // dark-gray
								colorTemp = (GXColor){0x80, 0x80, 0x80, 0xff};
								i++;
								break;
							case L'9': // black
								colorTemp = (GXColor){0x00, 0x00, 0x00, 0xff};
								i++;
								break;
							default:
								colorTemp = font->color;
								break;
						}
						i++;
						break;
					default:
						break;	
				}// switch %c
			} // not going to be drawn
		} // is there a special key ?
		else
		{
			ftgxCharData* glyphData = NULL;

			glyphData = &(_findTexInFtMap(font, wText[i]))->charData;

			if(glyphData != NULL)
			{
				if(font->ftKerningEnabled && i)
				{
					FT_Get_Kerning(font->ftFace, (_findTexInFtMap(font, wText[i-1]))->charData.glyphIndex, glyphData->glyphIndex, FT_KERNING_DEFAULT, &pairDelta);
					x_pos += pairDelta.x >> 6;
				}

				GX_InitTexObj(&glyphTexture, glyphData->glyphDataTexture, glyphData->textureWidth, glyphData->textureHeight, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
				//GX_InitTexObjLOD(&glyphTexture, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
				FreeTypeGX_copyTextureToFramebuffer(&glyphTexture, x_pos - x_offset, y_pos - glyphData->renderOffsetY - y_offset, glyphData->textureWidth, glyphData->textureHeight, colorTemp, font->alpha, 1, 1, font->angle, font->flipX, font->flipY);
				x_pos += glyphData->glyphAdvanceX;
				printed++;

				if(font->style & 0x0F00) {
					FreeTypeGX_drawTextFeature(font, x_pos - glyphData->glyphAdvanceX - x_offset, y_pos, FreeTypeGX_getWidthEx(font, wText, i)+2, FreeTypeGX_getOffset(font, wText), font->style);
				}
			}
		}
	}

	free(wText);

	return printed;
}

bool FreeTypeGX_cacheGlyphData(ML_Font *font, wchar_t charCode)
{
	FT_UInt gIndex;
	uint16_t textureWidth = 0, textureHeight = 0;

	gIndex = FT_Get_Char_Index(font->ftFace, charCode);
	if (!FT_Load_Glyph(font->ftFace, gIndex, FT_LOAD_DEFAULT))
	{
		FT_Render_Glyph(font->ftSlot, FT_RENDER_MODE_NORMAL);

		if(font->ftSlot->format == FT_GLYPH_FORMAT_BITMAP)
		{
			FT_Bitmap *glyphBitmap = &font->ftSlot->bitmap;

			textureWidth = FreeTypeGX_adjustTextureWidth(glyphBitmap->width);
			textureHeight = FreeTypeGX_adjustTextureHeight(glyphBitmap->rows);

			ftgxCharData charData = {
				font->ftSlot->advance.x >> 6,
				gIndex,
				textureWidth,
				textureHeight,
				font->ftSlot->bitmap_top,
				font->ftSlot->bitmap_top,
				textureHeight - font->ftSlot->bitmap_top,
				NULL
			};
			FreeTypeGX_loadGlyphData(glyphBitmap, &charData);

			_addTexToFtMap(font, charCode, charData);

			if(&charData == NULL) return 0;
			else return 1;
		}
	}

	return NULL;
}

uint16_t FreeTypeGX_cacheGlyphDataComplete(ML_Font *font)
{
	uint16_t i = 0;
	FT_UInt gIndex;
	FT_ULong charCode = FT_Get_First_Char(font->ftFace, &gIndex);

	while (gIndex != 0)
	{
		if(FreeTypeGX_cacheGlyphData(font, charCode) != 0) {
			i++;
		}

		charCode = FT_Get_Next_Char(font->ftFace, charCode, &gIndex);
	}

	return i;
}

void FreeTypeGX_loadGlyphData(FT_Bitmap *bmp, ftgxCharData *charData)
{
	uint32_t *glyphData = (uint32_t *)(memalign(32, charData->textureWidth * charData->textureHeight * 4));
	memset(glyphData, 0x00, charData->textureWidth * charData->textureHeight * 4);

	uint16_t imagePosY = 0, imagePosX = 0;
	uint32_t pixel;

	for (imagePosY = 0; imagePosY < bmp->rows; imagePosY++) {
		for (imagePosX = 0; imagePosX < bmp->width; imagePosX++) {
			pixel = (uint32_t)bmp->buffer[imagePosY * bmp->width + imagePosX];
			glyphData[imagePosY * charData->textureWidth + imagePosX] = 0x00000000 | (pixel << 24) | (pixel << 16) | (pixel << 8) | pixel;
		}
	}

	charData->glyphDataTexture = Metaphrasis_convertBufferToRGBA8(glyphData, charData->textureWidth, charData->textureHeight);

	free(glyphData);
}

void FreeTypeGX_drawTextFeature(ML_Font *font, int16_t x, int16_t y, uint16_t width, ftgxDataOffset offsetData, uint16_t format)
{
	uint16_t featureHeight = font->ftPointSize >> 4 > 0 ? font->ftPointSize >> 4 : 1;

	if(format & FONT_UNDERLINE)
	{
		switch(format & 0x00F0)
		{
			case FONT_ALIGN_TOP:
				FreeTypeGX_copyFeatureToFramebuffer(width, featureHeight, x, y + offsetData.max + 1, font->color, font->alpha);
				break;
			case FONT_ALIGN_MIDDLE:
				FreeTypeGX_copyFeatureToFramebuffer(width, featureHeight, x, y + ((offsetData.max - offsetData.min) >> 1) + 1, font->color, font->alpha);
				break;
			case FONT_ALIGN_BOTTOM:
				FreeTypeGX_copyFeatureToFramebuffer(width, featureHeight, x, y - offsetData.min, font->color, font->alpha);
				break;
			default:
				FreeTypeGX_copyFeatureToFramebuffer(width, featureHeight, x, y + 1, font->color, font->alpha);
				break;
		}
	}

	if(format & FONT_STRIKE)
	{
		switch(format & 0x00F0)
		{
			case FONT_ALIGN_TOP:
				FreeTypeGX_copyFeatureToFramebuffer(width, featureHeight, x, y + ((offsetData.max + offsetData.min) >> 1), font->color, font->alpha);
				break;
			case FONT_ALIGN_MIDDLE:
				FreeTypeGX_copyFeatureToFramebuffer(width, featureHeight, x, y, font->color, font->alpha);
				break;
			case FONT_ALIGN_BOTTOM:
				FreeTypeGX_copyFeatureToFramebuffer(width, featureHeight, x, y - ((offsetData.max + offsetData.min) >> 1), font->color, font->alpha);
				break;
			default:
				FreeTypeGX_copyFeatureToFramebuffer(width, featureHeight, x, y - ((offsetData.max - offsetData.min) >> 1), font->color, font->alpha);
				break;
		}
	}
}


uint16_t FreeTypeGX_adjustTextureWidth(uint16_t textureWidth) {
	return textureWidth % 4 == 0 ? textureWidth : 4 + textureWidth - (textureWidth % 4); }

uint16_t FreeTypeGX_adjustTextureHeight(uint16_t textureHeight) {
	return textureHeight % 4 == 0 ? textureHeight : 4 + textureHeight - (textureHeight % 4); }

void FreeTypeGX_charToWideChar(char* strChar, wchar_t *strWChar)
{
	char *tempSrc = strChar;
	wchar_t *tempDest = strWChar;
	while((*tempDest++ = *tempSrc++));
}

uint32_t* Metaphrasis_convertBufferToRGBA8(uint32_t* rgbaBuffer, uint16_t bufferWidth, uint16_t bufferHeight)
{
	uint32_t bufferSize = (bufferWidth * bufferHeight) << 2;
	uint32_t* dataBufferRGBA8 = (uint32_t *)memalign(32, bufferSize);
	memset(dataBufferRGBA8, 0x00, bufferSize);

	uint8_t *src = (uint8_t *)rgbaBuffer;
	uint8_t *dst = (uint8_t *)dataBufferRGBA8;

	uint16_t block = 0, i = 0, c = 0, ar = 0, gb = 0;

	for(block = 0; block < bufferHeight; block += 4) {
		    for(i = 0; i < bufferWidth; i += 4) {
		            for (c = 0; c < 4; c++) {
		                    for (ar = 0; ar < 4; ar++) {
		                            *dst++ = src[(((i + ar) + ((block + c) * bufferWidth)) * 4) + 3];
		                            *dst++ = src[((i + ar) + ((block + c) * bufferWidth)) * 4];
		                    }
		            }
		            for (c = 0; c < 4; c++) {
		                    for (gb = 0; gb < 4; gb++) {
		                            *dst++ = src[(((i + gb) + ((block + c) * bufferWidth)) * 4) + 1];
		                            *dst++ = src[(((i + gb) + ((block + c) * bufferWidth)) * 4) + 2];
		                    }
		            }
		    }
	}
	DCFlushRange(dataBufferRGBA8, bufferSize);

	return dataBufferRGBA8;
}

uint16_t FreeTypeGX_getWidth(ML_Font *font, const wchar_t *text)
{
	uint16_t strLength = wcslen(text);
	uint16_t strWidth = 0;
	FT_Vector pairDelta;
	uint16_t i = 0;

	for (i = 0; i < strLength; i++)
	{
		ftgxCharData* glyphData = NULL;

		glyphData = &(_findTexInFtMap(font, text[i]))->charData;

		if(glyphData != NULL)
		{
			if(font->ftKerningEnabled && (i > 0))
			{
				FT_Get_Kerning(font->ftFace, (_findTexInFtMap(font, text[i-1]))->charData.glyphIndex, glyphData->glyphIndex, FT_KERNING_DEFAULT, &pairDelta);
				strWidth += pairDelta.x >> 6;
			}

			strWidth += glyphData->glyphAdvanceX;
		}
	}

	return strWidth;
}

uint16_t FreeTypeGX_getWidthEx(ML_Font *font, const wchar_t *text, uint16_t i)
{
	uint16_t strWidth = 0;
	FT_Vector pairDelta;

	ftgxCharData* glyphData = NULL;

	glyphData = &(_findTexInFtMap(font, text[i]))->charData;

	if(glyphData != NULL)
	{
		if(font->ftKerningEnabled && (i > 0))
		{
			FT_Get_Kerning(font->ftFace, (_findTexInFtMap(font, text[i-1]))->charData.glyphIndex, glyphData->glyphIndex, FT_KERNING_DEFAULT, &pairDelta);
			strWidth += pairDelta.x >> 6;
		}

		strWidth += glyphData->glyphAdvanceX;
	}

	return strWidth;
}

uint16_t FreeTypeGX_getHeight(ML_Font *font, const wchar_t *text)
{
    ftgxDataOffset offset = FreeTypeGX_getOffset(font, text);

    return offset.max + offset.min;
}

ftgxDataOffset FreeTypeGX_getOffset(ML_Font *font, const wchar_t *text)
{
	uint16_t strLength = wcslen(text);
	uint16_t strMax = 0, strMin = 0;
	uint16_t i = 0;

	for(i = 0; i < strLength; i++)
	{
		ftgxCharData* glyphData = NULL;

		glyphData = &(_findTexInFtMap(font, text[i]))->charData;

		if(glyphData != NULL)
		{
			strMax = glyphData->renderOffsetMax > strMax ? glyphData->renderOffsetMax : strMax;
			strMin = glyphData->renderOffsetMin > strMin ? glyphData->renderOffsetMin : strMin;
		}
	}

	return (ftgxDataOffset){strMax, strMin};
}

uint16_t FreeTypeGX_getStyleOffsetWidth(uint16_t width, uint16_t format)
{
	if(format & FONT_JUSTIFY_LEFT)
	{
		return 0;
	}
	else if(format & FONT_JUSTIFY_CENTER)
	{
		return width >> 1;
	}
	else if(format & FONT_JUSTIFY_RIGHT)
	{
		return width;
	}

	return 0;
}

uint16_t FreeTypeGX_getStyleOffsetHeight(ftgxDataOffset offset, uint16_t format)
{
	if(format & FONT_ALIGN_TOP)
	{
		return -offset.max;
	}
	else if(format & FONT_ALIGN_MIDDLE)
	{
		return -(offset.max - offset.min) >> 1;
	}
	else if(format & FONT_ALIGN_BOTTOM)
	{
		return offset.min;
	}

	return 0;
}

ftMap *_findTexInFtMap(ML_Font *font, wchar_t charCode)
{
	ftMap *s;
	HASH_FIND(hh, font->fontData, &charCode, sizeof(wchar_t), s);  /* s: output pointer */
	return s;
}


void _addTexToFtMap(ML_Font *font, wchar_t charCode, ftgxCharData charData)
{
    ftMap *s;
    s = malloc(sizeof(ftMap));
    s->id = charCode;
    s->charData = charData;
    HASH_ADD(hh, font->fontData, id, sizeof(wchar_t), s);
}

