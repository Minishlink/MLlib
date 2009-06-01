#include <MLlib.h>

/**
* \file
* \brief This file contains draw functions.
*/

GXColor RGB_u32_u8 (u32 color) 
{
	u8 a, r, g, b;

	a = (color >> 24) & 0xFF; 
	r = (color >> 16) & 0xFF; 
	g = (color >> 8) & 0xFF; 
	b = (color) & 0xFF; 

	return (GXColor){r, g, b, a};
}

u16 RGB_888_565(u8 r, u8 g, u8 b) 
{
	return ((r >> 3) | ((g >> 2)<<5) | ((b >> 3)<<11)); 
}

u16 BGR_RGB(u32 bgr)
{
	return (((bgr & 31)<<11) | (((bgr>>5) & 63)<<5) | ((bgr>>11) & 31));
}


u32 RGB_888_565_2(u32 colorin)
{
	u8 r,g,b;
	
	r = (((colorin >> 16) & 0xFF)*31)/255;
	g = (((colorin >> 8) & 0xFF)*63)/255;
	b = (((colorin) & 0xFF)*31)/255;

	return ((r) << 11 | (g) << 5 | (b));	
}

//---------------------------------------------

u32 RGB2GC(u32 rgb)
{
	u8 r1 = (rgb&0xff000000) >> 24;
	u8 g1 = (rgb&0xff0000) >> 16;
	u8 b1 = (rgb&0xff00) >> 8;     
	u8 r2 = r1;
	u8 g2 = g1;
	u8 b2 = b1;
	int y1, cb1, cr1, y2, cb2, cr2, cb, cr;

	y1 = (299 * r1 + 587 * g1 + 114 * b1) / 1000;
	cb1 = (-16874 * r1 - 33126 * g1 + 50000 * b1 + 12800000) / 100000;
	cr1 = (50000 * r1 - 41869 * g1 - 8131 * b1 + 12800000) / 100000;

	y2 = (299 * r2 + 587 * g2 + 114 * b2) / 1000;
	cb2 = (-16874 * r2 - 33126 * g2 + 50000 * b2 + 12800000) / 100000;
	cr2 = (50000 * r2 - 41869 * g2 - 8131 * b2 + 12800000) / 100000;

	cb = (cb1 + cb2) >> 1;
	cr = (cr1 + cr2) >> 1;

	return (y1 << 24) | (cb << 16) | (y2 << 8) | cr;
}

u32 RGB2GC_2(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2)
{
	int y1, cb1, cr1, y2, cb2, cr2, cb, cr;

	y1 = (299 * r1 + 587 * g1 + 114 * b1) / 1000;
	cb1 = (-16874 * r1 - 33126 * g1 + 50000 * b1 + 12800000) / 100000;
	cr1 = (50000 * r1 - 41869 * g1 - 8131 * b1 + 12800000) / 100000;

	y2 = (299 * r2 + 587 * g2 + 114 * b2) / 1000;
	cb2 = (-16874 * r2 - 33126 * g2 + 50000 * b2 + 12800000) / 100000;
	cr2 = (50000 * r2 - 41869 * g2 - 8131 * b2 + 12800000) / 100000;

	cb = (cb1 + cb2) >> 1;
	cr = (cr1 + cr2) >> 1;

	return (y1 << 24) | (cb << 16) | (y2 << 8) | cr;
}

u32 FLIP32(u32 value)
{
	u32 b;

	b = (value & 0xff) << 24;
	b |= (value & 0xff00) << 8;
	b |= (value & 0xff0000) >> 8;
	b |= (value & 0xff000000) >> 24;

	return b;
}

u16 FLIP16(u16 value)
{
	return ((value & 0xff) << 8) | ((value & 0xff00) >> 8);
}

