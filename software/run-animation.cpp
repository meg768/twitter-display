#include "globals.h"


/*HUBBA */
int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	
	try {
		LogiMatrix matrix;
		Timer timer;

		int option = 0;
		int iterations = 1;
		int verbose = 0;
		float speed = 5.0;
		
		while ((option = getopt(argc, argv, "s:g:i:d:v")) != -1) {
			switch (option) {
				case 'i':
					iterations = atoi(optarg);
					break;
				case 'd':
					timer.setDuration(atoi(optarg));
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
		
		time_t startTime = time(NULL);
		
		const char *animation = optind < argc ? argv[optind] : 0;
		
		if (animation == 0) {
			fprintf(stderr, "No animation specified.\n");
			return -1;
		}

		std::list<Magick::Image> images;
		Magick::readImages(&images, animation);

		
		std::list<Magick::Image>::iterator iterator = images.begin();
		
		// Check if we have a first image
		if (iterator != images.end()) {
			// If so, get the number of animation iterations
			Magick::Image &image = *iterator;

			if (image.animationIterations() > 0) {
				iterations = image.animationIterations();
				if (verbose)
					printf("Image requests %d iterations.\n", iterations);
			}
		}
		
		if (iterations <= 0)
			iterations = 1;
		
		while (!timer.expired()) {
			
			// Done iterating?!
			if (iterator == images.end()) {
				iterations--;
				
				if (iterations == 0)
					break;

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


