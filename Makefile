PROGRAM_NAME = main
TARGET = bin/$(PROGRAM_NAME)
CC = g++
SRCDIR = src
BUILDDIR = build
CFLAGS = -g -c -std=c++17
LIB = -lglfw -lGLEW -lGL
#-lrenderEngine -lm -lGL -lGLU -lGLEW -lnetcdf_c++4 -lnetcdf -L/opt/cuda/lib64 -lcudart # -lfreeimage

OBJECTS = build/main.o build/Application.o build/Mesh.o build/Transform.o \
		  build/Shader.o build/Window.o \
		  build/Event.o build/KeyEvent.o build/MouseEvent.o \
		  build/WindowEvent.o \
		  build/Camera.o \
		  build/stb_image.o \
		  build/Texture.o \
		  build/Renderer.o \
		  build/Scene.o build/SceneStatus.o build/Entity.o \
		  build/Layer.o build/LayerStack.o \
		  build/ExampleLayer.o build/ImGuiLayer.o \
		  build/Framebuffer.o \
		  build/Material.o \
		  build/Light.o


## ImGui STUFF
IMGUI_DIR = src/vendor/imgui
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJECTS += $(addsuffix .o, $(addprefix build/, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)

CFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends



# linking
$(TARGET): $(OBJECTS)
		$(CC) $^ -o $(TARGET) $(LIB)



build/main.o: src/main.cpp
		$(CC) $(CFLAGS) -o $@ $<

build/Application.o: src/Application.cpp src/Application.h
		$(CC) $(CFLAGS) -o $@ $<

build/Mesh.o: src/Mesh.cpp src/Mesh.h
		$(CC) $(CFLAGS) -o $@ $<

build/Transform.o: src/Transform.cpp src/Transform.h
		$(CC) $(CFLAGS) -o $@ $<

build/Shader.o: src/Shader.cpp src/Shader.h
		$(CC) $(CFLAGS) -o $@ $<

build/Window.o: src/Window.cpp src/Window.h
		$(CC) $(CFLAGS) -o $@ $<

build/Camera.o: src/Camera.cpp src/Camera.h
		$(CC) $(CFLAGS) -o $@ $<

build/Event.o: src/Event/Event.cpp src/Event/Event.h
		$(CC) $(CFLAGS) -o $@ $<

build/KeyEvent.o: src/Event/KeyEvent.cpp src/Event/KeyEvent.h
		$(CC) $(CFLAGS) -o $@ $<

build/MouseEvent.o: src/Event/MouseEvent.cpp src/Event/MouseEvent.h
		$(CC) $(CFLAGS) -o $@ $<

build/WindowEvent.o: src/Event/WindowEvent.cpp src/Event/WindowEvent.h
		$(CC) $(CFLAGS) -o $@ $<

build/Texture.o: src/Texture.cpp src/Texture.h
		$(CC) $(CFLAGS) -o $@ $<

build/Renderer.o: src/Renderer.cpp src/Renderer.h
		$(CC) $(CFLAGS) -o $@ $<

build/Scene.o: src/Scene/Scene.cpp src/Scene/Scene.h
		$(CC) $(CFLAGS) -o $@ $<

build/SceneStatus.o: src/Scene/SceneStatus.cpp src/Scene/SceneStatus.h
		$(CC) $(CFLAGS) -o $@ $<

build/Entity.o: src/Scene/Entity.cpp src/Scene/Entity.h
		$(CC) $(CFLAGS) -o $@ $<

build/Layer.o: src/Layer.cpp src/Layer.h
		$(CC) $(CFLAGS) -o $@ $<

build/LayerStack.o: src/LayerStack.cpp src/LayerStack.h
		$(CC) $(CFLAGS) -o $@ $<

build/ExampleLayer.o: src/ExampleLayer.cpp src/ExampleLayer.h
		$(CC) $(CFLAGS) -o $@ $<

build/ImGuiLayer.o: src/ImGuiLayer.cpp src/ImGuiLayer.h
		$(CC) $(CFLAGS) -o $@ $<


build/Material.o: src/Material.cpp src/Material.h
		$(CC) $(CFLAGS) -o $@ $<

build/Light.o: src/Light.cpp src/Light.h
		$(CC) $(CFLAGS) -o $@ $<


build/Framebuffer.o: src/Framebuffer.cpp src/Framebuffer.h
		$(CC) $(CFLAGS) -o $@ $<

build/stb_image.o: src/vendor/stb_image/stb_image.cpp
		$(CC) $(CFLAGS) -o $@ $<





##---------------------------------------------------------------------
## BUILD RULES IMGUI
##---------------------------------------------------------------------

build/%.o:%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

build/%.o:$(IMGUI_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

build/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<






clean:
		rm -f build/*.o
		rm -f $(TARGET)
