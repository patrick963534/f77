#include <stdio.h>
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_TYPES_H

#define CHECK_RETURN(ierror) if (ierror) goto fail;
#define WIDTH       800
#define HEIGHT      600
#define PIXEL_SZ    3

static void test(const char *text, int nWidth, int nHeight, const char * pFontName, int fontSize) 
{
    FT_Face face;
    FT_Error iError;
    FT_Library library;
    FT_Bitmap* bitmap;

    const char* pText = text;
    char* m_pData;
    int offx = 0;
    int i, j;

    int iCurXCursor, iCurYCursor;
    int bRet = 0;

    FT_Init_FreeType(&library);

    iError = FT_New_Face(library, pFontName, 0, &face);
    CHECK_RETURN(iError);

    iError = FT_Select_Charmap(face,FT_ENCODING_UNICODE);
    CHECK_RETURN(iError);

    iError = FT_Set_Pixel_Sizes(face, fontSize, fontSize);
    CHECK_RETURN(iError);

    {
        int tt_width = 0;
        iError = FT_Load_Glyph(face, FT_Get_Char_Index(face, *pText), FT_LOAD_DEFAULT);
        tt_width = face->glyph->metrics.width;
        tt_width = face->glyph->metrics.width;
    }    

    m_pData = calloc(1, WIDTH * HEIGHT * PIXEL_SZ);

    while (*pText != 0)
    {
        iError = FT_Load_Glyph(face, FT_Get_Char_Index(face, *pText), FT_LOAD_RENDER);
        CHECK_RETURN(iError);

        bitmap = &face->glyph->bitmap;

        for (i = 0; i < bitmap->rows; ++i) 
        {
            for (j = 0; j < bitmap->width; ++j) 
            {
                //  if it has gray>0 we set show it as 1, o otherwise
                int iY = (face->size->metrics.ascender - face->glyph->metrics.horiBearingY) >> 6; //iCurYCursor + i - (face->glyph->metrics.horiBearingY >> 6);
                int iX = face->glyph->metrics.horiBearingX >> 6;
                int iTemp = 0;
                int idx = ((i + iY) * WIDTH + j + iX + offx ) * PIXEL_SZ;
                int k;

                unsigned char cTemp = bitmap->buffer[i * bitmap->width + j];
                iTemp |= (cTemp ? 0xff : 0)<<24;
                iTemp |= cTemp << 16 | cTemp << 8 | cTemp;

                for (k = 0; k < PIXEL_SZ; k++)
                {
                    m_pData[idx + k] = bitmap->buffer[i * bitmap->width + j];                    m_pData[idx + k] = bitmap->buffer[i * bitmap->width + j];                    m_pData[idx + k] = bitmap->buffer[i * bitmap->width + j];                }
            }
        }

        offx += (face->glyph->metrics.horiBearingX + face->glyph->metrics.width) >> 6;

        pText++;
    }

    {
        FILE* save_fp = fopen("E:\\ft_result.ppm", "wb");
        fprintf(save_fp, "P6 %d %d 255 ", WIDTH, HEIGHT);
        fwrite(m_pData, 1, WIDTH * HEIGHT * PIXEL_SZ, save_fp);
        fclose(save_fp);
    }

    return;

fail:
    printf("test fail.");
}

int main()
{
    test("Hello World", 20, 20, "E:\\arial.ttf", 30);
    return 0;
}