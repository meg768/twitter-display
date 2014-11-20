/*
XXX
*/
#include "globals.h"


int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	LogiMatrix matrix;

	int option = 0;
	int delay = 0;
	
	while ((option = getopt(argc, argv, "g:d:")) != -1) {
		switch (option) {
			case 'g':
				matrix.setGamma(atof(optarg));
				break;
			case 'd':
				delay = atoi(optarg);
				break;
		}
	}

	const char *animation = optind < argc ? argv[optind] : 0;
	
	if (animation == 0) {
		fprintf(stderr, "No image specified.\n");
		return -1;
	}
	
	Magick::Image image;
	image.read(animation);

	int imageWidth = image.columns();
	int imageHeight = image.rows();

	// Make square image
	if (imageWidth != imageHeight) {
		if (imageWidth > imageHeight)
			image.crop(Magick::Geometry(imageHeight, imageHeight, (imageWidth - imageHeight) / 2, 0));
		else
			image.crop(Magick::Geometry(imageWidth, imageWidth, 0, (imageHeight - imageWidth) / 2));
		
		imageWidth  = image.rows();
		imageHeight = image.columns();

	}

	// Scale if needed
	if (imageWidth != matrix.width()) {
		image.sample(Magick::Geometry(matrix.width(), matrix.height()));
	}
	

	matrix.drawImage(image);
	matrix.refresh();
	
	
	if (delay > 0)
		sleep(delay);
	
    return 0;
}


