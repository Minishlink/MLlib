#ifndef DEF_ML_DRAW_H
#define DEF_ML_DRAW_H

/**
* \file
* \brief This file contains draw headers. (pictures conversions)
*/

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

extern GXColor RGB_u32_u8 (u32 color);
extern u16 RGB_888_565(u8 r, u8 g, u8 b);
extern u16 BGR_RGB(u32 bgr);
extern u32 RGB_888_565_2(u32 colorin);

extern u32 RGB2GC(u32 rgb);
extern u32 RGB2GC_2(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2);
extern u32 FLIP32(u32 value);
extern u16 FLIP16(u16 value);
extern void _PNGU_YCbYCr_TO_RGB8(unsigned int ycbycr, unsigned char *r1, unsigned char *g1, unsigned char *b1, unsigned char *r2, unsigned char *g2, unsigned char *b2);
extern int _pngu_clamp (int value, int min, int max);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
