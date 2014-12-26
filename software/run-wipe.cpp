#include <vector>

#include "globals.h"
	

using namespace std;

static const uint8_t gammaLut[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x02,
	0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
	0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
	0x02,0x02,0x02,0x02,0x02,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x04,
	0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
	0x04,0x04,0x04,0x04,0x04,0x04,0x05,0x05,
	0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,
	0x05,0x05,0x05,0x06,0x06,0x06,0x06,0x06,
	0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x07,
	0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,
	0x07,0x07,0x08,0x08,0x08,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x09,0x09,0x09,0x09,
	0x09,0x09,0x09,0x09,0x09,0x0a,0x0a,0x0a,
	0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0b,0x0b,
	0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0c,0x0c,
	0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0d,0x0d,
	0x0d,0x0d,0x0d,0x0d,0x0d,0x0e,0x0e,0x0e,
	0x0e,0x0e,0x0e,0x0e,0x0f,0x0f,0x0f,0x0f
};

#define DISPLAY_WIDTH 32
#define DISPLAY_HEIGHT 32

uint16_t gLevels[DISPLAY_HEIGHT][DISPLAY_WIDTH];


class Pattern
{
public:
	
	// constructor
	Pattern (const int32_t width, const int32_t height) :
	m_width(width), m_height(height) { }
	
	// destructor
	~Pattern (void) { }
	
	// reset to first frame in animation
	virtual void init (void) = 0;
	
	// calculate next frame in the animation
	virtual bool next (void) = 0;
	
	// get width and height
	void getDimensions (int32_t &width, int32_t &height) {
		width = m_width; height = m_height;
	}
	
	uint16_t translateHue (int32_t hue);
	uint16_t translateHueValue (int32_t hue, float value);
	
protected:
	const int32_t m_width;
	const int32_t m_height;
	
private:
};



#define MAKE_COLOR(r,g,b) (((r)&0xf)<<8)+(((g)&0xf)<<4)+((b)&0xf)

//---------------------------------------------------------------------------------------------
// convert a hue from 0 to 95 to its 12-bit RGB color
//
// hue: 0 = red, 32 = blue, 64 = green
//

uint16_t Pattern::translateHue (int32_t hue)
{
	uint8_t hi, lo;
	uint8_t r, g, b;
	
	hi = hue >> 4;
	lo = ((hue & 0xf) << 4) | (hue & 0xf);
	
	switch (hi) {
		case 0: r = 0xff;    g = 0;       b = lo;      break;
		case 1: r = 0xff-lo, g = 0,       b = 0xff;    break;
		case 2: r = 0,       g = lo,      b = 0xff;    break;
		case 3: r = 0,       g = 0xff,    b = 0xff-lo; break;
		case 4: r = lo,      g = 0xff,    b = 0;       break;
		case 5: r = 0xff,    g = 0xff-lo, b = 0;       break;
	}
	
	r = gammaLut[r];
	g = gammaLut[g];
	b = gammaLut[b];
	
	return MAKE_COLOR (r,g,b);
}


//---------------------------------------------------------------------------------------------
// convert a hue from 0 to 95 and a brightnewss from 0 to 1.0 to its 12-bit RGB color
//
// hue: 0 = red, 32 = blue, 64 = green
// value: 0 = off, 1.0 = 100%
//

uint16_t Pattern::translateHueValue (int32_t hue, float value)
{
	uint8_t hi, lo;
	uint8_t r, g, b;
	
	hi = hue >> 4;
	lo = ((hue & 0xf) << 4) | (hue & 0xf);
	
	switch (hi) {
		case 0: r = 0xff;    g = 0;       b = lo;      break;
		case 1: r = 0xff-lo, g = 0,       b = 0xff;    break;
		case 2: r = 0,       g = lo,      b = 0xff;    break;
		case 3: r = 0,       g = 0xff,    b = 0xff-lo; break;
		case 4: r = lo,      g = 0xff,    b = 0;       break;
		case 5: r = 0xff,    g = 0xff-lo, b = 0;       break;
	}
	
	r = ((float)r + 0.5) * value;
	g = ((float)g + 0.5) * value;
	b = ((float)b + 0.5) * value;
	
	r = gammaLut[r];
	g = gammaLut[g];
	b = gammaLut[b];
	
	return MAKE_COLOR (r,g,b);
}
class Wipe : public Pattern
{
public:
	
