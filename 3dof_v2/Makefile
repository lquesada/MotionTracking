LDFLAGS=-lSDL -lSDL_gfx -lSDL_ttf -lcv -lhighgui
CCFLAGS=-O3

HEADERS=config.h functions.h image.h matrix.h inputcamera.h outputscreen.h point.h rectangle.h

BINS=capture process processbig
all: $(BINS)

%.o:%.c $(HEADERS)
	gcc $(CCFLAGS) -c -o $@ $<

capture: capture.o inputcamera.o outputscreen.o matrix.o

process: process.o inputcamera.o outputscreen.o matrix.o functions.o rectangle.o point.o

processbig: processbig.o inputcamera.o outputscreen.o matrix.o functions.o rectangle.o point.o

clean:
	rm -f *.o $(BINS)
