#ifndef _animation_h
#define _animation_h

#include "canvas.h"

class Animation {
	
public:
	Animation(int duration = 60) {
		srand(time(NULL));

		_canvas = new Canvas();
		_duration = duration;
		_startTime = time(NULL);
		_speed = 1.0;
		_delay = 1000;
	}
	
	~Animation() {
		delete _canvas;
	}
	
	inline void gamma(double value) {
		_canvas->setGamma(value);
	}
	
	inline void duration(int value) {
		_duration = value;
	}
	
	inline int duration() {
		return _duration;
	}
	
	inline void speed(double value) {
		_speed = value;
		_delay = 1000.0 * value;
	}
	
	inline double speed() {
		return _speed;
	}
	
	inline void sleep() {
		if (_delay > 0)
			usleep(_delay);
	}
	
	inline Canvas *canvas() {
		return _canvas;
	}
	
	virtual int expired() {
		if (_duration > 0) {
			if (time(NULL) - _startTime > _duration) {
				return true;
			}
		}
		
		return false;
	}
	
	virtual void args(int argc, char *argv[]) {
		int option = 0;
		
		int optindSaved   = optind;
		int opterrSaved   = opterr;
		char *optargSaved = optarg;
		
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
					speed(atof(optarg));
					break;
			}
		}

		optind = optindSaved;
		optarg = optargSaved;
		opterr = opterrSaved;
		
	}
	
	virtual int run() {
		_canvas->clear();
		_canvas->refresh();
		
		while (!expired()) {
			loop();
		}
		
		_canvas->clear();
		_canvas->refresh();
		
		return 0;
		
	}

	virtual void loop() {
		sleep();
	}
	
	virtual int run(int argc, char *argv[]) {
		args(argc, argv);
		return run();
	}
	

	
protected:
	Canvas *_canvas;
	time_t _startTime;
	int _duration;
	int _delay;
	double _speed;
	
};


#endif
