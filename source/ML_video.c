#include <MLlib.h>
#include "MLlib_SplashScreen_png.h"

// FB
static void *xfb[2] = { NULL, NULL };
u32 whichfb = 0;

// GX
void *gp_fifo = NULL;
Mtx GXmodelView2D;

// Others
u8 _alphaFade;

/**
* \file
* \brief This file contains video functions.
*/


/** ****************************************************
************************ GX ****************************
**************************************************** **/

bool ML_LoadSpriteFromBuffer(u16 nb, const u8 *filename, int x, int y)
{
	png_file_gx_t png_data = read_png_gx_file_buffer(filename);
	
	if(png_data.gxdata==NULL) return 0;
	
	sprite[nb].data = png_data.gxdata;
	sprite[nb].width = png_data.width;
	sprite[nb].height = png_data.height;
	sprite[nb].x = x;
	sprite[nb].y = y;
	sprite[nb].visible = TRUE;
	sprite[nb].dx = 1;
	sprite[nb].dy = 1;
	sprite[nb].rotated = FALSE;
	sprite[nb].scaleX = 1;
	sprite[nb].scaleY = 1;
	sprite[nb].alpha = 255;
	sprite[nb].tiled = FALSE;
	sprite[nb].animated = FALSE;
	sprite[nb].currentFrame = 0;

	GX_InitTexObj(&sprite[nb].texObj, sprite[nb].data, sprite[nb].width, sprite[nb].height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);	
	GX_InitTexObjLOD(&sprite[nb].texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
	
	return 1;
}

bool ML_LoadSpriteFromFile(u16 nb, char *filename, int x, int y)
{
	png_file_gx_t png_data = read_png_gx_file(filename);
	
	if(png_data.gxdata==NULL) return 0;
	
	sprite[nb].data = png_data.gxdata;
	sprite[nb].width = png_data.width;
	sprite[nb].height = png_data.height;
	sprite[nb].x = x;
	sprite[nb].y = y;
	sprite[nb].visible = TRUE;
	sprite[nb].dx = 1;
	sprite[nb].dy = 1;
	sprite[nb].rotated = FALSE;
	sprite[nb].scaleX = 1;
	sprite[nb].scaleY = 1;
	sprite[nb].alpha = 255;
	sprite[nb].tiled = FALSE;
	sprite[nb].animated = FALSE;
	sprite[nb].currentFrame = 0;

	GX_InitTexObj(&sprite[nb].texObj, sprite[nb].data, sprite[nb].width, sprite[nb].height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);	
	GX_InitTexObjLOD(&sprite[nb].texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
	
	return 1;
}

void ML_DrawSprite(u16 nb)
{
	ML_DrawSpriteXY(nb, sprite[nb].x, sprite[nb].y);
}

void ML_DrawSpriteXY(u16 nb, int x, int y)
{
	sprite[nb].x = x; sprite[nb].y = y;
	
	if(!sprite[nb].visible || &sprite[nb].texObj == NULL || sprite[nb].alpha == 0 || sprite[nb].scaleX == 0 || sprite[nb].scaleY == 0 || sprite[nb].width == 0 || sprite[nb].height == 0) return;
	
	if(!sprite[nb].animated)
	{
		if(!sprite[nb].tiled)
		{
			Mtx44 m, m1, m2, mv;
			u16 width, height;
	
			GX_LoadTexObj(&sprite[nb].texObj, GX_TEXMAP0);
	
			GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
			GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);
	
			width = sprite[nb].width>>1;
			height = sprite[nb].height>>1;
			guMtxIdentity (m1);
			guMtxScaleApply(m1, m1, sprite[nb].scaleX, sprite[nb].scaleY, 1.0);
			Vector axis = (Vector) {0, 0, 1};
			guMtxRotAxisDeg (m2, &axis, sprite[nb].angle);
			guMtxConcat(m2, m1, m);

			guMtxTransApply(m, m, sprite[nb].x+width, sprite[nb].y+height, 0);
			guMtxConcat (GXmodelView2D, m, mv);
			GX_LoadPosMtxImm (mv, GX_PNMTX0);
	
			GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
			GX_Position3f32(-width, -height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, sprite[nb].alpha);
			GX_TexCoord2f32(0, 0);

			GX_Position3f32(width, -height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, sprite[nb].alpha);
			GX_TexCoord2f32(1, 0);

			GX_Position3f32(width, height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, sprite[nb].alpha);
			GX_TexCoord2f32(1, 1);

			GX_Position3f32(-width, height, 0);
			GX_Color4u8(0xFF, 0xFF, 0xFF, sprite[nb].alpha);
			GX_TexCoord2f32(0, 1);
			GX_End();
	
			GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);

			GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
			GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);
		}
		else
		{
			ML_DrawTile(nb, sprite[nb].x, sprite[nb].y, sprite[nb].currentFrame);
		}
	}
	else
	{
		if(sprite[nb].i == sprite[nb].waitForXRefreshBetweenFrames)
		{
			sprite[nb].i = 0;
			if(sprite[nb].currentFrame == sprite[nb].nbTiles-1) sprite[nb].currentFrame = 0;
			else sprite[nb].currentFrame++;
		} else sprite[nb].i++;
		
		ML_DrawTile(nb, sprite[nb].x, sprite[nb].y, sprite[nb].currentFrame);
	}
}

