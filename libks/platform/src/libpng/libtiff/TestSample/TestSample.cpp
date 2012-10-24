// TestSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tiffio.h"
#include "geotiff.h"
#include "xtiffio.h"

int _tmain(int argc, _TCHAR* argv[])
{
	const char* filePath = "D:\\data\\gis\\workshop\\092b06.tif";
	TIFF* tif = XTIFFOpen((const char*)(filePath), "r");
	GTIF* gtif = GTIFNew(tif);

	double	adfCoeff[6], x, y;
	x = 0.5;
	y = 0.5;
	if( !GTIFImageToPCS( gtif, &x, &y ) )
		return 1;
	adfCoeff[4] = x;
	adfCoeff[5] = y;

	x = 1.5;
	y = 0.5;
	if( !GTIFImageToPCS( gtif, &x, &y ) )
		return 1;
	adfCoeff[0] = x - adfCoeff[4];
	adfCoeff[1] = y - adfCoeff[5];

	x = 0.5;
	y = 1.5;
	if( !GTIFImageToPCS( gtif, &x, &y ) )
		return 1;
	adfCoeff[2] = x - adfCoeff[4];
	adfCoeff[3] = y - adfCoeff[5];

	return 0;
}

