#include "globals.h"

int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);
	

	int option       = 0;
	int iterations   = 1;
	int pointSize    = 16;
	int delay        = 5;
	const char *textColor  = "red";
	const char *fontName   = "Arial";
	int ruby = 0;
	
	try {
		LogiMatrix matrix;
		
		while ((option = getopt(argc, argv, "r:g:i:p:c:f:")) != -1) {
			switch (option) {
				case 'i':
					iterations = atoi(optarg);
					break;
				case 'p':
					pointSize = atoi(optarg);
					break;
				case 'r':
					ruby = atoi(optarg);
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
		
		if (iterations == 0)
			iterations = 1;
		
		const char *text = optind < argc ? argv[optind] : 0;
		
		if (text == 0) {
			fprintf(stderr, "No text specified.\n");
			return -1;
		}
		
		/*
		const char *fileName = "run-text.png";


		char cmd[1000];
		sprintf(cmd, "python run-text.py -o \"%s\" -c \"%s\" -p %d -f \"%s\" -t \"%s\" ", fileName, textColor, pointSize, fontName, text);
			
		printf("Executing command: %s\n", cmd);
		system(cmd);
			*/
		
		
			Magick::Image tmp("32x32", "black");
			
			char fontFile[200];
			sprintf(fontFile, "./fonts/%s.ttf", fontName);
			
			tmp.font(fontFile);
			tmp.strokeColor("transparent");
			tmp.fillColor(textColor);
			tmp.fontPointsize(pointSize);
			
			Magick::TypeMetric metric;
			tmp.fontTypeMetrics(text, &metric);

			Magick::Image image(Magick::Geometry(metric.textWidth() + 2, 32), "black");
			image.font(fontFile);
			image.strokeColor("transparent");
			image.fillColor(textColor);
			image.fontPointsize(pointSize);
		
			//image.resize(Magick::Geometry(metric.textWidth() + 2, 32));
			image.draw(Magick::DrawableText(1, 16 - metric.textHeight() / 2, text));

		
			
		
		//Magick::Image image;
		//image.read(fileName);
		
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
			
			if (++offsetX >= (imageWidth + screenWidth)) {
				offsetX = -screenWidth;
				count++;
			}

			usleep(delay * 1000);
		}
	}
	catch (std::exception &error) {
		fprintf(stderr, "%s\n", error.what());
		return -1;
	}

	
    return 0;
}


