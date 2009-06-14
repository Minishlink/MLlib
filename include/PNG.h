#ifndef __PNG_H__
#define __PNG_H__

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/*typedef struct _png_file_t
{
	u16 width, height;
	png_byte color_type;
	int row_bytes;
	png_byte bit_depth;

	//png_bytep *data;
	png_bytep gxdata;
} png_file_t;
typedef png_file_t png_file_gx_t;*/

bool read_png_gx_file(char* file_name, ML_Image *image);
bool read_png_gx_file_buffer(const u8* file_name, ML_Image *image);

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
