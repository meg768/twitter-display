#include "globals.h"


/*
class Animation {
	
public:
	Animation(LogiMatrix *matrix, int duration = -1) {
		_matrix = matrix;
		_duration = duration;
		_startTime = time(NULL);
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
		while (!expired()) {
			loop();
		}
		
	};
	
	virtual void loop() {
		
	}
	
private:
	LogiMatrix *_matrix;
	int _duration;
	time_t _startTime;
	
};


class GifAnimation {
	
	
public:
	GifAnimation(LogiMatrix *matrix) : Animation(matrix) {
		
	}
	
	virtual void initaialize(int argc, char *argv[]) {
		
	};
};
*/

int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	
	try {
		LogiMatrix matrix;
		Timer timer;

		int option = 0;
		int iterations = -1;
		int verbose = 0;
		float speed = 5.0;
		
		while ((option = getopt(argc, argv, "s:g:i:d:v")) != -1) {
			switch (option) {
				case 'i':
					iterations = atoi(optarg);
					break;
				case 'd':
					timer.duration(atoi(optarg));
					break;
				case 'v':
					verbose = true;
					break;
				case 'g':
					matrix.setGamma(atof(optarg));
					break;
				case 's':
					speed = atof(optarg);
					break;
			}
		}
		
		const char *animation = optind < argc ? argv[optind] : 0;
		
		if (animation == 0) {
			fprintf(stderr, "No animation specified.\n");
			return -1;
		}
		
		if (timer.duration() == 0)
			return 0;
		
		std::list<Magick::Image> images;
		Magick::readImages(&images, animation);
		
		std::list<Magick::Image>::iterator iterator = images.begin();
		
		// Check if we have a first image
		if (iterator != images.end()) {
			// If so, get the number of animation iterations
			Magick::Image &image = *iterator;

			if (iterations == -1) {
				iterations = image.animationIterations();
			}
		}
		
		if (iterations < 0)
			iterations = 0;
		
		while (!timer.expired()) {
			
			// Done iterating?!
			if (iterator == images.end()) {
				
				// If duration set, ignore iterations
				if (iterations > 0 && timer.duration() > 0) {
					iterations--;
					
					if (iterations == 0)
						break;
					
				}

				iterator = images.begin();
			}
			
			Magick::Image &image = *iterator;
			
			// Draw the image
			matrix.drawImage(image);
			
			// Get the animation delay factor
			size_t delay = image.animationDelay();
			
			iterator++;
			matrix.refresh();

			// Wait for next frame to display
			// (Seems like we have to reduce the delay by some factor)
			usleep(int(double((delay * 1000)) * speed));
			
		}
		
		matrix.clear();
		matrix.refresh();
		
		
	}
	catch (std::exception &error) {
		fprintf(stderr, "Could not start animation: %s\n", error.what());
		return -1;
	}
	
    return 0;
}


