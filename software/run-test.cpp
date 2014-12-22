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

void HSL2RGB(double h, double sl, double l, uint8_t &R, uint8_t &G, uint8_t &B)
{
	double v;
	double r,g,b;
 
	r = l;   // default to gray
	g = l;
	b = l;
	v = (l <= 0.5) ? (l * (1.0 + sl)) : (l + sl - l * sl);
	if (v > 0)
	{
		double m;
		double sv;
		int sextant;
		double fract, vsf, mid1, mid2;
		
		m = l + l - v;
		sv = (v - m ) / v;
		h *= 6.0;
		sextant = (int)h;
		fract = h - sextant;
		vsf = v * sv * fract;
		mid1 = m + vsf;
		mid2 = v - vsf;
		switch (sextant)
		{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
		}
	}
	
	R = (uint8_t)(r * 255.0f);
	G = (uint8_t)(g * 255.0f);
	B = (uint8_t)(b * 255.0f);
}



class Clock {
	
public:
	Clock(LogiMatrix *matrix) {
		_matrix = matrix;
	}
	
	void foo() {
		for (int i = 0; i < 12; i++) {
			drawDot(minutes[i][0], minutes[i][1], 360.0);
		}
		for (int i = 0; i < 12; i++) {
			drawDot(hours[i][0], hours[i][1], 240.0);
		}
	}

	
	void drawDot(int x, int y, double hue) {
		
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
		
		HSL2RGB(hue, 1.0, 1.0, red, green, blue);
		_matrix->setPixel(x + 1, y + 1, red, green, blue);
		_matrix->setPixel(x + 1, y + 2, red, green, blue);
		_matrix->setPixel(x + 2, y + 1, red, green, blue);
		_matrix->setPixel(x + 2, y + 2, red, green, blue);
		
		HSL2RGB(hue, 1.0, 0.3, red, green, blue);
		
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


