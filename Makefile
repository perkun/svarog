PROGRAM_NAME = main
TARGET = bin/$(PROGRAM_NAME)
CC = g++
SRCDIR = src
BUILDDIR = build
CFLAGS = -g -c -std=c++17
LIB = -lglfw -lGLEW -lGL #-lrenderEngine -lm -lGL -lGLU -lGLEW -lnetcdf_c++4 -lnetcdf -L/opt/cuda/lib64 -lcudart # -lfreeimage

OBJECTS = build/main.o build/Svarog.o build/Mesh.o build/Transform.o \
		  build/SceneNode.o build/Shader.o build/Window.o \
		  build/Event.o build/KeyEvent.o build/MouseEvent.o \
		  build/WindowEvent.o \
		  build/Camera.o \
		  build/stb_image.o \
		  build/Texture.o \
		  build/Renderer.o

 #build/SceneGraph.o


# linking
$(TARGET): $(OBJECTS)
		$(CC) $^ -o $(TARGET) $(LIB)

build/main.o: src/main.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/Svarog.o: src/Svarog.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/Mesh.o: src/Mesh.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/Transform.o: src/Transform.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/SceneNode.o: src/SceneNode.cpp
		$(CC) $(CFLAGS) -o $@ $^

#build/SceneGraph.o: src/SceneGraph.cpp
		#$(CC) $(CFLAGS) -o $@ $^

build/Shader.o: src/Shader.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/Window.o: src/Window.cpp
		$(CC) $(CFLAGS) -o $@ $^


build/Camera.o: src/Camera.cpp
		$(CC) $(CFLAGS) -o $@ $^


build/Event.o: src/Event/Event.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/KeyEvent.o: src/Event/KeyEvent.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/MouseEvent.o: src/Event/MouseEvent.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/WindowEvent.o: src/Event/WindowEvent.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/Texture.o: src/Texture.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/Renderer.o: src/Renderer.cpp
		$(CC) $(CFLAGS) -o $@ $^

build/stb_image.o: src/vendor/stb_image/stb_image.cpp
		$(CC) $(CFLAGS) -o $@ $^

clean:
		rm -f build/*.o
		rm -f $(TARGET)
