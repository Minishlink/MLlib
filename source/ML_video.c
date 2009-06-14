#include <MLlib.h>
#include "MLlib_SplashScreen_png.h"

// FB
static void *xfb[2] = { NULL, NULL };
static u32 whichfb = 0;

// GX
static void *gp_fifo = NULL;
static Mtx GXmodelView2D;

// Others
static u8 _alphaFade;

/**
* \file
* \brief This file contains video functions.
*/


/** ****************************************************
************************ GX ****************************
**************************************************** **/

bool ML_LoadSpriteFromBuffer(ML_Image *image, ML_Sprite *sprite, const u8 *filename, int x, int y)
{	
	return _loadImage(image, sprite, NULL, NULL, filename, x, y, 0);
}

bool ML_LoadSpriteFromFile(ML_Image *image, ML_Sprite *sprite, char *filename, int x, int y)
{
	return _loadImage(image, sprite, NULL, filename, NULL, x, y, 1);
}

bool ML_LoadBackgroundFromBuffer(ML_Image *image, ML_Sprite *sprite, ML_Background *background, const u8 *filename, int x, int y)
{	
	return _loadImage(image, NULL, background, NULL, filename, x, y, 0);
}

bool ML_LoadBackgroundFromFile(ML_Image *image, ML_Sprite *sprite, ML_Background *background, char *filename, int x, int y)
{
	return _loadImage(image, NULL, background, filename, NULL, x, y, 1);
}

