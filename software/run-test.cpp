#include "globals.h"


#define COLOR(red, green, blue) (uint16_t)((red << 8) | (green << 4) | blue)


int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	LogiMatrix matrix;
	

	Magick::Image imageA;
	imageA.read("images/A.png");
	//imageA.matte(true);
	
	Magick::Image imageB;
	imageB.read("images/B.png");
	
	Magick::Image imageC;
	imageC.type(Magick::TrueColorType);
	//imageC.matte(true);
	imageC.read("images/C.png");

	imageC.draw(Magick::DrawableCompositeImage(0, 0, imageA));
	//imageA.getConstPixels(0, 0, 32, 32);

	/*imageC.composite(imageA, 0, 0, Magick::OverCompositeOp);
	imageC.syncPixels();
	imageC.composite(imageB, 0, 0, Magick::OverCompositeOp);
	imageC.syncPixels();
	*/
	matrix.drawImage(imageC);
	matrix.refresh();
		
	
    return 0;
}


