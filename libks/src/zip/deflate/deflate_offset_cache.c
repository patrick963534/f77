#include "deflate.c.h"
#include <assert.h>

#define OFFSET_COUNT    32769

static void get_offset_code(unsigned int offset, int* bits, int* nbit)
{
	static uint o;
	static uint is_init = 0;
	static uint offset_cache[32769];

	if (!is_init)
	{
		static const unsigned short dists[30] = {
			1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
			257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
			8193, 12289, 16385, 24577};
		static const unsigned short dext[30] = {
			0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
			7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
			12, 12, 13, 13};
	        
   		uint i, extra, i2, offset;
		for(offset = 1; offset <= 32768; offset++)
		{
	   		for(i = 0; i < 30 && offset >= dists[i]; i++);
			i--;

			i2 = revbits(i, 5);
			extra = offset - dists[i];
			i2 |= (extra << 5);	
		
			offset_cache[offset] = i2 | ((dext[i] + 5) << 24);
		}
		is_init = 1;
	}
	o = offset_cache[offset];
    *bits = o & 0xffffff;
    *nbit = o >> 24;
}

void deflate_init_offset_cache()
{
    if (offset_cache != NULL)
        return;

    offset_cache = deflate_generate_cache(OFFSET_COUNT,  400 * 1024, get_offset_code);
}