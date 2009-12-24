#ifndef DEF_MLLIB_H
#define DEF_MLLIB_H

/**
 * @mainpage MLlib Documentation
 * @image html MLlib_logo.png
 * Hi and welcome to the MLlib documentation !
 */
 
/**
* \file
* \brief This file is the "central-mother" of the lib.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include <network.h>
#include <errno.h>
#include <math.h>
#include <gccore.h>
#include <ogcsys.h>
#include <ogc/ios.h>
#include <ogc/ipc.h>
#include <mp3player.h>
#include <asndlib.h>
#include <gcmodplay.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <wchar.h>

// libpng ported to Wii by mavakadachi 
#include <png/png.h>

// FreeTypelib ported to Wii by Tantric
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H

// MLlib now
#include "ML_define.h"
#include "globals.h"

#include "ML_draw.h"
#include "ML_time.h"
#include "ML_init.h"
#include "ML_system.h"
#include "ML_video.h"
#include "ML_sprites.h"
#include "ML_input.h"
#include "ML_mp3player.h"
#include "ML_modplayer.h"
#include "ML_backgrounds.h"
#include "ML_images.h"
#include "ML_math.h"
#include "ML_font.h"
#include "PNG.h"

#endif

