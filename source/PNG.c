#include <MLlib.h>

static u32 __image_load_pos = 0;
static void __image_load_buffer(png_structp png_ptr, png_bytep data, png_size_t length)
{
		unsigned char* buffer= (unsigned char*)png_get_io_ptr(png_ptr);
		memcpy(data, buffer + __image_load_pos, length);
		__image_load_pos += length;
}

// THANKS A LOT CHAOSTEIL ! :D
png_file_gx_t read_png_gx_file(char* file_name)
{
    u32 x = 0, y = 0;
    int r = 0;
    u32 _width = 0, _height = 0;
    u8* _pixels = NULL;
    
    png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep *row_pointers;
	png_file_gx_t lol;

	char header[8]; // Check for support PNG header.
	FILE* fp = NULL;

	fp = fopen((const char*)file_name, "rb");
	if(!fp)
		return lol;

	if(fread(header, 1, 8, fp)!=8){
		fclose(fp);
		return lol;
	}

	if(png_sig_cmp((png_bytep)header, 0, 8)){
		fclose(fp);
		return lol;
	}

	// Create a PNG handle
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if(!png_ptr){
		fclose(fp);
		return lol;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return lol;
	}

	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return lol;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	_width = info_ptr->width;
	_height = info_ptr->height;
	if(_width % 4 != 0 || _height % 4 != 0){
		fclose(fp);
		return lol;
	}
	color_type = info_ptr->color_type;
	bit_depth = info_ptr->bit_depth;
	if(bit_depth!=8){
		fclose(fp);
		return lol;
	}

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	// Read File
	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return lol;
	}

	// Some helper functions to allow more pngs
	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_gray_1_2_4_to_8(png_ptr);
	if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	if(color_type == PNG_COLOR_TYPE_RGBA) // Swap RGBA to ARGB
		png_set_swap_alpha(png_ptr);

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * _height);
	if(!row_pointers){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return lol;
	}
	for(y = 0; y<_height; y++)
		row_pointers[y] = (png_byte*)malloc(info_ptr->rowbytes);
	
	png_read_image(png_ptr, row_pointers);

	fclose(fp);

	if(_pixels)
		free(_pixels); _pixels = NULL;

	_pixels = (u8*)(memalign(32, _width*_height*4));
	
	// THANKS DHEWG!! My first born is yours.
	u8 *d = (u8*)(_pixels);
	u8 *s = NULL;
	if(color_type == PNG_COLOR_TYPE_RGBA){ // 32bit
		for (y = 0; y < _height; y += 4) {
			for (x = 0; x < _width; x += 4) {
				for (r = 0; r < 4; ++r) {
					s = &row_pointers[y + r][x << 2];
					*d++ = s[0];  *d++ = s[1];
					*d++ = s[4];  *d++ = s[5];
					*d++ = s[8];  *d++ = s[9];
					*d++ = s[12]; *d++ = s[13];
				}
				for (r = 0; r < 4; ++r) {
					s = &row_pointers[y + r][x << 2];
					*d++ = s[2];  *d++ = s[3];
					*d++ = s[6];  *d++ = s[7];
					*d++ = s[10]; *d++ = s[11];
					*d++ = s[14]; *d++ = s[15];
				}
			}
		}
	}else if(color_type == PNG_COLOR_TYPE_RGB){ // 24bit
		for (y = 0; y < _height; y += 4) {
			for (x = 0; x < _width; x += 4) {
				for (r = 0; r < 4; ++r) {
					s = &row_pointers[y + r][x * 3];
					*d++ = 0xff;  *d++ = s[0];
					*d++ = 0xff;  *d++ = s[3];
					*d++ = 0xff;  *d++ = s[6];
					*d++ = 0xff; *d++ = s[9];
				}

				for (r = 0; r < 4; ++r) {
					s = &row_pointers[y + r][x * 3];
					*d++ = s[1];  *d++ = s[2];
					*d++ = s[4];  *d++ = s[5];
					*d++ = s[7]; *d++ = s[8];
					*d++ = s[10]; *d++ = s[11];
				}
			}
		}
	}
	
	png_file_gx_t png_filep;
	png_filep.width = _width;
	png_filep.height = _height;
	png_filep.color_type = color_type;
	png_filep.row_bytes = info_ptr->rowbytes;
	png_filep.bit_depth = bit_depth;
	png_filep.data = row_pointers;
	png_filep.gxdata = (void*)_pixels;
	
	// Move flush cached memory.
	DCFlushRange(_pixels, _width * _height * 4);
	
	// Free up some memory
	if(row_pointers){
		for(y = 0; y < _height; y++){
			free(row_pointers[y]); row_pointers[y] = NULL;
		}
		free(row_pointers); row_pointers = NULL;
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    
    return png_filep;
}

