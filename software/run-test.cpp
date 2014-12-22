#include "globals.h"


#define COLOR(red, green, blue) (uint16_t)((red << 8) | (green << 4) | blue)


static int minutes[12][2] = {

	{14,  0},
	{21,  2},
	{26,  7},
	{28, 14},
	{26, 21},
	{21, 26},
	{14, 28},

	{ 7, 26},
	{ 2, 21},
	{ 0, 14},
	{ 2,  7},
	{ 7,  2}
	
};

static int hours[12][2] = {
	
	{14,  6},
	{18,  7},
	{21, 10},
	{22, 14},
	{21, 18},
	{18, 21},
	{14, 22},
	
	{10, 21},
	{ 7, 18},
	{ 6, 14},
	{ 7, 10},
	{10,  7}
	
};

class Clock {
	
public:
	Clock(LogiMatrix *matrix) {
		_matrix = matrix;
	}
	
	void foo() {
		for (int i = 0; i < 12; i++) {
			drawDot(minutes[i][0], minutes[i][1]);
		}
		for (int i = 0; i < 12; i++) {
			drawDot(hours[i][0], hours[i][1]);
		}
	}

	
	void drawDot(int x, int y) {
		
		uint8_t red = 255;
		uint8_t green = 0;
		uint8_t blue = 0;
		
		_matrix->setPixel(x + 1, y + 1, red, green, blue);
		_matrix->setPixel(x + 1, y + 2, red, green, blue);
		_matrix->setPixel(x + 2, y + 1, red, green, blue);
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
};


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
	
	clock.foo();
	
	matrix.refresh();
	

    return 0;
}


