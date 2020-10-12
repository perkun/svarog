PROGRAM_NAME = main
TARGET = bin/$(PROGRAM_NAME)
CC = g++
SRCDIR = src
BUILDDIR = build
CFLAGS = -g -c -std=c++11
LIB = #-lrenderEngine -lglfw -lm -lGL -lGLU -lGLEW -lnetcdf_c++4 -lnetcdf -L/opt/cuda/lib64 -lcudart # -lfreeimage

OBJECTS = build/main.o


# linking
$(TARGET): $(OBJECTS)
		$(CC) $^ -o $(TARGET) $(LIB)

build/main.o: src/main.cpp
		$(CC) $(CFLAGS) -o $@ $^


clean:
		rm -f build/*.o
		rm -f $(TARGET)