bool _loadImage(ML_Image *image, ML_Sprite *sprite, ML_Background *background, char *filename, const u8 *buffer, int x, int y, bool fat)
{
	_initImage(image);
	
	if(sprite)
		_initSprite(sprite);
	else if(background) _initBackground(background);

	bool ok = 0;
	
	if(fat)
	{
		if(!fatInitDefault()) return 0;
		
		chdir("/");
		ok = read_png_gx_file(filename, image);
	}
	else ok = read_png_gx_file_buffer(buffer, image);
	
	if(!ok) return 0;
	
	if(sprite)
	{
		sprite->width = image->width;
		sprite->height = image->height;
		sprite->x = x;
		sprite->y = y;
	}
	else if(background)
	{
		background->width = image->width;
		background->height = image->height;
		background->x = x;
		background->y = y;
	}
	
	GX_InitTexObj(&image->texObj, image->data, image->width, image->height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);	
	GX_InitTexObjLOD(&image->texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
	
	return 1;
}

void _initImage(ML_Image *image)
{	
	image->width = 0;
	image->height = 0;
	image->data = NULL;
}

void _initSprite(ML_Sprite *sprite)
{	
	*sprite = (ML_Sprite){0};
	sprite->visible = TRUE;
	sprite->dx = 1;
	sprite->dy = 1;
	sprite->scaleX = 1;
	sprite->scaleY = 1;
	sprite->alpha = 255;
}

void _initBackground(ML_Background *background)
{	
	*background = (ML_Background){0};
	background->visible = TRUE;
	background->scaleX = 1;
	background->scaleY = 1;
	background->alpha = 255;
}

void ML_DeleteImage(ML_Image *image)
{
	if(image->data)
	{
		free(image->data);
		image->data = NULL;
	}
}

void ML_DrawSprite(ML_Image *image, ML_Sprite *sprite)
{
	ML_DrawSpriteXY(image, sprite, sprite->x, sprite->y);
}

void ML_DrawSpriteXY(ML_Image *image, ML_Sprite *sprite, int x, int y)
{
	sprite->x = x; sprite->y = y;
	
	if(!ML_IsSpriteVisible(sprite)) return;
	
	if(!sprite->animated)
	{
		if(!sprite->tiled)
			_drawImage(&image->texObj, sprite->x, sprite->y, sprite->width, sprite->height, sprite->scaleX, sprite->scaleY, sprite->angle, sprite->alpha, 0, 0, 0, 0);
		else
			ML_DrawTile(image, sprite, sprite->x, sprite->y, sprite->currentFrame);
	}
	else
	{
		if(sprite->i == sprite->waitForXRefreshBetweenFrames)
		{
			sprite->i = 0;
			if(sprite->currentFrame == sprite->anime_to-1) sprite->currentFrame = sprite->anime_from;
			else sprite->currentFrame++;
		} else sprite->i++;
		
		ML_DrawTile(image, sprite, sprite->x, sprite->y, sprite->currentFrame);
	}
}

void ML_DrawSpriteFull(ML_Image *image, ML_Sprite *sprite, int x, int y, float angle, float scaleX, float scaleY, u8 alpha)
{
	sprite->x = x;
	sprite->y = y;
	sprite->angle = angle;
	sprite->scaleX = scaleX;
	sprite->scaleY = scaleY;
	sprite->alpha = alpha;
	
	ML_DrawSpriteXY(image, sprite, sprite->x, sprite->y);
}

void ML_DrawTexture(GXTexObj *texObj, int x, int y, u16 width, u16 height, float angle, float scaleX, float scaleY, u8 alpha)
{
	_drawImage(texObj, x, y, width, height, scaleX, scaleY, angle, alpha, 0, 0, 0, 0);
}

void ML_InitTile(ML_Sprite *sprite, u16 width, u16 height)
{
	sprite->tiled = TRUE;
	sprite->tileWidth = width;
	sprite->tileHeight = height;
	sprite->nbTiles = (sprite->width/sprite->tileWidth) * (sprite->height/sprite->tileHeight);
}

void ML_DrawTile(ML_Image *image, ML_Sprite *sprite, int x, int y, u16 frame)
{
	if(!ML_IsSpriteVisible(sprite)) return;
	
	sprite->x = x; sprite->y = y;

	if(sprite->tiled)
		 _drawImage(&image->texObj, sprite->x, sprite->y, sprite->width, sprite->height, sprite->scaleX, sprite->scaleY, sprite->angle, sprite->alpha, 1, frame, sprite->tileWidth, sprite->tileHeight);
}

void ML_DrawRect(int x, int y, u16 width, u16 height, u32 rgba)
{
	int x2 = x + width;
    int y2 = y + height;
    Vector v[] = {{x,y,0.0f}, {x2,y,0.0f}, {x2,y2,0.0f}, {x,y2,0.0f}, {x,y,0.0f}};
    
    int i;
    GXColor color = RGB_u32_u8(rgba);
    
	GX_Begin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);
    for(i = 0; i < 4; i++) 
    {
        GX_Position3f32(v[i].x, v[i].y,  v[i].z);
        GX_Color4u8(color.r, color.g, color.b, color.a);
    }
    GX_End();
}

void ML_Brightness(u8 alpha)
{ 
    Vector v[] = {{0,0,0.0f}, {screenMode->fbWidth,0,0.0f}, {screenMode->fbWidth,screenMode->xfbHeight,0.0f}, {0,screenMode->xfbHeight,0.0f}, {0,0,0.0f}};    
    int i;

    GX_Begin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);
    for(i = 0; i < 4; i++) 
    {
        GX_Position3f32(v[i].x, v[i].y,  v[i].z);
        GX_Color4u8(0x00, 0x00, 0x00, alpha);
    }
    GX_End();
}

bool ML_FadeIn()
{
	if(_alphaFade > 240) _alphaFade -= 5;
	else if(_alphaFade <= 240 && _alphaFade > 0) _alphaFade-=4;
			
	ML_Brightness(_alphaFade);
	
	if(_alphaFade == 0) return 1;
	else return 0;
}

bool ML_FadeOut()
{
	if(_alphaFade < 240) _alphaFade += 4;
	else if(_alphaFade >= 240 && _alphaFade < 255) _alphaFade+=5;
			
	ML_Brightness(_alphaFade);
	
	if(_alphaFade == 255) return 1;
	else return 0;
}

