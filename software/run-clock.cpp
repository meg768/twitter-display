#include "globals.h"


#define RGB(r, g ,b)	((uint32_t) (((uint8_t) (r) | ((uint16_t) (g) << 8)) | (((uint32_t) (uint8_t) (b)) << 16)))
#define RED(rgb)		((uint8_t) (rgb))
#define BLUE(rgb)		((uint8_t) ((rgb) >> 16))
#define GREEN(rgb)		((uint8_t) (((uint16_t) (rgb)) >> 8))

class ClockAnimation  {
	
	
public:
	
	ClockAnimation(LogiMatrix *canvas)
	{
		_canvas          = canvas;
		_hourColor       = RGB(255, 0, 0);
		_hourBackColor   = RGB(0, 255, 0);
		_minuteColor     = RGB(0, 0, 255);
		_minuteBackColor = RGB(0, 0, 127);
		_blipColor       = RGB(255, 255, 255);
	}
	
	inline void drawSquare(int x, int y, int red, int green, int blue)
	{
		
		drawPixel(x, y, red, green, blue);
		drawPixel(x, y - 1, red, green, blue);
		drawPixel(x - 1, y - 1, red, green, blue);
		drawPixel(x - 1, y, red, green, blue);
	}
	
	inline void drawSquare(int x, int y, int color)
	{
		
		drawSquare(x, y, RED(color), GREEN(color), BLUE(color));
	}
	
	inline void drawPixel(int x, int y, int color)
	{
		
		drawPixel(x, y, RED(color), GREEN(color), BLUE(color));
	}
	
	inline void drawPixel(int x, int y, int red, int green, int blue) {
		
		_canvas->setPixel(x + 16, y + 16, red, green, blue);
	}
	
	
	void drawHours(int hours) {
		static int coords[12][2] = {
			{ 0, -15}, {8, -13}, {13, -8},
			{15,   0}, {13, 8}, {8, 13},
			{ 0,  15}, { -8,  13}, {-13,  8},
			{-15,  0}, {-13,  -8}, {-8, -13}
		};
		
		hours = hours % 12;
		
		for (int i = 0; i < 12; i++) {
			if (i <= hours)
				drawSquare(coords[i][0], coords[i][1], RED(_hourColor), GREEN(_hourColor), BLUE(_hourColor));
			else
				drawSquare(coords[i][0], coords[i][1], RED(_hourBackColor), GREEN(_hourBackColor), BLUE(_hourBackColor));
		}
	}
	
	
	void drawMinutes(int minutes, int seconds) {
		static int coords[12][2] = {
			{ 0, -11}, { 6,  -9}, { 10, -5},
			{11,   0}, { 10,  5}, {  6,  9},
			{ 0,  11}, {-6,   9}, {-10,  5},
			{-11,  0}, {-10, -5}, { -6, -9}
		};
		
		
		double time = (double)minutes + (double)seconds / 60.0;
		int value = (int)((time + 2.5) / 5.0) * 5;;
		
		for (int i = 0; i < 12; i++) {
			if (i * 5 <= value)
				drawSquare(coords[i][0], coords[i][1], RED(_minuteColor), GREEN(_minuteColor), BLUE(_minuteColor));
			else
				drawSquare(coords[i][0], coords[i][1], RED(_minuteBackColor), GREEN(_minuteBackColor), BLUE(_minuteBackColor));
		}
		
	}
	
	
	void drawBlip() {
		static int coords[12][2] = {
			{ 0, 0}, {-1, 0}, {-2, 0}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {2, 0}, {1, 0}
		};
		
		static int ticks = 0;
		
		
		int x = coords[ticks][0];
		int y = coords[ticks][1];
		
		int red = RED(_blipColor);
		int green = GREEN(_blipColor);
		int blue = BLUE(_blipColor);
		
		drawPixel(x - 2, y - 1, 0, 0, 0);
		drawPixel(x - 2, y - 0, 0, 0, 0);
		drawPixel(x + 1, y - 1, 0, 0, 0);
		drawPixel(x + 1, y - 0, 0, 0, 0);
		
		drawPixel(x, y, red, green, blue);
		drawPixel(x, y - 1, red, green, blue);
		drawPixel(x - 1, y - 1, red, green, blue);
		drawPixel(x - 1, y, red, green, blue);
		
		ticks = (ticks + 1) % 12;
		
		
	}
	
	
	void drawClock(int hours, int minutes, int seconds = 0) {
		drawHours(hours);
		drawMinutes(minutes, seconds);
	}
	
	inline virtual void idle() {
		drawBlip();
		usleep(25 * 1000);
		_canvas->refresh();
	}
	

	virtual void run(Timer &timer) {
		
		
		int hours   = 0;
		int minutes = 0;
		int seconds = 0;
		
		unsigned int ticks = 0;
		
		time_t t = time(0);
		struct tm *now = localtime(&t);
		
		while (!timer.expired()) {
			
			ticks++;
			
			
			time_t t = time(0);
			now = localtime(&t);
			
			if (now->tm_hour >= 23 || now->tm_hour <= 5) {
				_hourColor       = RGB(255, 0, 0);
				_hourBackColor   = RGB(70, 0, 0);
				_minuteColor     = RGB(255, 0, 0);
				_minuteBackColor = RGB(80, 0, 0);
				_blipColor       = RGB(0, 0, 255);
			}
			else {
				_hourColor       = RGB(255, 0, 0);
				_hourBackColor   = RGB(70, 0, 0);
				_minuteColor     = RGB(0, 0, 255);
				_minuteBackColor = RGB(0, 0, 80);
				_blipColor       = RGB(255, 255, 255);
			}
			
			now->tm_hour = now->tm_hour % 12;
			
			if (now->tm_min	< minutes) {
				
				while (!timer.expired() && --minutes > 0) {
					drawClock(hours, minutes, 0);
					idle();
					idle();
				}
			}
			
			if (now->tm_hour < hours) {
				
				while (!timer.expired() && --hours > 0) {
					drawClock(hours, minutes);
					idle();
					idle();
				}
			}
			
			if (!timer.expired()) {
				if (seconds != now->tm_sec) {
					hours   = now->tm_hour;
					minutes = now->tm_min;
					seconds = now->tm_sec;
					drawClock(hours, minutes, seconds);
				}
				
				idle();
			}
		}
	}
	
private:
	LogiMatrix *_canvas;
	uint32_t _hourColor;
	uint32_t _minuteColor;
	uint32_t _blipColor;
	uint32_t _hourBackColor;
	uint32_t _minuteBackColor;
	
	
};




int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	LogiMatrix matrix;
	Timer timer;
	
	int option = 0;
	int duration = -1;
	
	while ((option = getopt(argc, argv, "g:d:")) != -1) {
		switch (option) {
			case 'g':
				matrix.setGamma(atof(optarg));
				break;
			case 'd':
				timer.setDuration(atoi(optarg));
				break;
		}
	}
	
	ClockAnimation animation(&matrix);
	animation.run(timer);

	matrix.clear();
	matrix.refresh();
	
    return 0;
}


