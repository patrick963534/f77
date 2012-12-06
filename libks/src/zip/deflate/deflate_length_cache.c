#include "deflate.c.h"

#define LENTH_COUNT    258

static void get_length_code(uint matchlen, int* bits, int* nbit)
{
	uint extra, i, code;
    static const unsigned short lens[29] = {
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};
    static const unsigned short lext[29] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
        
	static const unsigned short matchlens[11] = {0, 0, 0, 64, 32, 96, 16, 80, 48, 112, 8};

	if(matchlen <= 10)
	{
        *bits = matchlens[matchlen];
        *nbit = 7;
	}
	else
	{
		for(i = 0; i < 29 && matchlen >= lens[i]; i++);
		i--;
		code = i + 254 + 3; // 257...285 i1 = CODE
		
		if(code < 280)
		{
            extra = matchlen - lens[i];

			code = revbits(code, 7);
			code |= (extra << 7);

            *bits = code;
            *nbit = 7 + lext[i];
		}
		else
		{
            extra = matchlen - lens[i];	

			code = code + 192 - 280;
			code = revbits(code, 8);
			code |= (extra << 8);

            *bits = code;
            *nbit = 8 + lext[i];
		}
	}
}

void deflate_init_length_cache()
{
    if (length_cache != NULL)
        return;

    length_cache = deflate_generate_cache(LENTH_COUNT, 10240, get_length_code);
}