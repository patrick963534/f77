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

static uint revbits(uint n, int numbits)
{
	static const uchar rev5[32] = {
        0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 
        10, 26, 6, 22, 14, 30, 1, 17, 9, 25, 
        5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31
    };

	uint i, result = 0;
	if(numbits == 5)
		return rev5[n];
	else
	{
		for (i = numbits; i; --i)
		{
			result <<= 1;
			if (n & 1)
			result |= 1;
			n >>= 1;
		}
		return result;
	}
}

static void encode_offset(uint offset)
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
	out(o & 0xffffff, o >> 24); 
 
}

static void encode_literal(uint lit)
{
    out(encoding[lit].bits, encoding[lit].nbit);
}

static void encode_matchlen(uint matchlen)
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
		out(matchlens[matchlen], 7);
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
			out(code, 7 + lext[i]);
		}
		else
		{
            extra = matchlen - lens[i];	

			code = code + 192 - 280;
			code = revbits(code, 8);
			code |= (extra << 8);
			out(code, 8 + lext[i]);
		}
	}
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

	if(size > 0)
	{
		encode_literal(*src);
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
				
			encode_matchlen(n);
			encode_offset((uint)(src - o));
			src += n;
		}
		else
		{
			encode_literal(*src);
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