#include "globals.h"


class Animation {
	
public:
	Animation(LogiMatrix *matrix, int duration = -1) {
		_duration = duration;
		_matrix = matrix;
		_startTime = time(NULL);
	}
	
	void duration(int value) {
		_duration = value;
	}
	
	void gamma(double value) {
		_matrix->setGamma(value);
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
		
		while (!expired()) {
			loop();
		}

		_matrix->clear();
		
	};
	
	virtual void loop() {
	}

	
protected:
	LogiMatrix *_matrix;
	int _duration;
	time_t _startTime;
	
};

class ClockAnimation : public Animation {
	
public:
	ClockAnimation(LogiMatrix *matrix) : Animation(matrix) {
		int size = matrix->width() * matrix->height();
		_twinkle = new Twinkler[size];
		
		for (int i = 0; i < size; i++) {
			_twinkle[i].state = 0;
		}
	}
	
	~ClockAnimation() {
		delete []_twinkle;
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
	

	virtual void loop() {
		int width = _matrix->width();
		int height = _matrix->height();
		int size = width * height;
		
		Twinkler *twinkle = _twinkle;
		
		for (int i = 0; i < size; i++, twinkle++) {
			switch (twinkle->state) {
				case 0: {
					twinkle->hue = (rand() % 6) * 60;
					twinkle->brightness = 0;
					twinkle->speed = (rand() % 10) + 5;
					twinkle->max = (rand() % 30) + 70;
					twinkle->length = (rand() % 50) + 50;
					break;
				}
				case 1: {
					twinkle->brightness += twinkle->speed;
					
					if (twinkle->brightness > twinkle->max) {
						twinkle->brightness = twinkle->max;
						twinkle->state++;
					}
					break;
					
				}
				case 2: {
					if (--twinkle->length <= 0) {
						twinkle->state++;
					}
					break;
					
				}
				case 3: {
					if (--twinkle->length <= 0) {
						twinkle->state++;
					}
					break;
					
				}

			}
			
			uint8_t red, green, blue;
			HslToRgb((double)twinkle->hue, (double)twinkle->brightness / 100.0, 1.0, red, green, blue);
			
			_matrix->setPixel(i % width, i / height, red, green, blue);
		}
		
	}

protected:
	
	typedef struct {
		int state;
		int hue;
		int brightness;
		int speed;
		int max;
		int length;
	} Twinkler;
	
	
	Twinkler *_twinkle;
};


int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);
	
	LogiMatrix matrix;
	ClockAnimation animation(&matrix);
	
	int option = 0;
	
	while ((option = getopt(argc, argv, "g:d:")) != -1) {
		switch (option) {
			case 'd':
				animation.duration(atoi(optarg));
				break;
			case 'g':
				animation.gamma(atof(optarg));
				break;
		}
	}
	
	
	clock.run();
	
	
	
	return 0;
}


