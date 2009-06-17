#ifndef DEF_ML_SPRITES_H
#define DEF_ML_SPRITES_H

/**
* \file
* \brief This file contains sprites headers.
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

inline void _initSprite(ML_Sprite *sprite);

/**
* \fn void ML_CloneSprite(ML_Sprite *sprite1, ML_Sprite *sprite2)
* \brief This function clones two sprites.
* @param sprite1 The original sprite
* @param sprite2 The sprite which will be the same as sprite1
*/
extern void ML_CloneSprite(ML_Sprite *sprite1, ML_Sprite *sprite2);

/**
* \fn bool ML_IsSpriteVisible(ML_Sprite *sprite)
* \brief This function checks if the sprite is visible.
* @param sprite Sprite
* @return 1 if it's visible, 0 else.
*/
extern bool ML_IsSpriteVisible(ML_Sprite *sprite);

/**
* \fn void ML_AnimateSprite(ML_Sprite *sprite, bool enabled, u8 waitForXRefreshBetweenFrames)
* \brief This function animates the sprite which needs to be tiled (and initialized for that).
* @param sprite Sprite
* @param enabled Animation enabled (1) or disabled (0)
* @param waitForXRefreshBetweenFrames This is the last of time between each frames.
*/
extern void ML_AnimateSprite(ML_Sprite *sprite, bool enabled, u8 waitForXSecondsBetweenFrames);

/**
* \fn void ML_AnimateSpriteEx(ML_Sprite *sprite, bool enabled, u8 waitForXRefreshBetweenFrames, u8 from, u8 to)
* \brief This function animates the sprite which needs to be tiled (and initialized for that). It will animate from "from" to "to" in loops.
* @param sprite Sprite
* @param enabled Animation enabled (1) or disabled (0)
* @param waitForXRefreshBetweenFrames This is the last of time between each frames.
* @param from From where the sprite will begin the animation
* @param to To where the sprite will finish the animations
*/
extern void ML_AnimateSpriteEx(ML_Sprite *sprite, bool enabled, u8 waitForXRefreshBetweenFrames, u8 from, u8 to);

/**
* \fn void ML_MoveSpriteWiimotePad(ML_Sprite *sprite, u8 wpad)
* \brief This function moves the sprite with the D-Pad of the Wiimote.
* @param sprite Sprite
* @param wpad Which Wiimote
*/
extern void ML_MoveSpriteWiimotePad(ML_Sprite *sprite, u8 wpad);

/**
* \fn void ML_MoveSpriteWiimoteIR(ML_Sprite *sprite, u8 wpad)
* \brief This function moves the sprite with Wiimote IR.
* @param sprite Sprite
* @param wpad Which Wiimote
*/
extern void ML_MoveSpriteWiimoteIR(ML_Sprite *sprite, u8 wpad);

/**
* \fn bool ML_IsWiimoteInSprite(u8 wpad, ML_Sprite *sprite)
* \brief This function checks if the Wiimote pointer is in the sprite.
* @param wpad Wiimote number
* @param sprite Sprite
* @return 1 if it's in the sprite, 0 else.
*/
extern bool ML_IsWiimoteInSprite(u8 wpad, ML_Sprite *sprite);

/**
* \fn bool ML_IsCollision(ML_Sprite *sprite, ML_Sprite *sprite2)
* \brief This function checks if there is a collision between two sprites. It's a box-detection.
* @param sprite 1st Sprite
* @param sprite2 2nd Sprite
* @return 1 if there is collision, 0 else.
*/
extern bool ML_IsCollision(const ML_Sprite *sprite, const ML_Sprite *sprite2);

/**
* \fn bool ML_IsCollisionEx(ML_Sprite *sprite, ML_Sprite *sprite2)
* \brief This function checks if there is a collision between two sprites. It's a pixel-detection.
* @param sprite 1st Sprite
* @param sprite2 2nd Sprite
* @return 1 if there is collision, 0 else.
*/
extern bool ML_IsCollisionEx(const ML_Sprite *sprite, const ML_Sprite *sprite2);

/**
* \fn void ML_Cursor(ML_Sprite *sprite, u8 wpad)
* \brief This function draws the sprite as a cursor. Wiimote pointer will be in the center of the sprite.
* @param sprite Sprite
* @param wpad Which Wiimote
*/
extern void ML_Cursor(ML_Sprite *sprite, u8 wpad);

/**
* \fn void ML_RotateSprite(ML_Sprite *sprite, float angle, u8 autoRotate)
* \brief This function rotates the sprite.
* @param sprite Sprite
* @param angle Angle of rotation (degrees)
* @param autoRotate If TRUE, the sprite will rotate of <angle> degrees each frame. If it's FALSE, the sprite will rotate to the angle indicated.
*/
extern void ML_RotateSprite(ML_Sprite *sprite, float angle, u8 autoRotate);

/**
* \fn void ML_SetSpriteXY(ML_Sprite *sprite, int x, int y)
* \brief This function changes the X and Y positions of the sprite.
* @param sprite Sprite
* @param x New X position
* @param y New Y position
*/
extern void ML_SetSpriteXY(ML_Sprite *sprite, int x, int y);

/**
* \fn void ML_SetSpriteScale(ML_Sprite *sprite, float scaleX, float scaleY)
* \brief This function changes the Scale X and Scale Y of the sprite.
* @param sprite Sprite
* @param scaleX New scale for the width of the sprite
* @param scaleY New scale for the height of the sprite
*/
extern void ML_SetSpriteScale(ML_Sprite *sprite, float scaleX, float scaleY);

/**
* \fn void ML_SetSpriteX(ML_Sprite *sprite, int x)
* \brief This function changes the X position of the sprite.
* @param sprite Sprite
* @param x New X position
*/
extern void ML_SetSpriteX(ML_Sprite *sprite, int x);

/**
* \fn void ML_SetSpriteY(ML_Sprite *sprite, int y)
* \brief This function changes the Y position of the sprite.
* @param sprite Sprite
* @param y New Y position
*/
extern void ML_SetSpriteY(ML_Sprite *sprite, int y);

/**
* \fn void ML_SetSpriteSize(ML_Sprite *sprite, u16 width, u16 height)
* \brief This function changes the size of your sprite.
* @param sprite Sprite
* @param width New width
* @param height New height
*/
extern void ML_SetSpriteSize(ML_Sprite *sprite, u16 width, u16 height);

/**
* \fn void ML_SetSpriteVelocity(ML_Sprite *sprite, int dx, int dy)
* \brief This function set the velocity members of the sprite.
* @param sprite Sprite
* @param dx New horizontal velocity
* @param dy New vertical velocity
*/
extern void ML_SetSpriteVelocity(ML_Sprite *sprite, int dx, int dy);

/**
* \fn void ML_SetSpriteAlpha(ML_Sprite *sprite, u8 alpha)
* \brief This function set the transparency of the sprite.
* @param sprite Sprite
* @param alpha Transparency ( 0 -> 255 )
*/
extern void ML_SetSpriteAlpha(ML_Sprite *sprite, u8 alpha);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

