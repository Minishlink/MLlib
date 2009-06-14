#include <MLlib.h>

static u32 __image_load_pos = 0;
static void __image_load_buffer(png_structp png_ptr, png_bytep data, png_size_t length)
{
		unsigned char* buffer= (unsigned char*)png_get_io_ptr(png_ptr);
		memcpy(data, buffer + __image_load_pos, length);
		__image_load_pos += length;
}

// THANKS A LOT CHAOSTEIL ! :D
bool read_png_gx_file(char* file_name, ML_Image *image)
{
    u32 x = 0, y = 0;
    int r = 0;
    
    png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep *row_pointers;

	char header[8]; // Check for support PNG header.
	FILE* fp = NULL;

	fp = fopen((const char*)file_name, "rb");
	if(!fp)
		return 0;

	if(fread(header, 1, 8, fp)!=8){
		fclose(fp);
		return 0;
	}

	if(png_sig_cmp((png_bytep)header, 0, 8)){
		fclose(fp);
		return 0;
	}

	// Create a PNG handle
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if(!png_ptr){
		fclose(fp);
		return 0;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}

	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	image->width = info_ptr->width;
	image->height = info_ptr->height;
	if(image->width % 4 != 0 || image->height % 4 != 0){
		fclose(fp);
		return 0;
	}
	color_type = info_ptr->color_type;
	bit_depth = info_ptr->bit_depth;
	if(bit_depth!=8){
		fclose(fp);
		return 0;
	}

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	// Read File
	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return 0;
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

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * image->height);
	if(!row_pointers){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return 0;
	}
	for(y = 0; y < image->height; y++)
		row_pointers[y] = (png_byte*)malloc(info_ptr->rowbytes);
	
	png_read_image(png_ptr, row_pointers);

	fclose(fp);

	if(image->data)
		free(image->data); image->data = NULL;

	image->data = (u8*)(memalign(32, image->width * image->height * 4));
	
	// THANKS DHEWG!! My first born is yours.
	u8 *d = (u8*)(image->data);
	u8 *s = NULL;
	if(color_type == PNG_COLOR_TYPE_RGBA){ // 32bit
		for (y = 0; y < image->height; y += 4) {
			for (x = 0; x < image->width; x += 4) {
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
		for (y = 0; y < image->height; y += 4) {
			for (x = 0; x < image->width; x += 4) {
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
	
	// Move flush cached memory.
	DCFlushRange(image->data, image->width * image->height * 4);
	
	// Free up some memory
	if(row_pointers){
		for(y = 0; y < image->height; y++){
			free(row_pointers[y]); row_pointers[y] = NULL;
		}
		free(row_pointers); row_pointers = NULL;
	}	
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    
    return 1;
}

bool read_png_gx_file_buffer(const u8* file_name, ML_Image *image) 
{
 	u32 x = 0, y = 0;
    int r = 0;
    
    png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep *row_pointers;

	char header[8]; // Check for support PNG header.
	void* buffer = (void*)file_name;

	memcpy(header, buffer, 8);
	if(png_sig_cmp((png_bytep)header, 0, 8)){
		return 0;
	}

	if(png_sig_cmp((png_bytep)header, 0, 8)){
		return 0;
	}

	// Create a PNG handle
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr){
		return 0;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return 0;
	}

	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return 0;
	}

	__image_load_pos = 0;
	png_set_read_fn(png_ptr, buffer, __image_load_buffer);

	png_read_info(png_ptr, info_ptr);

	image->width = info_ptr->width;
	image->height = info_ptr->height;
	if(image->width % 4 != 0 || image->height % 4 != 0){
		return 0;
	}
	color_type = info_ptr->color_type;
	bit_depth = info_ptr->bit_depth;
	if(bit_depth!=8){
		return 0;
	}

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	// Read File
	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return 0;
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

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * image->height);
	if(!row_pointers){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return 0;
	}
	for(y = 0; y < image->height; y++)
		row_pointers[y] = (png_byte*)malloc(info_ptr->rowbytes);
	
	png_read_image(png_ptr, row_pointers);

	if(image->data)
		free(image->data); image->data = NULL;

	image->data = (u8*)(memalign(32, image->width * image->height * 4));
	
	// THANKS DHEWG!! My first born is yours. <-- Thanks you too !
	u8 *d = (u8*)(image->data);
	u8 *s = NULL;
	if(color_type == PNG_COLOR_TYPE_RGBA){ // 32bit
		for (y = 0; y < image->height; y += 4) {
			for (x = 0; x < image->width; x += 4) {
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
		for (y = 0; y < image->height; y += 4) {
			for (x = 0; x < image->width; x += 4) {
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
	
	// Move flush cached memory.
	DCFlushRange(image->data, image->width * image->height * 4);
	
	// Free up some memory
	if(row_pointers){
		for(y = 0; y < image->height; y++){
			free(row_pointers[y]); row_pointers[y] = NULL;
		}
		free(row_pointers); row_pointers = NULL;
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    
    return 1;
}
	
