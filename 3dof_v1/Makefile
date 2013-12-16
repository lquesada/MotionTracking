LDFLAGS=-lSDL -lSDL_gfx -lSDL_ttf -lcv -lhighgui
CCFLAGS=-O3

all: capture process

VMatrix.o: VMatrix.cpp VMatrix.h Vector.h Pixel.h RGBPixel.h Image.template Filter.h
	g++ $(CCFLAGS) -c -o VMatrix.o VMatrix.cpp

InputCamera.o: InputCamera.h InputCamera.cpp Image.template RGBPixel.h Filter.h
	g++ $(CCFLAGS) -c -o InputCamera.o InputCamera.cpp

Filter.o: Filter.h Filter.cpp
	g++ $(CCFLAGS) -c -o Filter.o Filter.cpp

Axis.o: Axis.h Axis.cpp Output.h
	g++ $(CCFLAGS) -c -o Axis.o Axis.cpp

Output.o: Output.h Output.cpp VMatrix.h Image.template RGBPixel.h Vector.h
	g++ $(CCFLAGS) -c -o Output.o Output.cpp

OutputSDL.o: OutputSDL.h OutputSDL.cpp Image.template RGBPixel.h Filter.h Output.h Axis.h
	g++ $(CCFLAGS) -c -o OutputSDL.o OutputSDL.cpp

capture.o: capture.cpp Image.template VMatrix.h BWPixel.h RGBPixel.h InputCamera.h OutputSDL.h Filter.h Output.h
	g++ $(CCFLAGS) -c -o capture.o capture.cpp

capture: VMatrix.o InputCamera.o OutputSDL.o capture.o Filter.o Output.o
	g++ $(LDFLAGS) -o capture VMatrix.o InputCamera.o OutputSDL.o Filter.o Output.o capture.o

process.o: process.cpp Image.template VMatrix.h BWPixel.h RGBPixel.h InputCamera.h OutputSDL.h Filter.h Output.h Axis.h
	g++ $(CCFLAGS) -c -o process.o process.cpp

process: VMatrix.o InputCamera.o OutputSDL.o process.o Filter.o Output.o Axis.o
	g++ $(LDFLAGS) -o process VMatrix.o InputCamera.o OutputSDL.o Filter.o Output.o Axis.o process.o

clean:
	rm -f capture process *.o