png_file_gx_t read_png_gx_file_buffer(const u8* file_name) 
{
 	u32 x = 0, y = 0;
    int r = 0;
    u32 _width = 0, _height = 0;
    u8* _pixels = NULL;
    
    png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep *row_pointers;
	png_file_gx_t lol;

	char header[8]; // Check for support PNG header.
	void* buffer = (void*)file_name;

	memcpy(header, buffer, 8);
	if(png_sig_cmp((png_bytep)header, 0, 8)){
		return lol;
	}

	if(png_sig_cmp((png_bytep)header, 0, 8)){
		return lol;
	}

	// Create a PNG handle
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr){
		return lol;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return lol;
	}

	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return lol;
	}

	__image_load_pos = 0;
	png_set_read_fn(png_ptr, buffer, __image_load_buffer);

	png_read_info(png_ptr, info_ptr);

	_width = info_ptr->width;
	_height = info_ptr->height;
	if(_width % 4 != 0 || _height % 4 != 0){
		return lol;
	}
	color_type = info_ptr->color_type;
	bit_depth = info_ptr->bit_depth;
	if(bit_depth!=8){
		return lol;
	}

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	// Read File
	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return lol;
	}

	// Some helper functions to allow more pngs
	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_gray_1_2_4_to_8(png_ptr);
	if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	if(color_type == PNG_COLOR_TYPE_RGBA) // Swap RGBA to ARGB
		png_set_swap_alpha(png_ptr);

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * _height);
	if(!row_pointers){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return lol;
	}
	for(y = 0; y<_height; y++)
		row_pointers[y] = (png_byte*)malloc(info_ptr->rowbytes);
	
	png_read_image(png_ptr, row_pointers);

	if(_pixels)
		free(_pixels); _pixels = NULL;

	_pixels = (u8*)(memalign(32, _width*_height*4));
	
	// THANKS DHEWG!! My first born is yours. <-- Thanks you too !
	u8 *d = (u8*)(_pixels);
	u8 *s = NULL;
	if(color_type == PNG_COLOR_TYPE_RGBA){ // 32bit
		for (y = 0; y < _height; y += 4) {
			for (x = 0; x < _width; x += 4) {
				for (r = 0; r < 4; ++r) {
					s = &row_pointers[y + r][x << 2];
					*d++ = s[0];  *d++ = s[1];
					*d++ = s[4];  *d++ = s[5];
					*d++ = s[8];  *d++ = s[9];
					*d++ = s[12]; *d++ = s[13];
				}
				for (r = 0; r < 4; ++r) {
					s = &row_pointers[y + r][x << 2];
					*d++ = s[2];  *d++ = s[3];
					*d++ = s[6];  *d++ = s[7];
					*d++ = s[10]; *d++ = s[11];
					*d++ = s[14]; *d++ = s[15];
				}
			}
		}
	}else if(color_type == PNG_COLOR_TYPE_RGB){ // 24bit
		for (y = 0; y < _height; y += 4) {
			for (x = 0; x < _width; x += 4) {
				for (r = 0; r < 4; ++r) {
					s = &row_pointers[y + r][x * 3];
					*d++ = 0xff;  *d++ = s[0];
					*d++ = 0xff;  *d++ = s[3];
					*d++ = 0xff;  *d++ = s[6];
					*d++ = 0xff; *d++ = s[9];
				}

				for (r = 0; r < 4; ++r) {
					s = &row_pointers[y + r][x * 3];
					*d++ = s[1];  *d++ = s[2];
					*d++ = s[4];  *d++ = s[5];
					*d++ = s[7]; *d++ = s[8];
					*d++ = s[10]; *d++ = s[11];
				}
			}
		}
	}
	
	png_file_gx_t png_filep;
	png_filep.width = _width;
	png_filep.height = _height;
	png_filep.color_type = color_type;
	png_filep.row_bytes = info_ptr->rowbytes;
	png_filep.bit_depth = bit_depth;
	png_filep.data = row_pointers;
	png_filep.gxdata = (void*)_pixels;
	
	// Move flush cached memory.
	DCFlushRange(_pixels, _width * _height * 4);
	
	// Free up some memory
	if(row_pointers){
		for(y = 0; y < _height; y++){
			free(row_pointers[y]); row_pointers[y] = NULL;
		}
		free(row_pointers); row_pointers = NULL;
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    
    return png_filep;
}
	
