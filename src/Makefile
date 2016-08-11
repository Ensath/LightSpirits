# Copyright (c) 2016 Noah Freed
# This program is available under the "zlib license". Please see the file COPYING.
# This program uses code from Will Usher's TwinklebearDev SDL 2.0 tutorial. Check it out at http://www.willusher.io/pages/sdl2/.

CXX = g++
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib -lSDL2_image
SDL_INCLUDE = -I/usr/include/SDL2 -I/home/noah/oss/LightSpirits/include

# You may need to change -std=c++11 to -std=c++0x if your compiler is a bit older
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = LightSpirits

all: $(EXE)

$(EXE): main.o
	$(CXX) $< $(LDFLAGS) -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
