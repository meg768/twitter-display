#include "globals.h"


#define COLOR(red, green, blue) (uint16_t)((red << 8) | (green << 4) | blue)


int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	LogiMatrix matrix;
	
	int op = 0;
	int option = 0;
	
	while ((option = getopt(argc, argv, "o:")) != -1) {
		switch (option) {
			case 'o':
				op = atoi(optarg);
				break;
		}
	}
	
	Magick::Image imageA;
	imageA.read("images/A.png");
	//imageA.matte(true);
	
	Magick::Image imageB;
	imageB.read("images/B.png");
	
	Magick::Image imageC;
	imageC.read("images/C.png");
	//imageC.matte(true);

	imageC.composite(imageA, 0, 0, Magick::CompositeOp(op));
	imageC.composite(imageB, 0, 0, Magick::CompositeOp(op));
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


