#include "globals.h"


//
// This is right now a total mess!
// But the result is finally OK...
//

typedef struct {
	double r;       // percent
	double g;       // percent
	double b;       // percent
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // percent
	double v;       // percent
} hsv;

static rgb      hsv2rgb(hsv in);


void HslToRgb(double h, double s, double v, double &r, double &g, double &b)
{
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;
	
	if(s <= 0.0) {       // < is bogus, just shuts up warnings
		r = v;
		g = v;
		b = v;
		return;
	}
	hh = h;
	if(hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - s);
	q = in.v * (1.0 - (s * ff));
	t = in.v * (1.0 - (s * (1.0 - ff)));
	
	switch(i) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
			
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		case 5:
		default:
			r = v;
			g = p;
			b = q;
			break;
	}
	
}

rgb hsv2rgb(hsv in)
{
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;
	
	if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if(hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));
	
	switch(i) {
		case 0:
			out.r = in.v;
			out.g = t;
			out.b = p;
			break;
		case 1:
			out.r = q;
			out.g = in.v;
			out.b = p;
			break;
		case 2:
			out.r = p;
			out.g = in.v;
			out.b = t;
			break;
			
		case 3:
			out.r = p;
			out.g = q;
			out.b = in.v;
			break;
		case 4:
			out.r = t;
			out.g = p;
			out.b = in.v;
			break;
		case 5:
		default:
			out.r = in.v;
			out.g = p;
			out.b = q;
			break;
	}
	return out;
}

static int minutesCoordsXX[12][2] = {
	
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

static int minutesCoords[12][2] = {
	
	{14,  1},
	{21,  2},
	{26,  7},
	{27, 14},
	{26, 21},
	{21, 26},
	{14, 27},
	
	{ 7, 26},
	{ 2, 21},
	{ 1, 14},
	{ 2,  7},
	{ 7,  2}
	
};

static int hoursCoords[12][2] = {
	
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
	
	void draw() {
		
		time_t t = time(0);
		struct tm *now = localtime(&t);
		
		double hours   = (double)((now->tm_hour % 12) * 60 + now->tm_min) / (12.0 * 60.0);
		double minutes = (double)(now->tm_min) / (60.0);
		double seconds = (double)(now->tm_sec) / (60.0);
		
		for (int i = 0; i < 12; i++) {
			double angle = (double)i / 12.0 * 360.0;
			angle -= minutes * 360.0;
			while (angle < 0.0)
				angle += 360.0;
			drawDot(minutesCoords[i][0], minutesCoords[i][1], angle);
		}
		for (int i = 0; i < 12; i++) {
			double angle = (double)i / 12.0 * 360.0;
			angle -= hours * 360.0;
			while (angle < 0.0)
				angle += 360.0;
			drawDot(hoursCoords[i][0], hoursCoords[i][1], angle);
		}
		
		drawDot(14, 14, seconds * 360.0);

		/*
		 hsv hsv;
		rgb rgb;
		
		hsv.h = seconds * 360.0;
		hsv.s = 1.0;
		hsv.v = 1.0;
		
		
		
		rgb = hsv2rgb(hsv);
		uint8_t red = rgb.r * 255;
		uint8_t green = rgb.g * 255;
		uint8_t blue = rgb.b * 255;
		
		for (int x = 0; x < 32; x++) {
			for (int y = 0; y < 32; y++) {
				if (x == 0 || y == 0 || x == 31 || y == 31) {
					
					_matrix->setPixel(x, y, red, green, blue);
				}
			}
		}
		 */
		_matrix->refresh();
		usleep(1000);

	}
	
	
	void drawDot(int x, int y, double hue) {
		
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
		
		
		hsv hsv;
		rgb rgb;
		
		hsv.h = hue;
		hsv.s = 1.0;
		hsv.v = 1.0;
		
		rgb = hsv2rgb(hsv);
		
		red = rgb.r * 255;
		green = rgb.g * 255;
		blue = rgb.b * 255;
		
		
		//HSL2RGB(hue, 1.0, 1.0, red, green, blue);
		_matrix->setPixel(x + 1, y + 1, red, green, blue);
		_matrix->setPixel(x + 1, y + 2, red, green, blue);
		_matrix->setPixel(x + 2, y + 1, red, green, blue);
		_matrix->setPixel(x + 2, y + 2, red, green, blue);
		
		hsv.h = hue;
		hsv.s = 1.0;
		hsv.v = 0.3;
		
		rgb = hsv2rgb(hsv);
		
		red = rgb.r * 255;
		green = rgb.g * 255;
		blue = rgb.b * 255;
		
		
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
	Timer timer(-1);
	
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
	
	Clock clock(&matrix);
	
	while (!timer.expired()) {
		clock.draw();
	}
	
	
	return 0;
}


