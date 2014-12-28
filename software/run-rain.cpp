#include "globals.h"

using namespace std;

/*
static char *digit_0[7][7] = {
	' XXXXX ',
	'X     X',
	'X     X',
	'X     X',
	'X     X',
	'X     X',
	' XXXXX '
}
static char digits[][] = {
	{
		''
	}
};
*/


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
	Animation(int duration = -1) {
		_matrix = new LogiMatrix();
		_duration = duration;
		_startTime = time(NULL);
		_speed = 1.0;
	}
	
	~Animation() {
		delete _matrix;
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
		
		int delay = (int)(_speed * 1000.0);
		
		_matrix->clear();
		_matrix->refresh();
		
		while (!expired()) {
			loop();
			
			if (delay > 0)
				usleep(delay);
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
		_row     = 0;
		_column  = 0;
		_length  = 0;
		_delay   = 0;
		_ticks   = 0;
		_hue     = -1;
	}
	
	void reset() {
		_length = (rand() % 15) + 20;
		_row    = -(rand() % 100);
		_delay  = (rand() % 5) + 2;
		_ticks  = 0;
	}
	
	void column(int value) {
		_column = value;
	}
	
	void hue(int value) {
		_hue = value;
	}
	
	void draw(LogiMatrix *_matrix) {
		int hue = 120;
		int x = _column;
		int y = _row;
/*
		if (_hue < 0) {
			time_t t = time(0);
			struct tm *now = localtime(&t);
			hue = ((now->tm_hour % 12) * 60 + now->tm_min) / 2;
			
		}
		else
			hue = _hue;
*/
		_matrix->setPixel(x, y--, 255, 255, 255);
		
		for (int i = 0; i < _length; i++) {

			
			uint8_t	red, green, blue;
			
			double saturation = 1.0;
			double brightness = 1.0 - ((double)i / (double)_length);
			
			brightness += (double)((rand() % 50) - 25) / 100.0;
			
			if (brightness > 1.0)
				brightness = 1.0;
			
			if (brightness < 0)
				brightness = 0;
			
			HslToRgb((double)hue, saturation, brightness, red, green, blue);
			_matrix->setPixel(x, y--, red, green, blue);
			/*
			int percent = (100 * i) / _length;
			
			HSL color;
			color.hue        = _hue;
			color.saturation = 100;
			color.luminance  = 100 - percent;

			// Shake the brightness a bit
			//color.luminance = (color.luminance * percent * (rand() % 100)) / 100;
			//double factor = (double)i / (double)_length;
			//double stability = 1.0 - factor;
			//double fix = stability * double(rand() % 10);
			//color.luminance = (color.luminance * percent * (rand() % 100)) / 100;

			//double luminance = color.luminance;
			
			//color.luminance  = (color.luminance * ((rand() % 10) - 5)) / 5;
			
			
			double factor = (double)(rand() % 100) * (double)i / (double)_length;
			factor = factor - 50.0;
			//			factor = 1.0 / factor;
//			factor = 1 - factor;
			factor = factor / 5.0;
			
			color.luminance = (int)((double)color.luminance + factor);

			if (color.luminance > 100)
				color.luminance = 100;

			if (color.luminance < 0)
				color.luminance = 0;
			
			_matrix->setPixel(x, y--, color);
			 */
		}
	}
	
	void idle() {
		_ticks++;
		
		if (_ticks >= _delay) {
			_ticks = 0;
			_row++;
			
			if (_row - _length > 32)
				reset();
			
		}

	}
	
private:
	int _length, _iterations, _delay, _ticks, _hue;
	int _row, _column;
};

///////////////////////////////////////////////////////////////////////////////////////////////////


class MatrixAnimation : public Animation {
	
public:
	MatrixAnimation() : Animation() {

		int size = _matrix->width();
		
		_worms.resize(size);
		
		for (int i = 0; i < size; i++) {
			_worms[i].column(i);
			_worms[i].reset();
		}
	}
	
	~MatrixAnimation() {
	}
	
	void hue(int value) {
		for (int i = 0; i < (int)_worms.size(); i++) {
			_worms[i].hue(value);
		}
		
	}

	virtual void loop() {
		_matrix->clear();
		
		for (int i = 0; i < _matrix->width(); i++) {
			_worms[i].draw(_matrix);
			_worms[i].idle();
		}
		
		_matrix->refresh();
		
	}

protected:
	vector <Worm> _worms;
};

///////////////////////////////////////////////////////////////////////////////////////////////////




int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);
	
	srand(time(NULL));

	MatrixAnimation animation;
	animation.duration(60);
	animation.speed(2.0);
	
	int option = 0;
	
	while ((option = getopt(argc, argv, "h:s:g:d:")) != -1) {
		switch (option) {
			case 'd':
				animation.duration(atoi(optarg));
				break;
			case 'g':
				animation.gamma(atof(optarg));
				break;
			case 's':
				animation.speed(atof(optarg));
				break;
			case 'h':
				animation.hue(atoi(optarg));
				break;
		}
	}
	
	animation.run();
	
	
	return 0;
}


