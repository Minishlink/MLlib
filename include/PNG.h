#ifndef __PNG_H__
#define __PNG_H__

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

typedef struct _png_file_t
{
	int width, height;
	png_byte color_type;
	int row_bytes;
	png_byte bit_depth;

	png_bytep *data;
	png_bytep gxdata;
} png_file_t;

typedef png_file_t png_file_gx_t;


png_file_gx_t read_png_gx_file(char* file_name);
png_file_gx_t read_png_gx_file_buffer(const u8* file_name);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
