BIN=openglcamara osgcamara osgcamaramove osgcamararotate osgcamarapelota osgjuegomartillo posgcamara posgcamaramove posgcamararotate posgcamarapelota posgjuegomartillo

CCFLAGS=-O2
OPENCVINCLUDE=-I./opencv
OPENCVLIBRARY=-lcxcore -lcv -lcvaux -lhighgui -lml
GLINCLUDE=
GLLIBRARY=-lGL -lglut -lGLU -lX11 -lXext -lXmu -lXi -lc -lm
OSGLIBRARY=-losgViewer -losg -losgDB -losgGA -losgText -losgUtil -lOpenThreads

all: $(BIN)

thread.o: thread.cpp thread.h mutex.h mutex.cpp
	g++ $(CCFLAGS) -c $< -o $@

mutex.o: mutex.cpp mutex.h
	g++ $(CCFLAGS) -c $< -o $@

webcamframework.o: webcamframework.cpp webcamframework.h
	g++ $(CCFLAGS) $(OPENCVINCLUDE) $(OSGINCLUDE) -c $< -o $@

pwebcamframework.o: pwebcamframework.cpp webcamframework.h
	g++ $(CCFLAGS) $(OPENCVINCLUDE) $(OSGINCLUDE) -c $< -o $@

openglcamara: openglcamara.o
	g++ $(CCFLAGS) $(OPENCVLIBRARY) $(GLLIBRARY) openglcamara.o -o openglcamara

osg%.o: osg%.cpp webcamframework.h
	g++ $(CCFLAGS) $(OPENCVINCLUDE) $(OSGINCLUDE) -c $< -o $@

%.o: %.cpp
	g++ $(CCFLAGS) $(OPENCVINCLUDE) $(OSGINCLUDE) -c $< -o $@

osg%: osg%.o webcamframework.o thread.o mutex.o mutex.h thread.h webcamframework.h
	g++ $(CCFLAGS) $(OPENCVLIBRARY) $(OSGLIBRARY) $(GLLIBRARY) mutex.o thread.o webcamframework.o $< -o $@

posg%: osg%.o pwebcamframework.o thread.o mutex.o mutex.h thread.h webcamframework.h
	g++ $(CCFLAGS) $(OPENCVLIBRARY) $(OSGLIBRARY) $(GLLIBRARY) mutex.o thread.o pwebcamframework.o $< -o $@

clean:
	rm -rf $(BIN) *.o
