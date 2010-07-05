#include <MLlib.h>

/**
* \file
* \brief This file contains sprites functions.
*/

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

void ML_DrawSprite(ML_Sprite *sprite)
{
	ML_DrawSpriteXY(sprite, sprite->x, sprite->y);
}

void ML_DrawSpriteXY(ML_Sprite *sprite, int x, int y)
{
	sprite->x = x; sprite->y = y;
	
	if(!ML_IsSpriteVisible(sprite)) return;
	
	if(!sprite->animated)
	{
		_drawImage(&sprite->image->texObj, sprite->image->data, sprite->x, sprite->y, sprite->width, sprite->height, sprite->scaleX, sprite->scaleY, sprite->angle, sprite->alpha, sprite->tiled, sprite->currentFrame, sprite->tileWidth, sprite->tileHeight, sprite->flipX, sprite->flipY);
	}
	else
	{
		if(sprite->i == sprite->waitForXRefreshBetweenFrames)
		{
			sprite->i = 0;
			if(sprite->currentFrame == sprite->animeTo) 
			{ 
				if(sprite->timesAnimated > 1)
				{
					sprite->timesAnimated--;
				}
				else if(sprite->timesAnimated == 1)
				{
					sprite->timesAnimated = 0;
					sprite->animated = false;
				}	
				
				sprite->currentFrame = sprite->animeFrom;			
			}
			else sprite->currentFrame++;
		} else sprite->i++;
		
		ML_DrawTile(sprite, sprite->x, sprite->y, sprite->currentFrame);
	}
}

void ML_DrawSpriteFull(ML_Sprite *sprite, int x, int y, float angle, float scaleX, float scaleY, u8 alpha)
{
	sprite->x = x;
	sprite->y = y;
	sprite->angle = angle;
	sprite->scaleX = scaleX;
	sprite->scaleY = scaleY;
	sprite->alpha = alpha;
	
	ML_DrawSpriteXY(sprite, sprite->x, sprite->y);
}

void ML_InitTile(ML_Sprite *sprite, u16 width, u16 height)
{
	sprite->tiled = TRUE;
	sprite->tileWidth = width;
	sprite->tileHeight = height;
	sprite->nbTiles = (sprite->width/sprite->tileWidth) * (sprite->height/sprite->tileHeight);
}

void ML_DrawTile(ML_Sprite *sprite, int x, int y, u16 frame)
{
	if(!ML_IsSpriteVisible(sprite)) return;
	
	 sprite->x = x; sprite->y = y;

	_drawImage(&sprite->image->texObj, sprite->image->data, sprite->x, sprite->y, sprite->width, sprite->height, sprite->scaleX, sprite->scaleY, sprite->angle, sprite->alpha, sprite->tiled, frame, sprite->tileWidth, sprite->tileHeight, sprite->flipX, sprite->flipY);
}

void ML_DrawSpriteText(ML_Sprite *sprite, int x, int y, const char *text, ...)
{ 
	int i = 0, size = 0, j = 0; // current index, size, number of char for this line passed
    char buffer[1024];
	u8 c = 0; // current char
	
	size = strlen(text);
	if(size > 1024) return;
	
    va_list argp;
    va_start(argp, text);
    size = vsprintf(buffer, text, argp);
    va_end(argp);
	
    for(i=0; i < size; i++)
    {
        c = buffer[i]-32;
        if(buffer[i] == '\n')
        {
        	y += sprite->tileHeight*sprite->scaleY;
        	j = 0;
        }
        else if(x+j*sprite->tileWidth*sprite->scaleX >= _screenWidth) 
        { 
        	y += sprite->tileHeight*sprite->scaleY; 
        	ML_DrawTile(sprite, x, y, c); 
        	j = 0;
        }
        else ML_DrawTile(sprite, x+j*sprite->tileWidth*sprite->scaleX, y, c); j++;
        
        if(y+sprite->tileHeight*sprite->scaleY >= _screenHeight) return;
    }
}

void ML_DrawSpriteTextBox(ML_Sprite *sprite, int x, int y, int x2, int y2, const char *text, ...)
{
	int i = 0, size = 0, j = 0; // current index, size, number of char for this line passed
    char buffer[1024];
	u8 c = 0; // current char
	
	size = strlen(text);
	if(size > 1024) return;
	
    va_list argp;
    va_start(argp, text);
    size = vsprintf(buffer, text, argp);
    va_end(argp);
	
    for(i=0; i < size; i++)
    {
        c = buffer[i]-32;
        if(buffer[i] == '\n')
        {
        	y += sprite->tileHeight*sprite->scaleY;
        	j = 0;
        }
        else if(x+j*sprite->tileWidth*sprite->scaleX >= x2) 
        { 
        	y += sprite->tileHeight*sprite->scaleY; 
        	ML_DrawTile(sprite, x, y, c); 
        	j = 0;
        }
        else ML_DrawTile(sprite, x+j*sprite->tileWidth*sprite->scaleX, y, c); j++;
        
        if(y+sprite->tileHeight*sprite->scaleY >= y2) return;
    }
}

