#include "deflate.c.h"
#include <string.h>
#include <ks/defs.h>
#include <ks/log.h>
#include <stdlib.h>

static uint     g_bitoffset;
static uchar*   g_dst;
static uchar*   g_hashtable[8192];

static void out(uint data, uint len)
{
	uint *dst = (uint *)(g_dst + (g_bitoffset >> 3));
	uint bo = (g_bitoffset & 0x7);
	*dst = ((*dst) & ((1 << bo) - 1)) | (data << bo);
	g_bitoffset += len;
}

static uint deflate(uchar *source, uchar *destination, uint bitoffset, uint size, uint last)
{
	static uint i;

	uchar *src = source;
	uchar *last_source_byte = source + size - 1;

	g_dst = destination;
	g_bitoffset = bitoffset;

	out(last, 1); // last block?
	out(1, 2); // block type 01

    deflate_init_all_cache();

	if(size > 0)
	{
        out(static_huffman_cache->code_bits[*src], static_huffman_cache->code_nbit[*src]);
        src++;
	}

	while(src <= last_source_byte)
	{
		uint hash, fetch;
		uchar *o;
		
		fetch = *((uint *)src);
		hash = ((fetch >> 11) ^ fetch) & 0x1fff;
		o = g_hashtable[hash];
		g_hashtable[hash] = src;
		
		if(o >= src - 32768 && o < src && o > source && src < last_source_byte - 1 && ((fetch^(*(uint*)o)) & 0xffffff) == 0)
		{
			uint n = 3;
			while(src + n <= last_source_byte && *(o + n) == *(src + n) && n < 257)
				n++;
				
            out(length_cache->code_bits[n], length_cache->code_nbit[n]);
            out(offset_cache->code_bits[(uint)(src - o)], offset_cache->code_nbit[(uint)(src - o)]);

			src += n;
		}
		else
		{
            out(static_huffman_cache->code_bits[*src], static_huffman_cache->code_nbit[*src]);
			src++;
		}
	}
	
	out(0, 7); // end of block

	return g_bitoffset;
}

char* zip_deflate_compress(const char* data, int sz, int* ret_sz)
{
    int bits;
    char* dst;
    
    dst  = calloc(1, sz * 2);
    bits = deflate((uchar*)data, (uchar*)&dst[4], 0, sz, 0);

    *((int*)dst) = sz;
    *ret_sz = (bits + 8 - 1) / 8 + 4;

    return dst;
}