// that's from PNGU lib :) A little modifed though
/*int _PNGU_EncodeFromYCbYCr(IMGCTX ctx, PNGU_u32 width, PNGU_u32 height, void *buffer, PNGU_u32 stride)
{
	png_uint_32 rowbytes;
	PNGU_u32 x, y, buffWidth;

	// Erase from the context any readed info
	pngu_free_info (ctx);
	ctx->propRead = 0;

	// Check if the user has selected a file to write the image
	if (ctx->source == PNGU_SOURCE_BUFFER);	

	else if (ctx->source == PNGU_SOURCE_DEVICE)
	{
		// Open file
		if (!(ctx->fd = fopen (ctx->filename, "wb")))
			return PNGU_CANT_OPEN_FILE;
	}

	else
		return PNGU_NO_FILE_SELECTED;

	// Allocation of libpng structs
	ctx->png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!(ctx->png_ptr))
	{
		if (ctx->source == PNGU_SOURCE_DEVICE)
			fclose (ctx->fd);
        return PNGU_LIB_ERROR;
	}

    ctx->info_ptr = png_create_info_struct (ctx->png_ptr);
    if (!(ctx->info_ptr))
    {
		png_destroy_write_struct (&(ctx->png_ptr), (png_infopp)NULL);
		if (ctx->source == PNGU_SOURCE_DEVICE)
			fclose (ctx->fd);
        return PNGU_LIB_ERROR;
    }

	if (ctx->source == PNGU_SOURCE_BUFFER)
	{
		// Installation of our custom data writer function
		ctx->cursor = 0;
		png_set_write_fn (ctx->png_ptr, ctx, pngu_write_data_to_buffer, pngu_flush_data_to_buffer);
	}
	else if (ctx->source == PNGU_SOURCE_DEVICE)
	{
		// Default data writer uses function fwrite, so it needs to use our FILE*
		png_init_io (ctx->png_ptr, ctx->fd);
	}

	// Setup output file properties
    png_set_IHDR (ctx->png_ptr, ctx->info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, 
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	// Allocate memory to store the image in RGB format
	rowbytes = width * 3;
	if (rowbytes % 4)
		rowbytes = ((rowbytes / 4) + 1) * 4; // Add extra padding so each row starts in a 4 byte boundary

	ctx->img_data = malloc (rowbytes * height);
	if (!ctx->img_data)
	{
		png_destroy_write_struct (&(ctx->png_ptr), (png_infopp)NULL);
		if (ctx->source == PNGU_SOURCE_DEVICE)
			fclose (ctx->fd);
		return PNGU_LIB_ERROR;
	}

	ctx->row_pointers = malloc (sizeof (png_bytep) * height);
	if (!ctx->row_pointers)
	{
		png_destroy_write_struct (&(ctx->png_ptr), (png_infopp)NULL);
		if (ctx->source == PNGU_SOURCE_DEVICE)
			fclose (ctx->fd);
		return PNGU_LIB_ERROR;
	}

	// Encode YCbYCr image into RGB8 format
	buffWidth = (width + stride) / 2;
	for (y = 0; y < height; y++)
	{
		ctx->row_pointers[y] = ctx->img_data + (y * rowbytes);

		for (x = 0; x < (width / 2); x++)
			PNGU_YCbYCr_TO_RGB8 ( ((PNGU_u32 *)buffer)[y*buffWidth+x], 
				((PNGU_u8 *) ctx->row_pointers[y]+x*6), ((PNGU_u8 *) ctx->row_pointers[y]+x*6+1),
				((PNGU_u8 *) ctx->row_pointers[y]+x*6+2), ((PNGU_u8 *) ctx->row_pointers[y]+x*6+3),
				((PNGU_u8 *) ctx->row_pointers[y]+x*6+4), ((PNGU_u8 *) ctx->row_pointers[y]+x*6+5) );
	}

	// Tell libpng where is our image data
	png_set_rows (ctx->png_ptr, ctx->info_ptr, ctx->row_pointers);

	// Write file header and image data
	png_write_png (ctx->png_ptr, ctx->info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	// Tell libpng we have no more data to write
	png_write_end (ctx->png_ptr, (png_infop) NULL);

	// Free resources
	free (ctx->img_data);
	free (ctx->row_pointers);
	png_destroy_write_struct (&(ctx->png_ptr), &(ctx->info_ptr));
	if (ctx->source == PNGU_SOURCE_DEVICE)
		fclose (ctx->fd);

	// Success
	return PNGU_OK;
}
*/