void ML_DrawSpriteFull(u16 nb, int x, int y, float angle, float scaleX, float scaleY, u8 alpha)
{
	sprite[nb].x = x;
	sprite[nb].y = y;
	sprite[nb].angle = angle;
	sprite[nb].scaleX = scaleX;
	sprite[nb].scaleY = scaleY;
	sprite[nb].alpha = alpha;
	
	ML_DrawSpriteXY(nb, sprite[nb].x, sprite[nb].y);
}

void ML_InitTile(u16 nb, u16 width, u16 height)
{
	sprite[nb].tiled = TRUE;
	sprite[nb].tileWidth = width;
	sprite[nb].tileHeight = height;
	sprite[nb].nbTiles = (sprite[nb].width/sprite[nb].tileWidth) * (sprite[nb].height/sprite[nb].tileHeight);
}

void ML_DrawTile(u16 nb, int x, int y, u16 frame)
{
	if(!sprite[nb].visible || &sprite[nb].texObj == NULL || sprite[nb].alpha == 0 || sprite[nb].scaleX == 0 || sprite[nb].scaleY == 0 || sprite[nb].width == 0 || sprite[nb].height == 0) return;

	if(sprite[nb].tiled)
	{
		Mtx44 m, m1, m2, mv;
		u16 width, height;
	
		u16 nbTileW = sprite[nb].width/sprite[nb].tileWidth;
		u16 nbTileH = sprite[nb].height/sprite[nb].tileHeight;
	
		// Frame Correction by spiffen
		f32 FRAME_CORR = 0.001f;
		f32 s1 = (((frame%nbTileW))/(f32)nbTileW)+(FRAME_CORR/sprite[nb].width);
		f32 s2 = (((frame%nbTileW)+1)/(f32)nbTileW)-(FRAME_CORR/sprite[nb].width);
		f32 t1 = (((int)(frame/nbTileW))/(f32)nbTileH)+(FRAME_CORR/sprite[nb].height);
		f32 t2 = (((int)(frame/nbTileW)+1)/(f32)nbTileH)-(FRAME_CORR/sprite[nb].height);
		
		GX_LoadTexObj(&sprite[nb].texObj, GX_TEXMAP0);
	
		GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
		GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);
	
		width = sprite[nb].tileWidth*0.5;
		height = sprite[nb].tileHeight*0.5;
		guMtxIdentity (m1);
		guMtxScaleApply(m1, m1, sprite[nb].scaleX, sprite[nb].scaleY, 1.0);
		Vector axis = (Vector) {0, 0, 1};
		guMtxRotAxisDeg (m2, &axis, sprite[nb].angle);
		guMtxConcat(m2, m1, m);

		guMtxTransApply(m, m, x+width, y+height, 0);
		guMtxConcat (GXmodelView2D, m, mv);
		GX_LoadPosMtxImm (mv, GX_PNMTX0);
	
		GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
		GX_Position3f32(-width, -height, 0);
		GX_Color4u8(0xFF, 0xFF, 0xFF, sprite[nb].alpha);
		GX_TexCoord2f32(s1, t1);

		GX_Position3f32(width, -height, 0);
		GX_Color4u8(0xFF, 0xFF, 0xFF, sprite[nb].alpha);
		GX_TexCoord2f32(s2, t1);

		GX_Position3f32(width, height, 0);
		GX_Color4u8(0xFF, 0xFF, 0xFF, sprite[nb].alpha);
		GX_TexCoord2f32(s2, t2);

		GX_Position3f32(-width, height, 0);
		GX_Color4u8(0xFF, 0xFF, 0xFF, sprite[nb].alpha);
		GX_TexCoord2f32(s1, t2);
		GX_End();
	
		GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);

		GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
		GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);
	}
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

