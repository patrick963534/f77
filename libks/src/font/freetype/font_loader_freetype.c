#include "font_loader_freetype.h"
#include <ks/log.h>
#include <ks/helper.h>
#include <ks/list.h>
#include <ks/libc.h>
#include <stdlib.h>
#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_TYPES_H

typedef struct cached_face_t
{
    FT_Face     face;
    char*       font_file;
    int         font_size;
    ks_list_t   item;
    
} cached_face_t;

static ks_list_t head = {&head, &head};
static FT_Library library;

static void init_library()
{
    static int is_init_library = 0;

    if (!is_init_library)
        FT_Init_FreeType(&library);

    is_init_library = 1;
}

static FT_Face create_face(const char* font_file, int font_size)
{
    FT_Face face;
    char    buf[FILENAME_MAX];
    int error;

    init_library();

    ks_helper_path_join_relative_app(buf, sizeof(buf), font_file);
    error = FT_New_Face(library, buf, 0, &face);
    if (error) ks_log("Fail to load font: %s", font_file);

    error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    if (error) ks_log("Fail to select charmap for font: %s", font_file);

    error = FT_Set_Pixel_Sizes(face, font_size, font_size);
    if (error) ks_log("Fail to set font size %d for font: %s", font_size, font_file);

    return face;
}

static cached_face_t* get_face(const char* font_file, int font_size)
{
    cached_face_t *pos;

    ks_list_for_each_entry(pos, &head, cached_face_t, item)
    {
        if (pos->font_size == font_size &&
            strcmp(pos->font_file, font_file) == 0)
        {
            return pos;
        }
    }

    pos = calloc(1, sizeof(*pos));
    pos->font_size = font_size;
    pos->font_file = ks_strdup(font_file);
    pos->face = create_face(font_file, font_size);
    ks_list_init(&pos->item);
    ks_list_add(&head, &pos->item);

    return pos;
}

static char* render_text(FT_Face face, const int* text, int* ret_w, int* ret_h)
{
    FT_Bitmap* bitmap;
    int* pixels;
    int offx = 0;
    int error;
    int i, j;

    int width = 320;
    int height = 240;
    
    pixels = (int*)calloc(1, width * height * 4);

    error = FT_Load_Glyph(face, FT_Get_Char_Index(face, *text), FT_LOAD_RENDER);

    bitmap = &face->glyph->bitmap;

    for (i = 0; i < bitmap->rows; ++i) 
    {
        for (j = 0; j < bitmap->width; ++j) 
        {
            int iY = (face->size->metrics.ascender - face->glyph->metrics.horiBearingY) >> 6;
            int iX = face->glyph->metrics.horiBearingX >> 6;
            int idx = (i + iY) * width + j + iX + offx;
            int bp = bitmap->buffer[i * bitmap->width + j];

            pixels[idx] = bp << 24 | bp << 16 | bp << 8 | bp;
        }
    }

    *ret_w = width;
    *ret_h = height;

    return (char*)pixels;
}

void so_font_loader_freetype_load(const char* font_file, int font_size, const char* text, so_font_loader_data_t* info)
{
    cached_face_t* cf;
    int* ucs_text;
    int length;

    length = ks_u8_lenth(text);
    ucs_text = malloc(length);
    ks_u8_to_ucs(ucs_text, length, text, strlen(text));

    cf = get_face(font_file, font_size);
    info->pixels = render_text(cf->face, ucs_text, &info->width, &info->height);    

    ks_helper_image_save_ppm("E:\\font_text.ppm", info->pixels, info->width, info->height);
}
