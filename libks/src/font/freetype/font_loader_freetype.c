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
    int error;

    error = FT_New_Face(library, font_file, 0, &face);
    if (error) ks_log("Fail to load font: %s", font_file);

    error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    if (error) ks_log("Fail to select charmap for font: %s", font_file);

    error = FT_Set_Pixel_Sizes(face, font_size, font_size);
    if (error) ks_log("Fail to set font size %d for font: %s", font_size, font_file);
}

static cached_face_t* get_face(const char* font_file, int font_size)
{
    cached_face_t *pos;

    ks_list_for_each_entry(pos, &head, cached_face_t, item)
    {
        if (strcmp(pos->font_file, font_file) == 0)
        {
            return pos;
        }
    }

    pos = calloc(1, sizeof(*pos));
    pos->font_file = ks_strdup(font_file);
    pos->face = create_face(font_file, font_size);
    ks_list_init(&pos->item);
    ks_list_add(&head, &pos->item);

    return pos;
}

void so_font_loader_freetype_load(const char* font_file, int font_size, const char* text, so_font_loader_data_t* info)
{
    init_library();


    ks_unused(font_file);
    ks_unused(info);
}
