#include "globals.h"

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
		//_matrix->refresh();
		
		while (!expired()) {
//			loop();
			sleep(1);
			//usleep(_speed * 1000);
		}

		_matrix->clear();
		//_matrix->refresh();
		
	};
	
	virtual void loop() {
	}

	
protected:
	LogiMatrix *_matrix;
	int _duration;
	time_t _startTime;
	double _speed;
	
};

class ClockAnimation : public Animation {
	
public:
	ClockAnimation(LogiMatrix *matrix) : Animation(matrix) {
		srand(time(NULL));

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
					twinkle->hue = (rand() % 3) * 120;
					twinkle->brightness = 0;
					twinkle->speed = 5; //(rand() % 10) + 1;
					twinkle->max = 100; //(rand() % 100);
					twinkle->duration = (rand() % 5) + 1;
					twinkle->delay = (rand() % 1000);
					twinkle->state++;
					break;
				}
				case 1: {
					if (--twinkle->delay < 0)
						twinkle->state++;

					break;
				}
				case 2: {
					twinkle->brightness += twinkle->speed;
					
					if (twinkle->brightness > twinkle->max) {
						twinkle->brightness = twinkle->max;
						twinkle->state++;
					}
					break;
					
				}
				case 3: {
					if (--twinkle->duration < 0) {
						twinkle->state++;
					}
					break;
					
				}
				case 4: {
					twinkle->brightness -= twinkle->speed;
					
					if (twinkle->brightness < 0) {
						twinkle->brightness = 0;
						twinkle->state = 0;
					}
					break;
					
				}

			}
			
			uint8_t red, green, blue;
			HslToRgb((double)twinkle->hue, 1.0, (double)twinkle->brightness / 100.0, red, green, blue);
			
			_matrix->setPixel(i % width, i / height, red, green, blue);
		}
		
		//_matrix->refresh();
		
	}

protected:
	
	typedef struct {
		int state;
		int hue;
		int brightness;
		int speed;
		int max;
		int duration;
		int delay;
	} Twinkler;
	
	
	Twinkler *_twinkle;
};

static Animation *my_animation = 0;
static LogiMatrix *my_matrix = 0;

void timer_handler (int signum)
{
	my_animation->loop();
	my_matrix->refresh();
}



int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);
	
	LogiMatrix matrix;
	ClockAnimation animation(&matrix);
	
	my_matrix = &matrix;
	my_animation = &animation;
	
	struct sigaction sa;
	struct itimerval timer;
	
	// install timer handler
	memset (&sa, 0, sizeof (sa));
	sa.sa_handler = &timer_handler;
	sigaction (SIGALRM, &sa, NULL);
	
	// configure the timer to expire after 20 msec
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 5000;
	
	// and every 20 msec after that.
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 5000;
	
	// start the timer
	setitimer (ITIMER_REAL, &timer, NULL);
	
	animation.duration(60);
	
	int option = 0;
	
	while ((option = getopt(argc, argv, "s:g:d:")) != -1) {
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
		}
	}
	
	
	animation.run();
	
	
	
	return 0;
}


