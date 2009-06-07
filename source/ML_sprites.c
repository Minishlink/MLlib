#include <MLlib.h>

/**
* \file
* \brief This file contains sprites functions.
*/

void ML_CloneSprite(ML_Sprite *sprite, ML_Sprite *sprite2)
{
	*sprite = *sprite2;
}

bool ML_IsSpriteVisible(ML_Sprite *sprite)
{
 	if(!sprite->visible || &sprite->texObj == NULL || sprite->alpha == 0 || sprite->scaleX == 0 || sprite->scaleY == 0 || sprite->width == 0 || sprite->height == 0)
 		sprite->visible = 0;
 	else sprite->visible = 1;
 	
 	return sprite->visible;
}

void ML_AnimateSprite(ML_Sprite *sprite, bool enabled, u8 waitForXRefreshBetweenFrames)
{
	ML_AnimateSpriteEx(sprite, enabled, waitForXRefreshBetweenFrames, 0, sprite->nbTiles);
}

void ML_AnimateSpriteEx(ML_Sprite *sprite, bool enabled, u8 waitForXRefreshBetweenFrames, u8 from, u8 to)
{
	if(sprite->tiled)
	{
		sprite->animated = enabled;
		sprite->waitForXRefreshBetweenFrames = waitForXRefreshBetweenFrames;
		sprite->anime_from = from;
		sprite->anime_to = to;
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
	if(Wiimote[wpad].IR.Valid)
	{
		sprite->x = (Wiimote[wpad].IR.X / screenMode->fbWidth * (screenMode->fbWidth + (sprite->width*sprite->scaleX) * 2)) - (sprite->width*sprite->scaleX);
		sprite->y = (Wiimote[wpad].IR.Y / screenMode->xfbHeight * (screenMode->xfbHeight + (sprite->height*sprite->scaleY) * 2)) - (sprite->height*sprite->scaleY);
		
		sprite->visible = true;
	} else sprite->visible = false;
}

bool ML_IsWiimoteInSprite(u8 wpad, ML_Sprite *sprite)
{	
	int cursorX = (Wiimote[wpad].IR.X / screenMode->fbWidth * (screenMode->fbWidth + 2)) - 1;
	int cursorY = (Wiimote[wpad].IR.Y / screenMode->xfbHeight * (screenMode->xfbHeight + 2)) - 1;
	
	if(cursorX >= sprite->x &&
	   cursorX <= sprite->x + sprite->width*sprite->scaleX &&
	   cursorY >= sprite->y &&
	   cursorY <= sprite->y + sprite->height*sprite->scaleY
	  ) return true;
	  else return false;
}

bool ML_IsCollision(const ML_Sprite *sprite, const ML_Sprite *sprite2)
{
	int sp1_left = sprite->x;
	int sp1_right = sprite->x + sprite->width*sprite->scaleX;
	int sp1_up = sprite->y;
	int sp1_down = sprite->y + sprite->height*sprite->scaleY;
	
	int sp2_left = sprite2->x;
	int sp2_right = sprite2->x + sprite2->width*sprite2->scaleX;
	int sp2_up = sprite2->y;
	int sp2_down = sprite2->y + sprite2->height*sprite2->scaleY;
	
	if(sp1_left > sp2_right ||
	   sp1_right < sp2_left ||
	   sp1_up > sp2_down ||
	   sp1_down < sp2_up)
	   return false;
	else return true;
}

bool ML_IsCollisionEx(const ML_Sprite *sprite, const ML_Sprite *sprite2)
{
	int sp1_left = sprite->x;
	int sp1_right = sprite->x + sprite->width*sprite->scaleX;
	int sp1_up = sprite->y;
	int sp1_down = sprite->y + sprite->height*sprite->scaleY;
	
	int sp2_left = sprite2->x;
	int sp2_right = sprite2->x + sprite2->width*sprite2->scaleX;
	int sp2_up = sprite2->y;
	int sp2_down = sprite2->y + sprite2->height*sprite2->scaleY;
	
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
				offset = ((((y + posY_spr1) >> 2)<<4)*sprite->width) + (((x+posX_spr1) >> 2)<<6) + ((((y+posY_spr1)%4 << 2) + (x+posX_spr1)%4 ) << 1); 
				a1 = *(sprite->data+offset);
				
				offset = ((((y + posY_spr2) >> 2)<<4)*sprite2->width) + (((x+posX_spr2) >> 2)<<6) + ((((y+posY_spr2)%4 << 2) + (x+posX_spr2)%4 ) << 1); 
				a2 = *(sprite2->data+offset);
				
				// Teste si les deux pixels ne sont pas transparents
				if(a1 != 0x00 && a2 != 0x00)
					return true;
			}
		}
		return false;
	}
}

void ML_Cursor(ML_Sprite *sprite, u8 wpad)
{
	ML_MoveSpriteWiimoteIR(sprite, wpad);
	ML_DrawSprite(sprite);
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

