#include "globals.h"

int main (int argc, char *argv[])
{
	srand(time(NULL));
	Magick::InitializeMagick(*argv);

	LogiMatrix matrix;
	Timer timer;

	int option = 0;
	
	while ((option = getopt(argc, argv, "g:d:")) != -1) {
		switch (option) {
			case 'g':
				matrix.setGamma(atof(optarg));
				break;
			case 'd':
				timer.setDuration(atoi(optarg));
				break;
		}
	}


	while (!timer.expired()) {

		int width = matrix.width();
		int height = matrix.height();

		
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int value = (rand() % 2) * 128;
				
				matrix.setPixel(x, y, value, value, value);
			}
		}

		matrix.refresh();
		usleep(3 * 1000);
	}
	
	matrix.clear();
	matrix.refresh();
	
    return 0;
}


