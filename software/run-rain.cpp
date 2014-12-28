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
		_hue     = 120;
	}
	
	void reset() {
		_length = (rand() % 15) + 20;
		_row    = -(rand() % 100);
		_delay  = (rand() % 4);
		_ticks  = 0;
	}
	
	void column(int value) {
		_column = value;
	}
	
	void hue(int value) {
		_hue = value;
	}
	
	void draw(LogiMatrix *_matrix) {
		int hue = 100;
		int x = _column;
		int y = _row;

		_matrix->setPixel(x, y--, 255, 255, 255);
		
		for (int i = 0; i < _length; i++) {

			double foo = 1.0 - ((double)i / (double)_length);
			
			HSL color;
			color.hue        = _hue;
			color.saturation = 100;
			color.luminance  = (int)(foo * 100.0);

			// Shake the brightness a bit
			//color.luminance += ((rand() % 50) - 25);
			
			if (color.luminance > 100)
				color.luminance = 100;

			if (color.luminance < 0)
				color.luminance = 0;
			
			_matrix->setPixel(x, y--, color);
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
	animation.speed(40.0);
	
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


