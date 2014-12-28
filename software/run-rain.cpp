#include "globals.h"

using namespace std;



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


///////////////////////////////////////////////////////////////////////////////////////////////////


class Animation {
	
public:
	Animation(LogiMatrix *matrix, int duration = -1) {
		_duration = duration;
		_matrix = matrix;
		_startTime = time(NULL);
		_speed = 1.0;
	}
	
	void duration(int value) {
		_duration = value;
	}
	
	void gamma(double value) {
		_matrix->setGamma(value);
	}

	void speed(double value) {
		_speed = value;
	}
	
	virtual int expired() {
		if (_duration > 0) {
			if (time(NULL) - _startTime > _duration) {
				return true;
			}
		}
		
		return false;
	}
	
	virtual void run() {
		
		_matrix->clear();
		_matrix->refresh();
		
		while (!expired()) {
			loop();
			usleep(_speed * 1000);
		}

		_matrix->clear();
		_matrix->refresh();
		
	};
	
	virtual void loop() {
	}

	
protected:
	LogiMatrix *_matrix;
	int _duration;
	time_t _startTime;
	double _speed;
	
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class Worm {
	
public:
	Worm() {
		_length = (rand() % 20) + 10;
	}
	
	void reset() {
		
		
		
	}
	
	void draw(LogiMatrix *_matrix, int x, int y) {
		_matrix->setPixel(x, y--, 255, 255, 255);
		
		int hue = 100;
		
		for (int i = 0; i < _length; i++) {
			uint8_t	red, green, blue;
			
			double hue = 120.0;
			double saturation = 1.0;
			double brightness = 1.0 - ((double)i / (double)_length);
			
			brightness += (double)((rand() % 30) - 15) / 100.0;
			
			if (brightness > 1.0)
				brightness = 1.0;
			
			if (brightness < 0)
				brightness = 0;
			
			HslToRgb(hue, saturation, brightness, red, green, blue);
			_matrix->setPixel(x, y--, red, green, blue);
		}
	}

	int _length;
};

///////////////////////////////////////////////////////////////////////////////////////////////////


class MatrixAnimation : public Animation {
	
public:
	MatrixAnimation(LogiMatrix *matrix) : Animation(matrix) {
		srand(time(NULL));

	}
	
	~MatrixAnimation() {
	}
	
	

	virtual void loop() {
		_matrix->refresh();
		
	}

protected:
	
};

///////////////////////////////////////////////////////////////////////////////////////////////////




int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);
	
	LogiMatrix matrix;
	
	Worm worms[32];
	int foo[32];

	for (int i = 0; i < 32; i++)
		foo[i] = rand() % 32;
	
	for (int x = 0; x < 32; x++) {
		for (int i = 0; i < 32; i++) {
			worms[i].draw(&matrix, i, foo[i]);
			foo[i] = foo[i] + 1;
			
		}
		matrix.refresh();
		usleep(1000);
	}
		
	
	return 0;
}


