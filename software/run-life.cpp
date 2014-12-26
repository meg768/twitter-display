#include "globals.h"

using namespace std;

class GameLife  {
public:
	GameLife(LogiMatrix *m, int delay_ms=25, bool torus=true)
	: delay_ms_(delay_ms), torus_(torus) {
		_canvas = m;

		width_ = canvas()->width();
		height_ = canvas()->height();
		
		srand(time(NULL));
		
		// Allocate memory
		values_ = new int*[width_];
		for (int x=0; x<width_; ++x) {
			values_[x] = new int[height_];
		}
		newValues_ = new int*[width_];
		for (int x=0; x<width_; ++x) {
			newValues_[x] = new int[height_];
		}
		
		// Init values randomly
		srand(time(NULL));
		for (int x=0; x<width_; ++x) {
			for (int y=0; y<height_; ++y) {
				values_[x][y]=rand()%2;
			}
		}
		r_ = rand()%255;
		g_ = rand()%255;
		b_ = rand()%255;
		
		if (r_<150 && g_<150 && b_<150) {
			int c = rand()%3;
			switch (c) {
				case 0:
					r_ = 255;
					break;
				case 1:
					g_ = 255;
					break;
				case 2:
					b_ = 255;
					break;
			}
		}
	}
	
	~GameLife() {
		for (int x=0; x<width_; ++x) {
			delete [] values_[x];
		}
		delete [] values_;
		for (int x=0; x<width_; ++x) {
			delete [] newValues_[x];
		}
		delete [] newValues_;
	}
	
	LogiMatrix *canvas() {
		return _canvas;
	}
	

	void run(Timer &timer) {
		while (!timer.expired()) {
			
			updateValues();
			
			for (int x=0; x<width_; ++x) {
				for (int y=0; y<height_; ++y) {
					if (values_[x][y])
						canvas()->setPixel(x, y, r_, g_, b_);
					else
						canvas()->setPixel(x, y, 0, 0, 0);
				}
			}
			_canvas->refresh();
			usleep(delay_ms_ * 1000); // ms
		}
	}
	
private:
	int numAliveNeighbours(int x, int y) {
		int num=0;
		if (torus_) {
			// Edges are connected (torus)
			num += values_[(x-1+width_)%width_][(y-1+height_)%height_];
			num += values_[(x-1+width_)%width_][y                    ];
			num += values_[(x-1+width_)%width_][(y+1        )%height_];
			num += values_[(x+1       )%width_][(y-1+height_)%height_];
			num += values_[(x+1       )%width_][y                    ];
			num += values_[(x+1       )%width_][(y+1        )%height_];
			num += values_[x                  ][(y-1+height_)%height_];
			num += values_[x                  ][(y+1        )%height_];
		}
		else {
			// Edges are not connected (no torus)
			if (x>0) {
				if (y>0)
					num += values_[x-1][y-1];
				if (y<height_-1)
					num += values_[x-1][y+1];
				num += values_[x-1][y];
			}
			if (x<width_-1) {
				if (y>0)
					num += values_[x+1][y-1];
				if (y<31)
					num += values_[x+1][y+1];
				num += values_[x+1][y];
			}
			if (y>0)
				num += values_[x][y-1];
			if (y<height_-1)
				num += values_[x][y+1];
		}
		return num;
	}
	
	void updateValues() {
		// Copy values to newValues
		for (int x=0; x<width_; ++x) {
			for (int y=0; y<height_; ++y) {
				newValues_[x][y] = values_[x][y];
			}
		}
		// update newValues based on values
		for (int x=0; x<width_; ++x) {
			for (int y=0; y<height_; ++y) {
				int num = numAliveNeighbours(x,y);
				if (values_[x][y]) {
					// cell is alive
					if (num < 2 || num > 3)
						newValues_[x][y] = 0;
				}
				else {
					// cell is dead
					if (num == 3)
						newValues_[x][y] = 1;
				}
			}
		}
		// copy newValues to values
		for (int x=0; x<width_; ++x) {
			for (int y=0; y<height_; ++y) {
				values_[x][y] = newValues_[x][y];
			}
		}
	}
	
	int** values_;
	int** newValues_;
	int delay_ms_;
	int r_;
	int g_;
	int b_;
	int width_;
	int height_;
	bool torus_;
	LogiMatrix *_canvas;
};



int main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	LogiMatrix matrix;
	Timer timer;
	
	int option = 0;
	int duration = 60;
	
	while ((option = getopt(argc, argv, "g:d:")) != -1) {
		switch (option) {
			case 'g':
				matrix.setGamma(atof(optarg));
				break;
			case 'd':
				timer.duration(atoi(optarg));
				break;
		}
	}
	GameLife animation(&matrix);
	animation.run(timer);

	matrix.clear();
	matrix.refresh();
	
    return 0;
}



