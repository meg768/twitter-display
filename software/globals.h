#ifndef _globals_h
#define _globals_h

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <memory.h>
#include <math.h>

#include <vector>


#include <Magick++.h>

////////////////////////////////////////////////////////////////////////////////////////

class Timer {
	
public:
	Timer(int duration = -1) {
		_duration  = duration;
		_startTime = time(NULL);
	}
	
	void setDuration(int duration) {
		_duration = duration;
	}
	
	int expired() {
		if (_duration > 0) {
			if (time(NULL) - _startTime > _duration) {
				return true;
			}
		}
		
		return false;
	}
	
private:
	time_t _startTime;
	int _duration;
};


////////////////////////////////////////////////////////////////////////////////////////

static void *__matrix = 0;

class LogiMatrix {
	
public:
	LogiMatrix(int width = 32, int height = 32) {
		__matrix = this;
		
		// Trap ctrl-c to call quit function
		signal(SIGINT, LogiMatrix::quit);
		signal(SIGKILL, LogiMatrix::quit);
		
		_width  = width;
		_height = height;
		
		_matrix = (uint16_t *)malloc(sizeof(uint16_t) * _width * _height);
		
		setGamma(0);
		openDevice();
		
		clear();
		refresh();
	}
	
	
	~LogiMatrix() {
		closeDevice();
		
		free(_matrix);
	}
	
	static void quit(int sig)
	{
		LogiMatrix *matrix = (LogiMatrix *)__matrix;
		
		matrix->clear();
		matrix->refresh();
		
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
		
		for (int i = 0; i < 256; i++) {
			if (gamma == 0)
				_gamma[i] = gamma_org[i];
			else if (gamma == 1)
				_gamma[i] = luminance_cie1931(i);
			else if (gamma == 100.0) {
				float factor = (float)i / 256.0;
				float value = 0;
				if (i > 0.008856) {
					value = (116.0 * pow(factor, 1.0/3.0) - 16.0);
				}
				else {
					value = 903.3 * factor;
					
				}
				_gamma[i] = (int)((value * 15.0 / 100.0) + 0.5);
				//L* = 116(Y/Yn)^1/3 - 16 , Y/Yn > 0.008856
				//L* = 903.3(Y/Yn), Y/Yn <= 0.008856
			}
			else
				_gamma[i] = (int)(pow((float)i / (float)255.0, gamma) * 15.0 + 0.5);
			
			//printf("%02d ", _gamma[i]);
			//if ((i % 16) == 15)
			//	printf("\n");
		}
		
		//printf("---\n");
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
	
	void drawImage(Magick::Image &image, int x, int y, int offsetX, int offsetY) {
		
		int screenWidth  = _width;
		int screenHeight = _height;
		
		int width        = screenWidth - x;
		int height       = screenHeight - y;
		
		const Magick::PixelPacket *pixels = image.getConstPixels(offsetX, offsetY, width, height);
		
		for (int row = y; row < height; row++) {
			for (int col = x; col < width; col++) {
				setPixel(col, row, pixels->red, pixels->green, pixels->blue);
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
