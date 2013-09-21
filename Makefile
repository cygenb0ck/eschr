OBJS = eschr.o\
	eschrObject.o\

CXXFLAGS =  `sdl-config --cflags` -O0 -g3 -ggdb -Wall -fmessage-length=0
LIBS = `sdl-config --libs` -lGL -lGLU -lSDL_image -lGLEW

TARGET =	eschr

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
