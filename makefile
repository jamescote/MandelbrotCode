# Make File for Assignment 3

TARGET=Assignment3
MODULES=ioutil.o main.o Mandelbrot.o
HEADERS=ioutil.h
CPPFLAGS=-ansi -Wall -fprofile-arcs -ftest-coverage `Magick++-config --cppflags --ldflags`

$(TARGET): $(MODULES)
	g++ $(CPPFLAGS) $(MODULES) -o $(TARGET)

clean:
	rm -f *.o $(TARGET) *~ *.gcov *.gcda *.gcno

all: clean $(TARGET)

ioutil.o: ioutil.cpp ioutil.h
	g++ $(CPPFLAGS) -c ioutil.cpp

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp

Mandelbrot.o: Mandelbrot.cpp Mandelbrot.h
	g++ $(CPPFLAGS) -c Mandelbrot.cpp 
