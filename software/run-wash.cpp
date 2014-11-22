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



class Wash : public Pattern
{
public:
	
	// constructor
	Wash (const int32_t width, const int32_t height);
	
	// constructor
	Wash
	(
	 const int32_t width, const int32_t height,
	 const float step, const float scale, const float angle
	 );
	
	// destructor
	~Wash (void);
	
	// reset to first frame in animation
	void init (void);
	
	// calculate next frame in the animation
	bool next (void);
	
	// get / set step, controls speed of wash
	float getStep (void) {
		return m_step;
	}
	void setStep (const float step) {
		m_step = step;
	}
	
	// get / set scale, controls width of wash
	float getScale (void) {
		return m_scale;
	}
	void setScale (const float scale) {
		m_scale = scale;
	}
	
	// get / set angle
	float getAngle (void) {
		return m_angle;
	}
	void setAngle (const float angle) {
		m_angle = angle;
	}
	
private:
	
	float m_step;
	float m_scale;
	float m_angle;
	float m_state;
};




//---------------------------------------------------------------------------------------------
// constructors
//

Wash::Wash
(
 const int32_t width, const int32_t height
 ) :
Pattern (width, height),
m_step(1.0), m_scale(1.0)
{
}


Wash::Wash
(
 const int32_t width, const int32_t height,
	const float step, const float scale, const float angle
 ) :
Pattern (width, height),
m_step(step), m_scale(scale), m_angle(angle)
{
}


//---------------------------------------------------------------------------------------------
// destructor
//

Wash::~Wash (void)
{
}


//---------------------------------------------------------------------------------------------
// init -- reset to first frame in animation
//

void Wash::init (void)
{
	m_state = 0;
}


//---------------------------------------------------------------------------------------------
// next -- calculate next frame in animation
//

bool Wash::next (void)
{
	int32_t row, col, hue;
	
	float rads = m_angle*M_PI/180.0;
	for (row = 0; row < m_height; row++) {
		float x = row - ((m_width-1.0)/2.0);
		for (col = 0; col < m_width; col++) {
			float y = ((m_height-1.0)/2.0) - col;
			float xp = x * cos (rads) - y * sin (rads);
			// float yp = x * sin (rads) + y * cos (rads);
			hue = m_state + m_scale * xp + 0.5;
			while (hue < 0) hue += 96;
			while (hue >= 96) hue -= 96;
			gLevels[row][col] = translateHue (hue);
		}
	}
	
	m_state = fmod ((m_state + m_step), 96.0);
	
	return (m_state == 0);
}



int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	LogiMatrix matrix;
	Timer timer;
	
	int option = 0;
	
	timer.setDuration(60);
	
	while ((option = getopt(argc, argv, "d:")) != -1) {
		switch (option) {
			case 'd':
				timer.setDuration(atoi(optarg));
				break;
		}
	}

	Pattern *pattern = new Wash (DISPLAY_WIDTH, DISPLAY_HEIGHT, 1.0, 1.0, 0);
	pattern->init ();
	

	while (!timer.expired()) {
		matrix.fill( (uint16_t *)gLevels);
		matrix.refresh();
		
		pattern->next();
	}
	
	matrix.clear();
	matrix.refresh();
	
    return 0;
}



