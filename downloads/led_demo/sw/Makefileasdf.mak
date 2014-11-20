#==============================================================================================
# LED Matrix Animated Pattern Generator
# Copyright 2014 by Glen Akins.
# All rights reserved.
# 
# Set editor width to 96 and tab stop to 4.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#==============================================================================================

all: runcircle runperlin runwash runtwinkle runwipe blank picture



runperlin: runperlin.o pattern.o perlin.o 
	g++ -o runperlin runperlin.o pattern.o perlin.o 

runperlin.o: runperlin.cpp globals.h pattern.h perlin.h
	g++ -c runperlin.cpp

pattern.o: pattern.cpp globals.h gammalut.h pattern.h
	g++ -c pattern.cpp

perlin.o: perlin.cpp globals.h pattern.h perlin.h
	g++ -c perlin.cpp




clean:
	rm -f runcircle runperlin runwash runtwinkle runwipe blank picture runcircle.o runperlin.o runwash.o runtwinkle.o pattern.o circle.o perlin.o wash.o twinkle.o wipe.o runwipe.o
