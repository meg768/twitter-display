#include "globals.h"


int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	LogiMatrix matrix;
	
	int red = 128;
	int green = 128;
	int blue = 128;
	
	int option = 0;
	
	while ((option = getopt(argc, argv, "r:g:b:")) != -1) {
		switch (option) {
			case 'r':
				red = atoi(optarg);
				break;
			case 'g':
				green = atoi(optarg);
				break;
			case 'b':
				blue = atoi(optarg);
				break;
		}
	}

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			matrix.setPixel(x, y, red, green, blue);
		}
	}

	matrix.refresh();
	
    return 0;
}


