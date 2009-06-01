#include <MLlib.h>

/**
* \file
* \brief This file contains sprites functions.
*/

void ML_CloneSprite(u16 nb, u16 cloned_nb)
{
	sprite[nb] = sprite[cloned_nb];
}

void ML_AnimateSprite(u16 nb, bool enabled, u8 waitForXRefreshBetweenFrames)
{
	if(sprite[nb].tiled)
	{
		sprite[nb].animated = enabled;
		sprite[nb].waitForXRefreshBetweenFrames = waitForXRefreshBetweenFrames;
	}
}

void ML_MoveSpriteWiimotePad(u16 nb, u8 wpad)
{
	if(Wiimote[wpad].Held.Right) sprite[nb].x += sprite[nb].dx;
	else if(Wiimote[wpad].Held.Left) sprite[nb].x -= sprite[nb].dx;
	if(Wiimote[wpad].Held.Down) sprite[nb].y += sprite[nb].dy;
	else if(Wiimote[wpad].Held.Up) sprite[nb].y -= sprite[nb].dy;
}

void ML_MoveSpriteWiimoteIR(u16 nb, u8 wpad)
{
	if(Wiimote[wpad].IR.Valid)
	{
		sprite[nb].x = (Wiimote[wpad].IR.X / screenMode->fbWidth * (screenMode->fbWidth + (sprite[nb].width*sprite[nb].scaleX) * 2)) - (sprite[nb].width*sprite[nb].scaleX);
		sprite[nb].y = (Wiimote[wpad].IR.Y / screenMode->xfbHeight * (screenMode->xfbHeight + (sprite[nb].height*sprite[nb].scaleY) * 2)) - (sprite[nb].height*sprite[nb].scaleY);
		
		sprite[nb].visible = true;
	} else sprite[nb].visible = false;
}

bool ML_IsWiimoteInSprite(u8 wpad, u16 nb)
{	
	int cursorX = (Wiimote[wpad].IR.X / screenMode->fbWidth * (screenMode->fbWidth + 2)) - 1;
	int cursorY = (Wiimote[wpad].IR.Y / screenMode->xfbHeight * (screenMode->xfbHeight + 2)) - 1;
	
	if(cursorX >= sprite[nb].x &&
	   cursorX <= sprite[nb].x + sprite[nb].width*sprite[nb].scaleX &&
	   cursorY >= sprite[nb].y &&
	   cursorY <= sprite[nb].y + sprite[nb].height*sprite[nb].scaleY
	  ) return true;
	  else return false;
}

bool ML_IsCollision(u16 nb, u16 nb2)
{
	int sp1_left = sprite[nb].x;
	int sp1_right = sprite[nb].x + sprite[nb].width*sprite[nb].scaleX;
	int sp1_up = sprite[nb].y;
	int sp1_down = sprite[nb].y + sprite[nb].height*sprite[nb].scaleY;
	
	int sp2_left = sprite[nb2].x;
	int sp2_right = sprite[nb2].x + sprite[nb2].width*sprite[nb2].scaleX;
	int sp2_up = sprite[nb2].y;
	int sp2_down = sprite[nb2].y + sprite[nb2].height*sprite[nb2].scaleY;
	
	if(sp1_left > sp2_right ||
	   sp1_right < sp2_left ||
	   sp1_up > sp2_down ||
	   sp1_down < sp2_up)
	   return false;
	else return true;
}

bool ML_IsCollisionEx(u16 nb, u16 nb2)
{
	int sp1_left = sprite[nb].x;
	int sp1_right = sprite[nb].x + sprite[nb].width*sprite[nb].scaleX;
	int sp1_up = sprite[nb].y;
	int sp1_down = sprite[nb].y + sprite[nb].height*sprite[nb].scaleY;
	
	int sp2_left = sprite[nb2].x;
	int sp2_right = sprite[nb2].x + sprite[nb2].width*sprite[nb2].scaleX;
	int sp2_up = sprite[nb2].y;
	int sp2_down = sprite[nb2].y + sprite[nb2].height*sprite[nb2].scaleY;
	
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
				offset = ((((y + posY_spr1) >> 2)<<4)*sprite[nb].width) + (((x+posX_spr1) >> 2)<<6) + ((((y+posY_spr1)%4 << 2) + (x+posX_spr1)%4 ) << 1); 
				a1 = *(sprite[nb].data+offset);
				
				offset = ((((y + posY_spr2) >> 2)<<4)*sprite[nb2].width) + (((x+posX_spr2) >> 2)<<6) + ((((y+posY_spr2)%4 << 2) + (x+posX_spr2)%4 ) << 1); 
				a2 = *(sprite[nb2].data+offset);
				
				// Teste si les deux pixels ne sont pas transparents
				if(a1 != 0x00 && a2 != 0x00)
					return true;
			}
		}
		return false;
	}
}

void ML_Cursor(u16 nb, u8 wpad)
{
	ML_MoveSpriteWiimoteIR(nb, wpad);
	ML_DrawSprite(nb);
}

void ML_RotateSprite(u16 nb, float angle, u8 autoRotate)
{
	sprite[nb].rotated = autoRotate;	
	if(sprite[nb].rotated)
	{
		sprite[nb].angle += angle;
	} else sprite[nb].angle = angle;
}

void ML_SetSpriteXY(u16 nb, int x, int y) { sprite[nb].x = x; sprite[nb].y = y; }
void ML_SetSpriteX(u16 nb, int x) { sprite[nb].x = x; }
void ML_SetSpriteY(u16 nb, int y) { sprite[nb].y = y; }
void ML_SetSpriteSize(u16 nb, u16 width, u16 height) { sprite[nb].width = width; sprite[nb].height = height; }
void ML_SetSpriteScale(u16 nb, float scaleX, float scaleY) { sprite[nb].scaleX = scaleX; sprite[nb].scaleY = scaleY; }
void ML_SetSpriteVelocity(u16 nb, int dx, int dy) { sprite[nb].x = dx; sprite[nb].dy = dy; }
void ML_SetSpriteAlpha(u16 nb, u8 alpha) { sprite[nb].alpha = alpha; }

