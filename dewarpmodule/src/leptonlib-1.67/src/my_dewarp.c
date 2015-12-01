/* This function demonstrates how to dewarp a document image based on text lines.
 * 
 * 
 */

/* Compile command:
 gcc -ansi -DANSI -fPIC -O2  -I../src -I/usr/X11R6/include  -DL_LITTLE_ENDIAN  -c -o my_dewarp.o my_dewarp.c
 gcc -ansi -DANSI -fPIC -o my_dewarp my_dewarp.o ../lib/nodebug/liblept.a -ltiff -ljpeg -lpng -lz -lm
 */

#include "allheaders.h"


int my_dewarp (char	 *input_file,
			  char *output_file)
{
/*	char	 *input_file, *output_file;*/
	PIX      *pixs, *pixn, *pixg, *pixb;
	L_DEWARP *dew;
	l_int32 result = 0;
	//input_file = "./1555-7.jpg";
	//output_file = "./1555-7-dewarped.jpg";

	/** Read image */
	pixs = pixRead(input_file);

	/** Normalize for varying background and noise */
	pixn = pixBackgroundNormSimple(pixs, NULL, NULL);
	pixg = pixConvertRGBToGray(pixn, 0.5, 0.3, 0.2);
	pixb = pixThresholdToBinary(pixg, 130);

	/** Build dewarp disparity model and then apply to the original image */
	dew = dewarpCreate(pixb, 1, 30, 15, 1);
	result = dewarpBuildModel(dew, 1);
	if(0==result)
	{
		result = dewarpApplyDisparity(dew, pixs, 1);
	}

	/** Write dewarped image to assigned destination file */
	if(0==result)
	{
		pixWrite(output_file, dew->pixd, IFF_JFIF_JPEG);
	}
	else
	{
		pixWrite(output_file, pixs, IFF_JFIF_JPEG);
	}

	/** Release allocated memories */
    pixDestroy(&pixs);
    pixDestroy(&pixn);
    pixDestroy(&pixg);
    pixDestroy(&pixb);
    dewarpDestroy(&dew);

    return result;
}