// Contribution of Cid2Mizard
void ML_DrawSpriteTextLimit(ML_Sprite *sprite, int x, int y, char *text, u8 limit) 
{
	char temp = text[limit];  
	text[limit] = 0;  
	ML_DrawSpriteSimpleText(sprite, x, y, text);  
	text[limit] = temp;
}

void ML_DrawSpriteSimpleText(ML_Sprite *sprite, int x, int y, const char *text)
{
	int i = 0, size = strlen(text);
	u8 c = 0;
	
    for(i=0; i < size; i++)
    {
        c = text[i]-32;
        ML_DrawTile(sprite, x+i*sprite->tileWidth*sprite->scaleX, y, c);
    }
}

void ML_CloneSprite(ML_Sprite *sprite1, ML_Sprite *sprite2)
{
	*sprite2 = *sprite1;
}

bool ML_IsSpriteVisible(ML_Sprite *sprite)
{
 	if(!sprite->visible || sprite->alpha == 0 || sprite->scaleX == 0 || sprite->scaleY == 0 || sprite->width == 0 || sprite->height == 0)
 		sprite->visible = 0;
 	else sprite->visible = 1;
 	
 	return sprite->visible;
}

void ML_AnimateSprite(ML_Sprite *sprite, bool enabled, u8 waitForXRefreshBetweenFrames)
{
	ML_AnimateSpriteEx2(sprite, enabled, waitForXRefreshBetweenFrames, 0, sprite->nbTiles-1, 0);
}

void ML_AnimateSpriteEx(ML_Sprite *sprite, bool enabled, u8 waitForXRefreshBetweenFrames, u8 from, u8 to)
{
	ML_AnimateSpriteEx2(sprite, enabled, waitForXRefreshBetweenFrames, from, to, 0);
}

void ML_AnimateSpriteEx2(ML_Sprite *sprite, bool enabled, u8 waitForXRefreshBetweenFrames, u8 from, u8 to, u8 times)
{
	if(sprite->tiled)
	{
		sprite->animated = enabled;
		sprite->waitForXRefreshBetweenFrames = waitForXRefreshBetweenFrames;
		sprite->animeFrom = from;
		sprite->animeTo = to;
		sprite->timesAnimated = times;
		sprite->currentFrame = from;
	}
}

void ML_MoveSpriteWiimotePad(ML_Sprite *sprite, u8 wpad)
{
	if(Wiimote[wpad].Held.Right) sprite->x += sprite->dx;
	else if(Wiimote[wpad].Held.Left) sprite->x -= sprite->dx;
	if(Wiimote[wpad].Held.Down) sprite->y += sprite->dy;
	else if(Wiimote[wpad].Held.Up) sprite->y -= sprite->dy;
}

void ML_MoveSpriteWiimoteIR(ML_Sprite *sprite, u8 wpad)
{
	if(Wiimote[wpad].IR.Valid)// && sprite->x > 0 - sprite->width*sprite->scaleX && sprite->y > 0 - sprite->height*sprite->scaleY)
	{
		//sprite->x = (Wiimote[wpad].IR.X / screenMode->fbWidth * (screenMode->fbWidth + (sprite->width*sprite->scaleX) * 2)) - (sprite->width*sprite->scaleX/2);
		//sprite->y = (Wiimote[wpad].IR.Y / screenMode->xfbHeight * (screenMode->xfbHeight + (sprite->height*sprite->scaleY) * 2)) - (sprite->height*sprite->scaleY/2);
		
		sprite->x = Wiimote[wpad].IR.X - (sprite->tileWidth*sprite->scaleX/2);
		sprite->y = Wiimote[wpad].IR.Y - (sprite->tileHeight*sprite->scaleY/2);
		
		sprite->visible = true;
	} else sprite->visible = false;
}

bool ML_IsWiimoteInSprite(u8 wpad, const ML_Sprite *sprite)
{	
	int cursorX = Wiimote[wpad].IR.X;
	int cursorY = Wiimote[wpad].IR.Y;
	
	int sp1_left = sprite->x;
	int sp1_right = sprite->x + (sprite->tileWidth*sprite->scaleX);
	int sp1_up = sprite->y;
	int sp1_down = sprite->y + (sprite->tileHeight*sprite->scaleY);
	
	if(sp1_left > cursorX ||
	   sp1_right < cursorX ||
	   sp1_up > cursorY ||
	   sp1_down < cursorY)
	   return false;
	else return true;
}

