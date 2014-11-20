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


class Circle : public Pattern
{
public:
	
	// constructor
	Circle (const int32_t width, const int32_t height);
	
	// constructor
	Circle
	(
	 const int32_t width, const int32_t height,
	 const float center_x, const float center_y,
	 const float speed, const float scale
	 );
	
	// destructor
	~Circle (void);
	
	// reset to first frame in animation
	void init (void);
	
	// calculate next frame in the animation
	bool next (void);
	
	// get / set center of circle
	void getCenter (float &x, float &y) {
		x = m_center_x; y = m_center_y;
	}
	void setCenter (const float x, const float y) {
		m_center_x = x; m_center_y = y;
		calculateDistanceLut ();
	}
	
	// get / set scale of the circle
	// 0.5 -> diameter = 2x width of display
	// 1.0 -> diameter = width of panel
	// 2.0 -> diameter = 1/2 width of display
	float getScale (void) {
		return m_scale;
	}
	void setScale (const float scale) {
		m_scale = scale;
	}
	
	// get set speed
	// this is the increment added/subtracted from the internal state
	// variable after computing each frame of the animation
	float getSpeed (void) {
		return m_speed;
	}
	void setSpeed (const float speed) {
		m_speed = speed;
	}
	
private:
	
	float m_speed;
	float m_scale;
	float m_center_x;
	float m_center_y;
	float m_state;
	
	void calculateDistanceLut (void);
	vector<vector<float> >  m_distance_lut;
	
};









//---------------------------------------------------------------------------------------------
// constructors
//

Circle::Circle
(
 const int32_t width, const int32_t height
 ) :
Pattern (width, height),
m_center_x((width-1.0)/2.0), m_center_y((height-1.0)/2.0),
m_speed(1.0), m_scale(1.0)
{
	calculateDistanceLut ();
}


Circle::Circle
(
 const int32_t width, const int32_t height,
 const float center_x, const float center_y,
 const float speed, const float scale
 ) :
Pattern (width, height),
m_center_x(center_x), m_center_y(center_y),
m_speed(speed), m_scale(scale)
{
	calculateDistanceLut ();
}


//---------------------------------------------------------------------------------------------
// destructor
//

Circle::~Circle (void)
{
}


//---------------------------------------------------------------------------------------------
// init -- reset to first frame in animation
//

void Circle::init (void)
{
	m_state = 0;
}


//---------------------------------------------------------------------------------------------
// next -- calculate next frame in animation
//

bool Circle::next (void)
{
	int32_t row, col, distance, hue;
	
	for (row = 0; row < m_height; row++) {
		for (col = 0; col < m_width; col++) {
			distance = m_scale * 96.0 * m_distance_lut[col][row];
			hue = m_state - distance;
			while (hue < 0) hue += 96;
			while (hue >= 96) hue -= 96;
			gLevels[row][col] = translateHue (hue);
		}
	}
	
	m_state = m_state + m_speed;
	if (m_state < 0) m_state += 96.0;
	if (m_state >= 96) m_state -= 96.0;
	
	return (m_state == 0);
}


//---------------------------------------------------------------------------------------------
// calculateDistanceLut
//

void Circle::calculateDistanceLut (void)
{
	// normalize pattern width to a diameter equal to display width
	float tmp_x = (m_width-1.0) / 2.0;
	float tmp_y = (m_width-1.0) / 2.0;
	float norm = sqrt (tmp_x*tmp_x + tmp_y*tmp_y);
	
	// resize lookup table width if needed
	if (this->m_width != m_distance_lut.size ()) {
		m_distance_lut.resize (this->m_width);
	}
	
	// iterate over each column
	for (int32_t col = 0; col < this->m_width; col++) {
		
		// resize lookup table height if needed
		if (this->m_height != m_distance_lut[col].size ()) {
			m_distance_lut[col].resize (this->m_height);
		}
		
		// iterate over each row
		for (int32_t row = 0; row < this->m_height; row++) {
			
			// find and save normalized distance from each point to center of circle
			float x = col - m_center_x;
			float y = row - m_center_y;
			float distance = sqrt (x*x + y*y) / norm;
			m_distance_lut[col][row] = distance;
		}
	}
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
				timer.setDuration(atoi(optarg));
				break;
		}
	}
	
	
	Pattern *pattern = new Circle (DISPLAY_WIDTH, DISPLAY_HEIGHT,
								   (DISPLAY_WIDTH - 1.0) / 2.0 -4, (DISPLAY_HEIGHT - 1.0) / 2.0 + 4,
								   1.0, 0.75);
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



