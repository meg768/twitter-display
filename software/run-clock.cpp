#include "globals.h"


class Clock {
	
public:
	Clock(LogiMatrix *matrix) {
		_matrix = matrix;
		_nightMode = false;
	}
	
	void HslToRgb(double h, double s, double v, uint8_t &red, uint8_t &green, uint8_t &blue)
	{
		double hh = 0, p = 0, q = 0, t = 0, ff = 0;
		double r = 0, g = 0, b = 0;
		long i = 0;
		
		if (s <= 0.0) {
			r = v, g = v, b = v;
		}
		else {
			hh = h;

			while (hh < 0)
				hh += 360.0;
			
			while (hh >= 360.0)
				hh -= 360.0;

			hh = hh / 60.0;
			i  = (long)hh;
			ff = hh - i;
			
			p = v * (1.0 - s);
			q = v * (1.0 - (s * ff));
			t = v * (1.0 - (s * (1.0 - ff)));
			
			switch(i) {
				case 0:
					r = v, g = t, b = p;
					break;
				case 1:
					r = q, g = v, b = p;
					break;
				case 2:
					r = p, g = v, b = t;
					break;
					
				case 3:
					r = p, g = q, b = v;
					break;
				case 4:
					r = t, g = p, b = v;
					break;
				default:
					r = v, g = p, b = q;
					break;
			}
			
		}
		
		red   = (uint8_t)(r * 255.0);
		green = (uint8_t)(g * 255.0);
		blue  = (uint8_t)(b * 255.0);
	}
	
	void drawMinutes(struct tm *now) {
		static int coords[12][2] = {
			
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

		if (_nightMode) {
			int minutes = (int)(((double)now->tm_min + 2.5) / 5.0);
			
			drawDot(coords[minutes][0], coords[minutes][1], 0);
		}
		else {
			double factor = (double)(now->tm_min) / (60.0);
			
			for (int i = 0; i < 12; i++) {
				drawDot(coords[i][0], coords[i][1], (double)i / 12.0 * 360.0 - factor * 360.0);
			}
			
		}
	};
	
	void drawHours(struct tm *now) {
		static int coords[12][2] = {
			
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
		
		if (_nightMode) {
			int hours = now->tm_hour % 12;
			drawDot(coords[hours][0], coords[hours][1], 0);
			
			
		}
		else {
			double factor = (double)((now->tm_hour % 12) * 60 + now->tm_min) / (12.0 * 60.0);
			
			for (int i = 0; i < 12; i++) {
				drawDot(coords[i][0], coords[i][1], (double)i / 12.0 * 360.0 - factor * 360.0);
			}
			
		}
		
	};

	
	void drawSeconds(struct tm *now) {
		double seconds = _nightMode ? 0 : (double)(now->tm_sec) / (60.0);
		drawDot(14, 14, seconds * 360.0);
		
	};
	
	void increment(struct tm *time) {
	
			time->tm_sec = 61;
			
			if (time->tm_sec >= 60)
				time->tm_sec = 0, time->tm_min++;
		
			if (time->tm_min >= 60)
				time->tm_min = 0, time->tm_hour++;
			
			if (time->tm_hour >= 24)
				time->tm_hour = 0;
	};
	
	void drawTime(struct tm *now) {

		_matrix->clear();
		
		drawHours(now);
		drawMinutes(now);
		drawSeconds(now);
		
		_matrix->refresh();
		
	}

	void drawTime() {
		
		time_t t = time(0);
		struct tm *now = localtime(&t);
		drawTime(now);
		
	}

	
	void drawDot(int x, int y, double hue) {
		
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
		
		HslToRgb(hue, 1.0, 1.0, red, green, blue);
		
		_matrix->setPixel(x + 1, y + 1, red, green, blue);
		_matrix->setPixel(x + 1, y + 2, red, green, blue);
		_matrix->setPixel(x + 2, y + 1, red, green, blue);
		_matrix->setPixel(x + 2, y + 2, red, green, blue);

		HslToRgb(hue, 1.0, 0.3, red, green, blue);
		
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
	int _nightMode;
};


int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);
	
	LogiMatrix matrix;
	Timer timer(-1);
	
	int option = 0;
	
	while ((option = getopt(argc, argv, "g:d:")) != -1) {
		switch (option) {
			case 'd':
				timer.duration(atoi(optarg));
				break;
			case 'g':
				matrix.setGamma(atof(optarg));
				break;
		}
	}
	
	Clock clock(&matrix);
	
	while (!timer.expired()) {
		time_t t = time(0);
		struct tm *now = localtime(&t);
		clock.drawTime(now);
		usleep(950);
	}
	
	matrix.clear();
	matrix.refresh();
	
	return 0;
}


