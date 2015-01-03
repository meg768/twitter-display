#include "globals.h"


int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);
	
	const char *textColor  = "red";
	const char *fontName   = "Arial";

	int option       = 0;
	int iterations   = 1;
	int pointSize    = 16;
	int speed        = 3;
	
	try {
		LogiMatrix matrix;
		
		while ((option = getopt(argc, argv, "g:i:p:c:f:s:")) != -1) {
			switch (option) {
				case 's':
					speed = atoi(optarg);
					break;
				case 'i':
					iterations = atoi(optarg);
					break;
				case 'p':
					pointSize = atoi(optarg);
					break;
				case 'c':
					textColor = optarg;
					break;
				case 'f':
					fontName = optarg;
					break;
				case 'g':
					matrix.setGamma(atof(optarg));
					break;
			}
		}
		
		if (speed < 0)
			speed = 0;
		
		if (speed > 10)
			speed = 10;
		
		if (iterations == 0)
			iterations = 1;
		
		const char *text = optind < argc ? argv[optind] : 0;
		
		if (text == 0) {
			fprintf(stderr, "No text specified.\n");
			return -1;
		}
		
		Magick::Image tmp("32x32", "black");
		
		char fontFile[200];
		sprintf(fontFile, "./fonts/%s", fontName);
		
		tmp.font(fontFile);
		tmp.strokeColor("transparent");
		tmp.fillColor(textColor);
		tmp.fontPointsize(pointSize);
		
		Magick::TypeMetric metric;
		tmp.fontTypeMetrics(text, &metric);
		
		Magick::Image image(Magick::Geometry(metric.textWidth() + 2.0, 32.0), "black");
		image.font(fontFile);
		image.strokeColor("transparent");
		image.fillColor(textColor);
		image.fontPointsize(pointSize);
		
		image.draw(Magick::DrawableText(1, 16.0 + metric.textHeight() / 2.0 + metric.descent(), text));

		int screenHeight = matrix.height();
		int screenWidth  = matrix.height();
		int imageWidth   = image.columns();
		int imageHeight  = image.rows();
		int offsetX      = -screenWidth;
		int offsetY      = -(screenHeight - imageHeight) / 2;
		
		int count = 0;
		

		while (count < iterations) {
			matrix.clear();

			const Magick::PixelPacket *pixels = image.getConstPixels(offsetX, offsetY, screenWidth, screenHeight);

			for (int row = 0; row < screenHeight; row++) {
				for (int col = 0; col < screenWidth; col++) {
					if (offsetX + col < 0 || offsetX + col >= imageWidth)
						matrix.setPixel(col, row, 0, 0, 0);
					else if (offsetY + row < 0 || offsetY + row >= imageHeight)
						matrix.setPixel(col, row, 0, 0, 0);
					else
						matrix.setPixel(col, row, pixels->red, pixels->green, pixels->blue);
					pixels++;
				}
			}
			
			matrix.refresh();
			
			if (++offsetX > imageWidth) {
				offsetX = -screenWidth;
				count++;
			}

			usleep(speed * 1000);
		}

	}
	catch (std::exception &error) {
		fprintf(stderr, "%s\n", error.what());
		return -1;
	}

	
    return 0;
}


