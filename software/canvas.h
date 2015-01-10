#ifndef _canvas_h
#define _canvas_h


typedef struct {
	uint8_t red;     // 0 - 255 */
	uint8_t green;   // 0 - 255 */
	uint8_t blue;    // 0 - 255 */
	
} RGB;

typedef struct {
	uint16_t hue;       // 0 - 360  */
	uint8_t saturation; // 0 - 100  */
	uint8_t luminance;  // 0 - 100  */
} HSL;


static void *__canvas = 0;

class Canvas {
	
public:
	Canvas(int width = 32, int height = 32) {
		__canvas = this;
		
		// Trap ctrl-c to call quit function
		signal(SIGINT, Canvas::quit);
		signal(SIGKILL, Canvas::quit);
		
		_width  = width;
		_height = height;
		
		_matrix = (uint16_t *)malloc(sizeof(uint16_t) * _width * _height);
		
		setGamma(0);
		openDevice();
		
		clear();
		refresh();
	}
	
	
	~Canvas() {
		closeDevice();
		free(_matrix);
	}
	
	static void quit(int sig)
	{
		Canvas *canvas = (Canvas *)__canvas;
		
		canvas->clear();
		canvas->refresh();
		
		exit(-1);
	}
	
	uint16_t luminance_cie1931(uint8_t c) {
		float out_factor = 15.0;
		float v = c * 100.0 / 255.0;
		return out_factor * ((v <= 8) ? v / 902.3 : pow((v + 16) / 116.0, 3));
	}
	