	// constructor
	Wipe (const int32_t width, const int32_t height);
	
	// constructor
	Wipe
	(
	 const int32_t width, const int32_t height,
	 const int32_t direction, const int32_t delay
	 );
	
	// destructor
	~Wipe (void);
	
	// reset to first frame in animation
	void init (void);
	
	// calculate next frame in the animation
	bool next (void);
	
	// get / set direction of wipe
	// 0 = L to R, 1 = R to L, 2 = T to B, 3 = B to T
	int32_t getDirection (void) {
		return m_direction;
	}
	void setDirection (const int32_t direction) {
		m_direction = direction;
	}
	
	// get / set delay between steps
	int32_t getDelay (void) {
		return m_delay;
	}
	void setDelay (const int32_t delay) {
		m_delay = delay;
	}
	
private:
	
	int32_t m_direction;
	int32_t m_delay;
	int32_t m_state;
	int32_t m_color;
	int32_t m_timer;
};



static const uint16_t wipeColors[7] = {
	0xf00, 0xff0, 0x0f0, 0x0ff, 0x00f, 0xf0f, 0xccc
};


//---------------------------------------------------------------------------------------------
// constructors
//

Wipe::Wipe
(
 const int32_t width, const int32_t height
 ) :
Pattern (width, height),
m_direction(0), m_delay(2)
{
}


Wipe::Wipe
(
 const int32_t width, const int32_t height,
	const int32_t direction, const int32_t delay
 ) :
Pattern (width, height),
m_direction(direction), m_delay(delay)
{
}


//---------------------------------------------------------------------------------------------
// destructor
//

Wipe::~Wipe (void)
{
}


//---------------------------------------------------------------------------------------------
// init -- reset to first frame in animation
//

void Wipe::init (void)
{
	m_state = 0;
	m_color = 0;
	m_timer = 0;
}


//---------------------------------------------------------------------------------------------
// next -- calculate next frame in animation
//

bool Wipe::next (void)
{
	int32_t row, col;
	
	if (m_timer == 0) {
		for (row = 0; row < m_height; row++) {
			for (col = 0; col < m_width; col++) {
				
				switch (m_direction) {
						
					case 0: // left to right
						if (col == m_state) {
							gLevels[row][col] = wipeColors[m_color];
						}
						break;
						
					case 1: // right to left
						if (col == m_state) {
							gLevels[row][m_width - 1 - col] = wipeColors[m_color];
						}
						break;
						
					case 2: // top to bottom
						if (row == m_state) {
							gLevels[row][col] = wipeColors[m_color];
						}
						break;
						
					case 3: // bottom to top
						if (row == m_state) {
							gLevels[m_height - 1 - row][col] = wipeColors[m_color];
						}
						break;
				}
			}
		}
		
		m_state++;
		if (((m_direction <= 1) && (m_state == m_width)) ||
			((m_direction >= 2) && (m_state == m_height))) {
			m_state = 0;
			m_color++;
			if (m_color == 7) {
				m_color = 0;
			}
		}
	}
	
	m_timer++;
	if (m_timer >= m_delay) {
		m_timer = 0;
	}
	
	return (m_timer == 0) && (m_state == 0);
}




int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);
	LogiMatrix matrix;
	Timer timer;
	
	int option = 0;
	
	while ((option = getopt(argc, argv, "d:")) != -1) {
		switch (option) {
			case 'd':
				timer.duration(atoi(optarg));
				break;
		}
	}
	
	
	Pattern *pattern =  new Wipe (DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 2);
	pattern->init ();
	
	while (!timer.expired()) {
		
		// write levels to display
		matrix.fill( (uint16_t *)gLevels);
		matrix.refresh();
		
		pattern->next();
		
	}
	
	matrix.clear();
	matrix.refresh();
	
	
	return 0;
}




