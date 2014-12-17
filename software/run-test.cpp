#include "globals.h"


#define COLOR(red, green, blue) (uint16_t)((red << 8) | (green << 4) | blue)


int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	LogiMatrix matrix;
	

	Magick::Image imageA;
	imageA.read("images/A.png");
	
	Magick::Image imageB;
	imageB.read("images/B.png");
	
	Magick::Image imageC("32x32", "black");
//	imageC.read("images/C.png");

	//imageA.getConstPixels(0, 0, 32, 32);

	imageC.composite(imageA, 0, 0, Magick::OverCompositeOp);
	imageC.syncPixels();
	imageC.composite(imageB, 0, 0, Magick::OverCompositeOp);
	imageC.syncPixels();
	
	matrix.drawImage(imageC);
	matrix.refresh();
		
	
    return 0;
}


