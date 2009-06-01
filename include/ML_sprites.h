#ifndef DEF_ML_SPRITES_H
#define DEF_ML_SPRITES_H

/**
* \file
* \brief This file contains sprites headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/**
* \fn void ML_CloneSprite(u16 nb, u16 cloned_nb)
* \brief This function clones a sprite.
* @param nb New sprite number
* @param cloned_nb Cloned sprite number
*/
extern void ML_CloneSprite(u16 nb, u16 cloned_nb);

/**
* \fn void ML_AnimateSprite(u16 nb, bool enabled, u8 waitForXRefreshBetweenFrames)
* \brief This function animates the sprite which needs to be tiled (and initialized for that).
* @param nb Sprite number
* @param enabled Animation enabled (1) or disabled (0)
* @param waitForXRefreshBetweenFrames This is the last of time between each frames.
*/
extern void ML_AnimateSprite(u16 nb, bool enabled, u8 waitForXSecondsBetweenFrames);

/**
* \fn void ML_MoveSpriteWiimotePad(u16 nb, u8 wpad)
* \brief This function moves the sprite with the D-Pad of the Wiimote.
* @param nb Sprite number
* @param wpad Which Wiimote
*/
extern void ML_MoveSpriteWiimotePad(u16 nb, u8 wpad);

/**
* \fn void ML_MoveSpriteWiimoteIR(u16 nb, u8 wpad)
* \brief This function moves the sprite with Wiimote IR.
* @param nb Sprite number
* @param wpad Which Wiimote
*/
extern void ML_MoveSpriteWiimoteIR(u16 nb, u8 wpad);

extern bool ML_IsWiimoteInSprite(u8 wpad, u16 nb);

extern bool ML_IsCollision(u16 nb, u16 nb2);
extern bool ML_IsCollisionEx(u16 nb, u16 nb2);

/**
* \fn void ML_Cursor(u16 nb, u8 wpad)
* \brief This function draws the sprite as a cursor.
* @param nb Sprite number
* @param wpad Which Wiimote
*/
extern void ML_Cursor(u16 nb, u8 wpad);

/**
* \fn void ML_RotateSprite(u16 nb, float angle, u8 autoRotate)
* \brief This function rotates the sprite.
* @param nb Sprite number
* @param angle Angle of rotation (degrees)
* @param autoRotate If TRUE, the sprite will rotate of <angle> degrees each frame. If it's FALSE, the sprite will rotate to the angle indicated.
*/
extern void ML_RotateSprite(u16 nb, float angle, u8 autoRotate);

/**
* \fn void ML_SetSpriteXY(u16 nb, int x, int y)
* \brief This function changes the X and Y positions of the sprite.
* @param nb Sprite number
* @param x New X position
* @param y New Y position
*/
extern void ML_SetSpriteXY(u16 nb, int x, int y);

/**
* \fn void ML_SetSpriteScale(u16 nb, float scaleX, float scaleY)
* \brief This function changes the Scale X and Scale Y of the sprite.
* @param nb Sprite number
* @param scaleX New scale for the width of the sprite
* @param scaleY New scale for the height of the sprite
*/
extern void ML_SetSpriteScale(u16 nb, float scaleX, float scaleY);

/**
* \fn void ML_SetSpriteX(u16 nb, int x)
* \brief This function changes the X position of the sprite.
* @param nb Sprite number
* @param x New X position
*/
extern void ML_SetSpriteX(u16 nb, int x);

/**
* \fn void ML_SetSpriteY(u16 nb, int y)
* \brief This function changes the Y position of the sprite.
* @param nb Sprite number
* @param y New Y position
*/
extern void ML_SetSpriteY(u16 nb, int y);

/**
* \fn void ML_SetSpriteSize(u16 nb, u16 width, u16 height)
* \brief This function changes the size of your sprite.
* @param nb Sprite number
* @param width New width
* @param height New height
*/
extern void ML_SetSpriteSize(u16 nb, u16 width, u16 height);

/**
* \fn void ML_SetSpriteVelocity(u16 nb, int dx, int dy)
* \brief This function set the velocity members of the sprite.
* @param nb Sprite number
* @param dx New horizontal velocity
* @param dy New vertical velocity
*/
extern void ML_SetSpriteVelocity(u16 nb, int dx, int dy);

/**
* \fn void ML_SetSpriteAlpha(u16 nb, u8 alpha)
* \brief This function set the transparency of the sprite.
* @param nb Sprite number
* @param alpha Transparency ( 0 -> 255 )
*/
extern void ML_SetSpriteAlpha(u16 nb, u8 alpha);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