bool ML_IsWiimoteInSpriteEx(u8 wpad, const ML_Sprite *sprite)
{
	int sp1_left = Wiimote[wpad].IR.X;
	int sp1_right = Wiimote[wpad].IR.X;
	int sp1_up = Wiimote[wpad].IR.Y;
	int sp1_down = Wiimote[wpad].IR.Y;
	
	int sp2_left = sprite->x;
	int sp2_right = sprite->x + sprite->tileWidth*sprite->scaleX;
	int sp2_up = sprite->y;
	int sp2_down = sprite->y + sprite->tileHeight*sprite->scaleY;
	
	if(sp1_left > sp2_right ||
       sp1_right < sp2_left ||
       sp1_up > sp2_down ||
       sp1_down < sp2_up)
       return false;
    else
    {
    	int rect_left, rect_up, posX_spr2 = 0, posY_spr2 = 0;
		
		if(sp1_up < sp2_up)
			rect_up = sp2_up;
		else rect_up = sp1_up;

		if(sp1_left < sp2_left)
			rect_left = sp2_left;
		else rect_left = sp1_left;
		
		posX_spr2 = abs(sp2_left - rect_left);
		posY_spr2 = abs(sp2_up - rect_up);

		// Exploration des pixels dans le rectangle commun		
		u32 offset = (((posY_spr2 >> 2)<<4)*(sprite->width)) + ((posX_spr2 >> 2)<<6) + (((posY_spr2%4 << 2) + posX_spr2%4 ) << 1); 
		u8 a = *(sprite->image->data+offset);
		
		// Teste si le pixel n'est pas transparent
		if(a != 0x00)
			return true;
		
		return false;
	}
}

bool ML_IsCollision(const ML_Sprite *sprite, const ML_Sprite *sprite2)
{
	return ML_IsCollisionRectRect(sprite->x, sprite->y, sprite->tileWidth*sprite->scaleX, sprite->tileHeight*sprite->scaleY, sprite2->x, sprite2->y, sprite2->tileWidth*sprite2->scaleX, sprite2->tileHeight*sprite2->scaleY);
}

bool ML_IsCollisionEx(const ML_Sprite *sprite, const ML_Sprite *sprite2)
{
	int sp1_left = sprite->x;
	int sp1_right = sprite->x + sprite->tileWidth*sprite->scaleX;
	int sp1_up = sprite->y;
	int sp1_down = sprite->y + sprite->tileHeight*sprite->scaleY;
	
	int sp2_left = sprite2->x;
	int sp2_right = sprite2->x + sprite2->tileWidth*sprite2->scaleX;
	int sp2_up = sprite2->y;
	int sp2_down = sprite2->y + sprite2->tileHeight*sprite2->scaleY;
	
	if(sp1_left > sp2_right ||
       sp1_right < sp2_left ||
       sp1_up > sp2_down ||
       sp1_down < sp2_up)
       return false;
    else
    {
    	int rect_left, rect_right, rect_down, rect_up, x, y;
    	
		// Test de collision de pixel
		// Bord du bas du rectangle
		if(sp1_down > sp2_down)
			rect_down = sp2_down;
		else rect_down = sp1_down;

		// Bord du haut du rectangle
		if(sp1_up < sp2_up)
			rect_up = sp2_up;
		else rect_up = sp1_up;

		// Bord de droite du rectangle
		if(sp1_right > sp2_right)
			rect_right = sp2_right;
		else rect_right = sp1_right;

		// Bord de gauche du rectangle
		if(sp1_left < sp2_left)
			rect_left = sp2_left;
		else rect_left = sp1_left;
		
		// Dimensions du rectangle
		int rect_width = rect_right - rect_left;
		int rect_height = rect_down - rect_up;

		// Position dans le premier sprite
		int posX_spr1 = abs(sp1_left - rect_left);
		int posY_spr1 = abs(sp1_up - rect_up);

		// Position dans le deuxième sprite
		int posX_spr2 = abs(sp2_left - rect_left);
		int posY_spr2 = abs(sp2_up - rect_up);

		// Exploration des pixels dans le rectangle commun
		u32 offset;
		u8 a1, a2;
		
		for(x = 0; x < rect_width; x++)
		{
			for(y = 0; y < rect_height; y++)
			{
				offset = ((((y + posY_spr1) >> 2)<<4)*sprite->tileWidth) + (((x+posX_spr1) >> 2)<<6) + ((((y+posY_spr1)%4 << 2) + (x+posX_spr1)%4 ) << 1); 
				a1 = *(sprite->image->data+offset);
				
				offset = ((((y + posY_spr2) >> 2)<<4)*sprite2->tileWidth) + (((x+posX_spr2) >> 2)<<6) + ((((y+posY_spr2)%4 << 2) + (x+posX_spr2)%4 ) << 1); 
				a2 = *(sprite2->image->data+offset);
				
				// Teste si les deux pixels ne sont pas transparents
				if(a1 != 0x00 && a2 != 0x00)
					return true;
			}
		}
		return false;
	}
}