	void setGamma(float gamma) {
		static  uint8_t gamma_org[] = {
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
		
		static uint8_t gamma_org2[] = {
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
			1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
			2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
			5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
			115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
			144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
			177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
			215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
		
		for (int i = 0; i < 256; i++) {
			if (gamma == 0.0)
				_gamma[i] = gamma_org[i];
			else if (gamma == -1.0)
				_gamma[i] = gamma_org2[i];
			else if (gamma == -2.0)
				_gamma[i] = luminance_cie1931(i);
			else if (gamma == -3.0) {
				float factor = (float)i / 256.0;
				float value = 0;
				if (i > 0.008856) {
					value = (116.0 * pow(factor, 1.0/3.0) - 16.0);
				}
				else {
					value = 903.3 * factor;
					
				}
				_gamma[i] = (int)((value * 15.0 / 100.0) + 0.5);
			}
			else
				_gamma[i] = (int)(pow((float)i / (float)255.0, gamma) * 15.0 + 0.5);
			
		}
		
	}
	
	inline void setPixel(int x, int y, uint16_t color) {
		
		if (x >= 0 && y >= 0 && x < _width && y < _height) {
			_matrix[y * _width + x] = color;
		}
	}
	
	inline void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
		
		if (x >= 0 && y >= 0 && x < _width && y < _height) {
			
			red   = _gamma[red];
			green = _gamma[green];
			blue  = _gamma[blue];
			
			_matrix[y * _width + x] = (red << 8) | (green << 4) | blue;
		}
	}

	inline void setPixel(int x, int y, RGB color) {
		
		if (x >= 0 && y >= 0 && x < _width && y < _height) {
			
			uint8_t red, green, blue;
			
			red   = _gamma[color.red];
			green = _gamma[color.green];
			blue  = _gamma[color.blue];
			
			_matrix[y * _width + x] = (red << 8) | (green << 4) | blue;
		}
	}

	inline void setPixel(int x, int y, HSL color) {
		
		if (x >= 0 && y >= 0 && x < _width && y < _height) {
			
			uint8_t red, green, blue;
			
			HslToRgb((double)color.hue, (double)color.saturation / 100.0, (double)color.luminance / 100.0, red, green, blue);
			
			red   = _gamma[red];
			green = _gamma[green];
			blue  = _gamma[blue];
			
			_matrix[y * _width + x] = (red << 8) | (green << 4) | blue;
		}
	}

	void HslToRgb(double h, double s, double v, uint8_t &red, uint8_t &green, uint8_t &blue)
	{
		double hh = 0, p = 0, q = 0, t = 0, ff = 0;
		double r = 0, g = 0, b = 0;
		long i = 0;
		
		if (s <= 0.0) {
			r = v, g = v, b = v;
		}
		else {
			hh = h;
			
			while (hh < 0)
				hh += 360.0;
			
			while (hh >= 360.0)
				hh -= 360.0;
			
			hh = hh / 60.0;
			i  = (long)hh;
			ff = hh - i;
			
			p = v * (1.0 - s);
			q = v * (1.0 - (s * ff));
			t = v * (1.0 - (s * (1.0 - ff)));
			
			switch(i) {
				case 0:
					r = v, g = t, b = p;
					break;
				case 1:
					r = q, g = v, b = p;
					break;
				case 2:
					r = p, g = v, b = t;
					break;
					
				case 3:
					r = p, g = q, b = v;
					break;
				case 4:
					r = t, g = p, b = v;
					break;
				default:
					r = v, g = p, b = q;
					break;
			}
			
		}
		
		red   = (uint8_t)(r * 255.0);
		green = (uint8_t)(g * 255.0);
		blue  = (uint8_t)(b * 255.0);
	}

	void drawImage(Magick::Image &image, int x, int y, int offsetX, int offsetY) {
		
		int screenWidth  = _width;
		int screenHeight = _height;
		
		int width        = screenWidth - x;
		int height       = screenHeight - y;
		
		const Magick::PixelPacket *pixels = image.getConstPixels(offsetX, offsetY, width, height);
		
		for (int row = y; row < height; row++) {
			for (int col = x; col < width; col++) {
				uint8_t red   = pixels->red;
				uint8_t green = pixels->green;
				uint8_t blue  = pixels->blue;
				setPixel(col, row, red, green, blue);
				pixels++;
			}
		}
	}
	
	void drawImage(Magick::Image &image) {
		drawImage(image, 0, 0, 0, 0);
	}
	
	inline int width() {
		return _width;
	}
	
	inline int height() {
		return _height;
	}

	void fill(uint16_t *buffer) {
		memcpy(_matrix, buffer, sizeof(uint16_t) * _width * _height);
	};
	
	void fill(uint8_t red, uint8_t green, uint8_t blue) {
		
		for (int row = 0; row < _height; row++) {
			for (int col = 0; col < _width; col++) {
				setPixel(col, row, red, green, blue);
			}
		}
		
	}
	
	void clear() {
		// Clear display
		memset(_matrix, 0, sizeof(uint16_t) * _width * _height);
	}
	

	void refresh() {
		static int nBuffer = 0;
		
		const int FPGA_PANEL_ADDR_REG   = 0x0008;
		const int FPGA_PANEL_DATA_REG   = 0x0009;
		const int FPGA_PANEL_BUFFER_REG = 0x000A;
		
		// Ping pong between buffers
		writeToDevice(FPGA_PANEL_ADDR_REG, nBuffer == 0 ? 0x0000 : 0x0400);
		
		uint16_t *mp = _matrix;
		
		// Write entire screen buffer to device
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				writeToDevice(FPGA_PANEL_DATA_REG, *mp++);
			}
		}
		
		// Make that buffer active
		writeToDevice(FPGA_PANEL_BUFFER_REG, nBuffer == 0 ? 0x0000 : 0x0001);
		
		// Switch buffer
		nBuffer = !nBuffer;
	}
	
private:
	inline void openDevice() {
		// open fpga memory device
		_device = open("/dev/logibone_mem", O_RDWR | O_SYNC);
	}
	
	inline void closeDevice() {
		close(_device);
	}
	
	inline void writeToDevice(uint16_t addr, uint16_t data) {
		(void *)pwrite(_device, &data, 2, addr);
	}
	
	int _width;
	int _height;
	int _device;
	
	uint16_t *_matrix;
	uint8_t _gamma[256];
};


#endif
