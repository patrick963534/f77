#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "deflate_draft.h"

#define CHUNK_SIZE 100000
unsigned char src[CHUNK_SIZE];
unsigned char dst[2*CHUNK_SIZE];

int deflate_demo(int argc, char **argv)
{
	FILE *ifile, *ofile;
	unsigned int in, n, prev, a;
	char *filenamepart;

	if(argc < 2)
	{
		printf("Demo project for QuickLZ ZIP 0.3. Copyright 2007 Lasse Mikke Reinhold.\n\n");
		printf("To compress files:\n");
		printf("    demo <destination file> <source file 1> [source file 2] [source file 3] ...\n\n");
		printf("Example of compression:\n");
		printf("    demo test.zip image.png c:\\readme.txt ..\\hello.xml\n");
		return (-1);

	}

	ofile = fopen(argv[1], "wb");
	new_zip();
	
	for(a = 2; a < (unsigned int)argc; a++)
	{
		filenamepart = strrchr(argv[a], '\\');
		if(filenamepart == 0)
			filenamepart = strrchr(argv[a], '/');
		if(filenamepart == 0)
			filenamepart = argv[a];
		else
			filenamepart += 1;

		ifile = fopen(argv[a], "rb");
		prev = ftell(ofile);
		add_file(filenamepart);
		while(feof(ifile) == 0)	
		{
			in = fread(src, 1, CHUNK_SIZE, ifile);
			n = add_data((char*)src, (char*)dst, in, feof(ifile) == 0 ? 0 : 1);
			fwrite(dst, 1, n, ofile);
		}

		fseek(ofile, prev, SEEK_SET);
		n = end_file((char*)dst);
		fwrite(dst, 1, n, ofile);
		fseek(ofile, 0, SEEK_END);
		fclose(ifile);
	}

	n = end_zip(dst);
	fwrite(dst, 1, n, ofile);

	fclose(ofile);

    return 0;
}
