#ifndef _animation_h
#define _animation_h

#include "canvas.h"

class Animation {
	
public:
	Animation(int duration = -1) {
		srand(time(NULL));

		_canvas = new Canvas();
		_duration = duration;
		_startTime = time(NULL);
		_speed = 1000;
	}
	
	~Animation() {
		delete _canvas;
	}
	
	void duration(int value) {
		_duration = value;
	}
	
	void gamma(double value) {
		_canvas->setGamma(value);
	}

	void speed(int value) {
		_speed = value;
	}
	
	void sleep() {
		if (_speed > 0)
			usleep(_speed);
	}
	
	virtual int expired() {
		if (_duration > 0) {
			if (time(NULL) - _startTime > _duration) {
				return true;
			}
		}
		
		return false;
	}
	
	virtual void init(int argc, char *argv[]) {
		int option = 0;
		int saved = optind;
		
		optind = 1;
		opterr = 0;
		
		while ((option = getopt(argc, argv, "s:g:d:")) != -1) {
			switch (option) {
				case 'd':
					duration(atoi(optarg));
					break;
				case 'g':
					gamma(atof(optarg));
					break;
				case 's':
					speed(atof(optarg) * 1000.0);
					break;
			}
		}

		optind = saved;
	}
	
	
	virtual int run(int argc, char *argv[]) {
		
		init(argc, argv);
		
		_canvas->clear();
		_canvas->refresh();
		
		while (!expired()) {
			loop();
			sleep();
		}

		_canvas->clear();
		_canvas->refresh();
		
		return 0;
	};
	
	virtual void loop() {
	}

	
protected:
	Canvas *_canvas;
	time_t _startTime;
	int _duration;
	int _speed;
	
};


#endif
