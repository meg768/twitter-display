#include "globals.h"


#define COLOR(red, green, blue) (uint16_t)((red << 8) | (green << 4) | blue)


int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	LogiMatrix matrix;
	
	int red = 0;
	int green = 0;
	int blue = 0;
	
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
			
			red = (15 * y	) / 32;
			
			for (int x = 0; x < 32; x++) {
				matrix.setPixel(x, y, COLOR(red, green, blue));
			}
		}
		
		matrix.refresh();
		
	
    return 0;
}