bool ML_IsCollisionSpriteRect(const ML_Sprite *sprite, int x, int y, int width, int height)
{
	return ML_IsCollisionRectRect(sprite->x, sprite->y, sprite->tileWidth*sprite->scaleX, sprite->tileHeight*sprite->scaleY, x, y, width, height);
}

bool ML_IsCollisionSpriteCircle(const ML_Sprite *sprite, int centerX, int centerY, int radius)
{
	return ML_IsCollisionRectCircle(sprite->x, sprite->y, sprite->tileWidth*sprite->scaleX, sprite->tileHeight*sprite->scaleY, centerX, centerY, radius);
}

bool ML_IsCollisionRectRect(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int sp1_right = x1 + width1;
	int sp1_down = y1 + height1;
	
	int sp2_right = x2 + width2;
	int sp2_down = y2 + height2;
	
	if(x1 > sp2_right ||
		sp1_right < x2 ||
		y1 > sp2_down ||
		sp1_down < y2)
		return false;
	else return true;
}

bool ML_IsCollisionCircleCircle(int centerX1, int centerY1, int radius1, int centerX2, int centerY2, int radius2)
{
	int radiusTotal = radius1+radius2;
	
	if(ML_Distance(centerX1, centerY1, centerX2, centerY2) < radiusTotal*radiusTotal)
		return true;
		
	return false;
}

bool ML_IsCollisionRectCircle(int x, int y, int width, int height, int centerX, int centerY, int radius)
{
	int circleDistanceX = abs(centerX - x - width/2);
	int circleDistanceY = abs(centerY - y - height/2);

	if(circleDistanceX > (width/2 + radius)) return false;
	if(circleDistanceY > (height/2 + radius)) return false;
	
	if(circleDistanceX <= (width/2)) return true;
	if(circleDistanceY <= (height/2)) return true;
	
	int cornerDistance = ((circleDistanceX - width/2)^2) + ((circleDistanceY - height/2)^2);

	return (cornerDistance <= radius*radius);
}

void ML_Cursor(ML_Sprite *sprite, u8 wpad)
{
	ML_MoveSpriteWiimoteIR(sprite, wpad);
	sprite->angle = Wiimote[wpad].IR.Angle;
	
	if(sprite->animated)
		ML_DrawSprite(sprite);
	else if(sprite->tiled)
		ML_DrawTile(sprite, sprite->x, sprite->y, sprite->currentFrame);
	else ML_DrawSprite(sprite);
	
	if(Wiimote[wpad].IR.Valid)// && sprite->x > 0 - sprite->width*sprite->scaleX && sprite->y > 0 - sprite->height*sprite->scaleY) 
	{
		sprite->x += sprite->tileWidth*sprite->scaleX/2;
		sprite->y += sprite->tileHeight*sprite->scaleY/2;
	}
}

void ML_RotateSprite(ML_Sprite *sprite, float angle, u8 autoRotate)
{
	sprite->rotated = autoRotate;	
	if(sprite->rotated)
	{
		sprite->angle += angle;
	} else sprite->angle = angle;
}

void ML_SetSpriteXY(ML_Sprite *sprite, int x, int y) { sprite->x = x; sprite->y = y; }
void ML_SetSpriteX(ML_Sprite *sprite, int x) { sprite->x = x; }
void ML_SetSpriteY(ML_Sprite *sprite, int y) { sprite->y = y; }
void ML_SetSpriteSize(ML_Sprite *sprite, u16 width, u16 height) { sprite->width = width; sprite->height = height; }
void ML_SetSpriteScale(ML_Sprite *sprite, float scaleX, float scaleY) { sprite->scaleX = scaleX; sprite->scaleY = scaleY; }
void ML_SetSpriteVelocity(ML_Sprite *sprite, int dx, int dy) { sprite->x = dx; sprite->dy = dy; }
void ML_SetSpriteAlpha(ML_Sprite *sprite, u8 alpha) { sprite->alpha = alpha; }
void ML_FlipSpriteX(ML_Sprite *sprite, bool flipX) { sprite->flipX = flipX; }
void ML_FlipSpriteY(ML_Sprite *sprite, bool flipY) { sprite->flipY = flipY; }
void ML_FlipSpriteXY(ML_Sprite *sprite, bool flipX, bool flipY) { sprite->flipX = flipX; sprite->flipY = flipY; }