void ML_Text(int nb, int x, int y, const char *text, ...)
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
        if(buffer[i] == '\n') { y += sprite[nb].tileHeight*sprite[nb].scaleY; j = 0; }
        else 
        { 
        	if((x+j*sprite[nb].tileWidth*sprite[nb].scaleX) >= screenMode->fbWidth) { y += sprite[nb].tileHeight*sprite[nb].scaleY; i--; j = -1; }
        	else  { ML_DrawTile(nb, x+j*sprite[nb].tileWidth*sprite[nb].scaleX, y, c); j++; }
        }
    }
}

void ML_SimpleText(int nb, int x, int y, const char *text)
{
	int i = 0, size = strlen(text);
	u8 c = 0;
	
    for(i=0; i < size; i++) 
    {
        c = text[i]-32;
        ML_DrawTile(nb, x+i*sprite[nb].tileWidth*sprite[nb].scaleX, y, c);
    }
}

void ML_SetBackgroundColor(GXColor color)
{
	GX_SetCopyClear(color, 0x00ffffff);	
}

// These two next functions are mainly inspired from GRRLIB, thank you for these equation/algorithm !
void ML_SetPixelColor(int nb, int x, int y, u32 color) 
{
	u8 *truc = (u8*)sprite[nb].data;
	u32 offset;

	offset = (((y >> 2)<<4)*sprite[nb].width) + ((x >> 2)<<6) + (((y%4 << 2) + x%4 ) <<1); // Fuckin equation found by NoNameNo ;)

	*(truc+offset)=(color) & 0xFF;
	*(truc+offset+1)=(color>>24) & 0xFF;
	*(truc+offset+32)=(color>>16) & 0xFF;
	*(truc+offset+33)=(color>>8) & 0xFF;
	
	sprite[nb].data = truc;
	
	DCFlushRange(sprite[nb].data, sprite[nb].width * sprite[nb].height * 4);
	GX_InitTexObj(&sprite[nb].texObj, sprite[nb].data, sprite[nb].width, sprite[nb].height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);	
	GX_InitTexObjLOD(&sprite[nb].texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
}

u32 ML_GetPixelColor(int nb, int x, int y) 
{
	u8 *truc = (u8*)sprite[nb].data;
	u8 r, g, b, a;
	u32 offset;

	offset = (((y >> 2)<<4)*sprite[nb].width) + ((x >> 2)<<6) + (((y%4 << 2) + x%4 ) << 1); // Fuckin equation found by NoNameNo ;)

	a=*(truc+offset);
	r=*(truc+offset+1);
	g=*(truc+offset+32);
	b=*(truc+offset+33);

	return((r<<24) | (g<<16) | (b<<8) | a);
}

void ML_SplashScreen()
{
	bool continuer = 1;
	int i = 0;
	
	GXTexObj texObj;
	Mtx44 m, m1, m2, mv;
	u16 width, height;
	
	png_file_gx_t png_data = read_png_gx_file_buffer(MLlib_SplashScreen_png);
	
	if(png_data.gxdata==NULL) 
		return;
	
	GX_InitTexObj(&texObj, png_data.gxdata, png_data.width, png_data.height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
	GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
	
	width = png_data.width * 0.5;
	height = png_data.height * 0.5;
	Vector axis = (Vector) {0, 0, 1 };
	
	while(continuer)
	{
		if(Wiimote[0].Held.A) { continuer = 0; }
		
		GX_LoadTexObj(&texObj, GX_TEXMAP0);
	
		GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
		GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);
		
		guMtxIdentity (m1);
		guMtxScaleApply(m1, m1, 1, 1, 1.0);
		guMtxRotAxisDeg (m2, &axis, 0);
		guMtxConcat(m2, m1, m);

		guMtxTransApply(m, m, 0+width, 0+height, 0);
		guMtxConcat (GXmodelView2D, m, mv);
		GX_LoadPosMtxImm (mv, GX_PNMTX0);
		
		GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
		GX_Position3f32(-width, -height, 0);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(0, 0);

		GX_Position3f32(width, -height, 0);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(1, 0);

		GX_Position3f32(width, height, 0);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(1, 1);

		GX_Position3f32(-width, height, 0);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(0, 1);
		GX_End();
		GX_LoadPosMtxImm(GXmodelView2D, GX_PNMTX0);

		GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
		GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);
		
		i++;
		if(i >= 1800) continuer = 0;
	
		ML_Refresh();
	}
	ML_Refresh();
	VIDEO_WaitVSync();
	DCFlushRange(png_data.gxdata, png_data.width * png_data.height * 4);
}

/*bool ML_Screenshot(const char *filename)
{
	if(fatInitDefault()) 
	{
		buffer = _PNGU_EncodeFromYCbYCr(pngContext, rmode->fbWidth, rmode->efbHeight, xfb[fb], 0);
		
		// enregsitrer le buffer dans fichier filename
		return 1;
	} 
	return 0;
}*/

//---------------------------------------------

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