void ML_DrawImageText(ML_Image *image, ML_Sprite *sprite, int x, int y, const char *text, ...)
{ 
    int i = 0, size = 0, j = 0;
    char buffer[1024];
	u8 c = 0;
	
    va_list argp;
    va_start(argp, text);
    size = vsprintf(buffer, text, argp);
    va_end(argp);
	
    for(i=0; i < size; i++) 
    {
        c = buffer[i]-32;
        if(buffer[i] == '\n' || x+j*sprite->tileWidth*sprite->scaleX >= screenMode->fbWidth) { y += sprite->tileHeight*sprite->scaleY; j = -1; }
        else ML_DrawTile(image, sprite, x+j*sprite->tileWidth*sprite->scaleX, y, c); j++; 
    }
}

void ML_DrawImageTextBox(ML_Image *image, ML_Sprite *sprite, int x, int y, int x2, int y2, const char *text, ...)
{
	int i = 0, size = 0, j = 0;
    char buffer[1024];
	u8 c = 0;
	
    va_list argp;
    va_start(argp, text);
    size = vsprintf(buffer, text, argp);
    va_end(argp);
	
    for(i=0; i < size; i++) 
    {
        c = buffer[i]-32;
        if(y > y2) return;
        if(buffer[i] == '\n' || x+j*sprite->tileWidth*sprite->scaleX >= x2) { y += sprite->tileHeight*sprite->scaleY; j = -1; }
        else ML_DrawTile(image, sprite, x+j*sprite->tileWidth*sprite->scaleX, y, c); j++; 
    }
}

void ML_DrawImageSimpleText(ML_Image *image, ML_Sprite *sprite, int x, int y, const char *text)
{
	int i = 0, size = strlen(text);
	u8 c = 0;
	
    for(i=0; i < size; i++)
    {
        c = text[i]-32;
        ML_DrawTile(image, sprite, x+i*sprite->tileWidth*sprite->scaleX, y, c);
    }
}

void ML_SetBackgroundColor(GXColor color)
{
	GX_SetCopyClear(color, 0x00ffffff);	
}

