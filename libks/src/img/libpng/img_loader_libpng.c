#include "img_loader_libpng.h"
#include <png.h>
#include <stdlib.h>

#define PNG_DEBUG 3

int x, y;

int width, height, rowbytes;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

static void read_png_file(const char* file_name)
{
    unsigned char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp)
        printf("[read_png_file] File %s could not be opened for reading", file_name);
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8))
        printf("[read_png_file] File %s is not recognized as a PNG file", file_name);

    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        printf("[read_png_file] png_create_read_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        printf("[read_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        printf("[read_png_file] Error during init_io");

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);


    /* read file */
    if (setjmp(png_jmpbuf(png_ptr)))
        printf("[read_png_file] Error during read_image");

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);

    if (bit_depth == 16)
        rowbytes = width*8;
    else
        rowbytes = width*4;

    for (y=0; y<height; y++)
        row_pointers[y] = (png_byte*) malloc(rowbytes);

    png_read_image(png_ptr, row_pointers);

    fclose(fp);
}


char* ks_img_loader_libpng_load(const char* file, int* w, int* h)
{
    read_png_file(file);

    *w = width;
    *h = height;

    return NULL;
}