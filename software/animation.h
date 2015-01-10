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
		_speed = 1.0;
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
		
		_canvas->clear();
		_canvas->refresh();
		
		while (!expired()) {
			loop();
			
			if (delay > 0)
				usleep(delay);
		}

		_canvas->clear();
		_canvas->refresh();
		
	};
	
	virtual void loop() {
	}

	
protected:
	Canvas *_canvas;
	int _duration;
	time_t _startTime;
	double _speed;
	
};


#endif