// These two next functions are mainly inspired from GRRLIB, thank you for these equation/algorithm !
void ML_SetPixelColor(ML_Image *image, int x, int y, u32 color) 
{
	u8 *truc = (u8*)image->data;
	u32 offset;

	offset = (((y >> 2)<<4)*image->width) + ((x >> 2)<<6) + (((y%4 << 2) + x%4 ) <<1); // Fuckin equation found by NoNameNo ;)

	*(truc+offset)=(color) & 0xFF;
	*(truc+offset+1)=(color>>24) & 0xFF;
	*(truc+offset+32)=(color>>16) & 0xFF;
	*(truc+offset+33)=(color>>8) & 0xFF;
	
	image->data = truc;
	
	DCFlushRange(image->data, image->width * image->height * 4);
	GX_InitTexObj(&image->texObj, image->data, image->width, image->height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);	
	GX_InitTexObjLOD(&image->texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
}

u32 ML_GetPixelColor(ML_Image *image, int x, int y) 
{
	u8 *truc = (u8*)image->data;
	u8 r, g, b, a;
	u32 offset;

	offset = (((y >> 2)<<4)*image->width) + ((x >> 2)<<6) + (((y%4 << 2) + x%4 ) << 1); // Fuckin equation found by NoNameNo ;)

	a=*(truc+offset);
	r=*(truc+offset+1);
	g=*(truc+offset+32);
	b=*(truc+offset+33);

	return((r<<24) | (g<<16) | (b<<8) | a);
}

void ML_SplashScreen()
{
	int i = 0;
	bool ok = 0;
	
	ML_Image image;
	_initImage(&image);
	ok = read_png_gx_file_buffer(MLlib_SplashScreen_png, &image);
	
	if(!ok) 
		return;
	
	GX_InitTexObj(&image.texObj, image.data, image.width, image.height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
	GX_InitTexObjLOD(&image.texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
	
	while(ok)
	{
		if(Wiimote[0].Held.A) { ok = 0; }
		
		_drawImage(&image.texObj, 0, 0, image.width, image.height, 1, 1, 0, 255, 0, 0, 0, 0);
		
		i++;
		if(i >= 1800) ok = 0;
	
		ML_Refresh();
	}
	ML_Refresh();
	VIDEO_WaitVSync();
	ML_DeleteImage(&image);
}

bool ML_Screenshot(const char *filename)
{
	if(fatInitDefault()) 
	{
		png_uint_32 rowbytes;
		unsigned int x, y, buffWidth;
		unsigned int width = screenMode->fbWidth;
		unsigned int height = screenMode->efbHeight;
		void *buffer = xfb[whichfb];
		FILE *fp;		

		// Check if the user has selected a file to write the image
		if (!(fp = fopen(filename, "wb")))
			return 0;
		
		// Allocation of libpng structs
		png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png_ptr)
		{
			fclose (fp);
			return 0;
		}

		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
			fclose(fp);
			return 0;
		}

		// Default data writer uses function fwrite, so it needs to use our FILE*
		png_init_io(png_ptr, fp);

		// Setup output file properties
		png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, 
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

		// Allocate memory to store the image in RGB format
		rowbytes = width * 3;
		if (rowbytes % 4)
			rowbytes = ((rowbytes / 4) + 1) * 4; // Add extra padding so each row starts in a 4 byte boundary

		png_bytep img_data = malloc(rowbytes * height);
		if(!img_data)
		{
			png_destroy_write_struct (&png_ptr, (png_infopp)NULL);
			fclose (fp);
			return 0;
		}

		png_bytep *row_pointers = malloc (sizeof (png_bytep) * height);
		if (!row_pointers)
		{
			png_destroy_write_struct (&png_ptr, (png_infopp)NULL);
			fclose (fp);
			return 0;
		}

		// Encode YCbYCr image into RGB8 format
		buffWidth = width / 2;
		for (y = 0; y < height; y++)
		{
			row_pointers[y] = img_data + (y * rowbytes);

			for (x = 0; x < (width / 2); x++)
				_PNGU_YCbYCr_TO_RGB8(((unsigned int *)buffer)[y*buffWidth+x], 
					((unsigned char *) row_pointers[y]+x*6), ((unsigned char *) row_pointers[y]+x*6+1),
					((unsigned char *) row_pointers[y]+x*6+2), ((unsigned char *) row_pointers[y]+x*6+3),
					((unsigned char *) row_pointers[y]+x*6+4), ((unsigned char *) row_pointers[y]+x*6+5) );
		}

		// Tell libpng where is our image data
		png_set_rows(png_ptr, info_ptr, row_pointers);

		// Write file header and image data
		png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

		// Tell libpng we have no more data to write
		png_write_end(png_ptr, (png_infop) NULL);

		// Free resources
		free (img_data);
		free (row_pointers);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose (fp);

		return 1;
	} 
	
	return 0;
}

//---------------------------------------------

void _drawImage(GXTexObj *texObj, int x, int y, u16 _width, u16 _height, float scaleX, float scaleY, float angle, u8 alpha, bool tiled, u16 frame, u16 tileWidth, u16 tileHeight)
{
	Mtx44 m, m1, m2, mv;
	u16 width, height;
		
	if(!tiled)
	{
		GX_LoadTexObj(texObj, GX_TEXMAP0);

		GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
		GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

		//width = (_width*scaleX)*0.5;
		//height = (_height*scaleY)*0.5;
		width = _width>>1;
		height = _height>>1;
		guMtxIdentity (m1);
		guMtxScaleApply(m1, m1, scaleX, scaleY, 1.0);
		Vector axis = (Vector) {0, 0, 1};
		guMtxRotAxisDeg (m2, &axis, angle);
		guMtxConcat(m2, m1, m);

		guMtxTransApply(m, m, x+width, y+height, 0);
		guMtxConcat (GXmodelView2D, m, mv);
		GX_LoadPosMtxImm (mv, GX_PNMTX0);

		GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
			GX_Position3f32(-width, -height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
			GX_TexCoord2f32(0, 0);

			GX_Position3f32(width, -height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
			GX_TexCoord2f32(1, 0);

			GX_Position3f32(width, height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
			GX_TexCoord2f32(1, 1);

			GX_Position3f32(-width, height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
			GX_TexCoord2f32(0, 1);
		GX_End();
	}
	else
	{	
		u16 nbTileW = _width/tileWidth;
		u16 nbTileH = _height/tileHeight;
	
		// Frame Correction by spiffen
		f32 FRAME_CORR = 0.001f;
		f32 s1 = (((frame%nbTileW))/(f32)nbTileW)+(FRAME_CORR/_width);
		f32 s2 = (((frame%nbTileW)+1)/(f32)nbTileW)-(FRAME_CORR/_width);
		f32 t1 = (((int)(frame/nbTileW))/(f32)nbTileH)+(FRAME_CORR/_height);
		f32 t2 = (((int)(frame/nbTileW)+1)/(f32)nbTileH)-(FRAME_CORR/_height);
		
		GX_LoadTexObj(texObj, GX_TEXMAP0);
	
		GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
		GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	
		width = tileWidth*0.5;
		height = tileHeight*0.5;
		guMtxIdentity (m1);
		guMtxScaleApply(m1, m1, scaleX, scaleY, 1.0);
		Vector axis = (Vector) {0, 0, 1};
		guMtxRotAxisDeg (m2, &axis, angle);
		guMtxConcat(m2, m1, m);

		guMtxTransApply(m, m, x+width, y+height, 0);
		guMtxConcat (GXmodelView2D, m, mv);
		GX_LoadPosMtxImm (mv, GX_PNMTX0);
	
		GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
			GX_Position3f32(-width, -height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
			GX_TexCoord2f32(s1, t1);

			GX_Position3f32(width, -height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
			GX_TexCoord2f32(s2, t1);

			GX_Position3f32(width, height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
			GX_TexCoord2f32(s2, t2);

			GX_Position3f32(-width, height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
			GX_TexCoord2f32(s1, t2);
		GX_End();
	}
	
	GX_LoadPosMtxImm(GXmodelView2D, GX_PNMTX0);

	GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
	GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);
}

void ML_GX_Init()
{
	f32 yscale = 0;
	u32 xfbHeight = 0;

	GXColor background = {0x00, 0x00, 0x00, 0x00};

	// setup the fifo and then init the flipper
	gp_fifo = (u8 *) memalign(32,DEFAULT_FIFO_SIZE);
	if(gp_fifo == NULL)
        return;
	memset(gp_fifo,0,DEFAULT_FIFO_SIZE);
	GX_Init(gp_fifo,DEFAULT_FIFO_SIZE);

	// clears the bg to color and clears the z buffer
	GX_SetCopyClear(background, 0x00ffffff);

	// other gx setup
	yscale = GX_GetYScaleFactor(screenMode->efbHeight,screenMode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0, 0, screenMode->fbWidth, screenMode->efbHeight);
	GX_SetDispCopySrc(0, 0, screenMode->fbWidth, screenMode->efbHeight);
	GX_SetDispCopyDst(screenMode->fbWidth, xfbHeight);
	GX_SetCopyFilter(screenMode->aa, screenMode->sample_pattern, GX_TRUE, screenMode->vfilter);

	GX_SetFieldMode(screenMode->field_rendering,((screenMode->viHeight==2*screenMode->xfbHeight)?GX_ENABLE:GX_DISABLE));

	if (screenMode->aa)
		GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
	else
		GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
		
	GX_SetDispCopyGamma(GX_GM_1_0);
	
	// setup the vertex descriptor
    // tells the flipper to expect direct data
    
    Mtx44 perspective;
    
    GX_ClearVtxDesc();
    GX_InvVtxCache ();
    GX_InvalidateTexAll();

    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);


    GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GX_SetZMode (GX_FALSE, GX_LEQUAL, GX_TRUE);

    GX_SetNumChans(1);
    GX_SetNumTexGens(1);
    GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    guMtxIdentity(GXmodelView2D);
    //guMtxTransApply (GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, -50.0F);
    guMtxTransApply(GXmodelView2D, GXmodelView2D, 0.0f, 0.0f, -5.0f);
    GX_LoadPosMtxImm(GXmodelView2D, GX_PNMTX0);
	
    guOrtho(perspective,0, 479, 0, 639, 0, 300.0F);
    GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);

    GX_SetViewport(0, 0, screenMode->fbWidth, screenMode->efbHeight, 0, 1);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetAlphaUpdate(GX_TRUE);

    GX_SetCullMode(GX_CULL_NONE);
}

//---------------------------------------------

void ML_GX_Refresh()
{
	GX_DrawDone();
		GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);
	GX_CopyDisp(xfb[whichfb],GX_TRUE);
}


/** ****************************************************
************************ FB ****************************
**************************************************** **/


void ML_InitVideo()
{
	VIDEO_Init();
	screenMode = VIDEO_GetPreferredMode(NULL);
	if(CONF_GetAspectRatio() == CONF_ASPECT_16_9)
    {
        screenMode->viWidth = 678;
        screenMode->viXOrigin = (VI_MAX_WIDTH_PAL - 678)/2;
    }
    
    _screenWidth = screenMode->fbWidth;
    _screenHeight = 480;
    
    xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(screenMode));
	xfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(screenMode));	
    VIDEO_Configure(screenMode);
	console_init(xfb[0], 20, 64, screenMode->fbWidth, screenMode->xfbHeight, screenMode->fbWidth*VI_DISPLAY_PIX_SZ);

	VIDEO_SetNextFramebuffer(xfb[0]);
	VIDEO_ClearFrameBuffer(screenMode, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer(screenMode, xfb[1], COLOR_BLACK);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(screenMode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	whichfb ^= 1;
}

//---------------------------------------------

void ML_CleanVideoBG()
{
	VIDEO_ClearFrameBuffer(screenMode, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer(screenMode, xfb[1], COLOR_BLACK);
}

//---------------------------------------------

void ML_CleanVideoBGColor(int color)
{
	VIDEO_ClearFrameBuffer(screenMode, xfb[0], color);
	VIDEO_ClearFrameBuffer(screenMode, xfb[1], color);
}

//---------------------------------------------

void ML_VSync()
{
	VIDEO_SetNextFramebuffer(xfb[whichfb]);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
}

//---------------------------------------------

void ML_RefreshConsoleMode()
{
	ML_GetPads();
	
	VIDEO_Flush();
	VIDEO_WaitVSync();
	
	ML_CallbackForPowerAndReset(0);
}

//---------------------------------------------

void ML_Refresh()
{
	ML_GetPads();
	ML_GX_Refresh();
	ML_VSync();

	whichfb ^= 1;
	ML_CallbackForPowerAndReset(1);
	GX_InvalidateTexAll();
}

void ML_GX_FreeMemory()
{
	if(xfb[0] != NULL)  { free(MEM_K1_TO_K0(xfb[0])); xfb[0] = NULL; }
	if(xfb[1] != NULL)  { free(MEM_K1_TO_K0(xfb[1])); xfb[1] = NULL; }
	if(gp_fifo != NULL) { free(gp_fifo); gp_fifo = NULL; }
}

