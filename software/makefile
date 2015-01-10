
CCFLAGS = `Magick++-config --cxxflags --cppflags`  -O2 -Wall -Wno-unused-value -Wno-unused-result -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-unused-variable -Wno-reorder
LDFLAGS = `Magick++-config --ldflags --libs`
SHARED  = globals.h includes.h animation.h canvas.h
TARGETS = run-rain run-image run-animation run-text run-test run-rgb run-static run-clock run-hue-block run-circle run-perlin run-twinkle run-wash run-life run-wipe

all: $(TARGETS)

clean:
	rm $(TARGETS)


run-rain: run-rain.cpp $(SHARED) 
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-image: run-image.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-animation: run-animation.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-text: run-text.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-test: run-test.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-rgb: run-rgb.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-static: run-static.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-clock: run-clock.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-hue-block: run-hue-block.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-circle: run-circle.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-perlin: run-perlin.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-twinkle: run-twinkle.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-wash: run-wash.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-life: run-life.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)

run-wipe: run-wipe.cpp $(SHARED)
	g++  $(CCFLAGS) -o $@ $@.cpp $(LDFLAGS)


