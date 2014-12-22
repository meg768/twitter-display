#include "globals.h"


#define COLOR(red, green, blue) (uint16_t)((red << 8) | (green << 4) | blue)

class Clock {
	
public:
	Clock(LogiMatrix *matrix) {
		_matrix = matrix;
	}
	
	void drawDot(int x, int y) {
		
		uint8_t red = 255;
		uint8_t green = 0;
		uint8_t blue = 0;
		
		_matrix->setPixel(x + 1, y + 1, red, green, blue);
		_matrix->setPixel(x + 1, y + 2, red, green, blue);
		_matrix->setPixel(x + 2, y + 2, red, green, blue);
		_matrix->setPixel(x + 2, y + 2, red, green, blue);
		
		red = red / 3;
		green = green / 3;
		blue = blue / 3;
		
		_matrix->setPixel(x + 1, y + 0, red, green, blue);
		_matrix->setPixel(x + 2, y + 0, red, green, blue);
		
		_matrix->setPixel(x + 0, y + 1, red, green, blue);
		_matrix->setPixel(x + 0, y + 2, red, green, blue);
		
		_matrix->setPixel(x + 1, y + 3, red, green, blue);
		_matrix->setPixel(x + 2, y + 3, red, green, blue);
		
		_matrix->setPixel(x + 3, y + 1, red, green, blue);
		_matrix->setPixel(x + 3, y + 2, red, green, blue);
		
		
	}
	
private:
	LogiMatrix *_matrix;
	

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
	
	Clock clock(&matrix);
	
	clock.drawDot(0, 0);
	clock.drawDot(15, 15);
	
	matrix.refresh();
	

    return 0;
}


