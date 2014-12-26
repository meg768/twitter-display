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
typedef struct {
	uint8_t state;
	uint8_t hue;
	uint8_t percent;
} Twinkler;

class Twinkle : public Pattern
{
public:
	
	// constructor
	Twinkle (const int32_t width, const int32_t height);
	
	// destructor
	~Twinkle (void);
	
	// reset to first frame in animation
	void init (void);
	
	// calculate next frame in the animation
	bool next (void);
	
private:
	
	void resize (void);
	vector<vector<Twinkler> > m_twinklers;
};


//---------------------------------------------------------------------------------------------
// constructors
//

Twinkle::Twinkle
(
 const int32_t width, const int32_t height
 ) :
Pattern (width, height)
{
	this->resize ();
}


//---------------------------------------------------------------------------------------------
// destructor
//

Twinkle::~Twinkle (void)
{
}


//---------------------------------------------------------------------------------------------
// resize state, hue, value arrays
//

void Twinkle::resize (void)
{
	m_twinklers.resize (m_height);
	for (int32_t row = 0; row < m_height; row++) {
		m_twinklers[row].resize (m_width);
	}
}


//---------------------------------------------------------------------------------------------
// init -- reset to first frame in animation
//

void Twinkle::init (void)
{
	for (int32_t row = 0; row < m_height; row++) {
		for (int32_t col = 0; col < m_width; col++) {
			m_twinklers[row][col].state = 0;
			m_twinklers[row][col].hue = 0;
			m_twinklers[row][col].percent = 0;
		}
	}
}


//---------------------------------------------------------------------------------------------
// next -- calculate next frame in animation
//

bool Twinkle::next (void)
{
	for (int32_t row = 0; row < m_height; row++) {
		for (int32_t col = 0; col < m_width; col++) {
			int32_t r = rand();
			switch (m_twinklers[row][col].state) {
					
				case 0: // off
					if (r < (0.025*RAND_MAX)) {
						m_twinklers[row][col].state = 1;
						m_twinklers[row][col].hue = r % 96;
						m_twinklers[row][col].percent = 10;
						gLevels[row][col] = translateHueValue (m_twinklers[row][col].hue,
															   (float)m_twinklers[row][col].percent/100.0);
					}
					break;
					
				case 1: // ramp up
					m_twinklers[row][col].percent += 10;
					gLevels[row][col] = translateHueValue (m_twinklers[row][col].hue,
														   (float)m_twinklers[row][col].percent/100.0);
					if (m_twinklers[row][col].percent == 100) {
						m_twinklers[row][col].state = 2;
					}
					break;
				case 2: // on
					if (r < (0.20*RAND_MAX)) {
						m_twinklers[row][col].state = 3;
					}
					break;
				case 3: // ramp down
					m_twinklers[row][col].percent -= 10;
					gLevels[row][col] = translateHueValue (m_twinklers[row][col].hue,
														   (float)m_twinklers[row][col].percent/100.0);
					if (m_twinklers[row][col].percent == 0) {
						m_twinklers[row][col].state = 0;
					}
					break;
					
			}
		}
	}
	
	return true;
}




int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);
	
	LogiMatrix matrix;
	Timer timer;

	timer.setDuration(60);
	
	int option = 0;
	
	while ((option = getopt(argc, argv, "d:")) != -1) {
		switch (option) {
			case 'd':
				timer.duration(atoi(optarg));
				break;
		}
	}

	Pattern *pattern = new Twinkle (DISPLAY_WIDTH, DISPLAY_HEIGHT);
	pattern->init();

	while (!timer.expired()) {
		matrix.fill( (uint16_t *)gLevels);
		matrix.refresh();
		
		pattern->next();
	}
	
	
	matrix.clear();
	matrix.refresh();
	
    return 0;
}



