PROGRAM_NAME = main
TARGET = bin/$(PROGRAM_NAME)
CC = g++
SRCDIR = src
BUILDDIR = build
CFLAGS = -g -c -std=c++17
LIB = -lglfw -lGLEW -lGL
#-lrenderEngine -lm -lGL -lGLU -lGLEW -lnetcdf_c++4 -lnetcdf -L/opt/cuda/lib64 -lcudart # -lfreeimage


# main src folder
MAIN_DIR = src
SOURCES = $(MAIN_DIR)/main.cpp $(MAIN_DIR)/Application.cpp $(MAIN_DIR)/Mesh.cpp \
		  $(MAIN_DIR)/Transform.cpp $(MAIN_DIR)/Shader.cpp $(MAIN_DIR)/Window.cpp \
		  $(MAIN_DIR)/Camera.cpp $(MAIN_DIR)/Texture.cpp $(MAIN_DIR)/Renderer.cpp \
		  $(MAIN_DIR)/LayerStack.cpp $(MAIN_DIR)/Material.cpp \
		  $(MAIN_DIR)/Framebuffer.cpp $(MAIN_DIR)/Light.cpp
CFLAGS += -I$(MAIN_DIR)


LAYER_DIR = src/Layer
SOURCES += $(LAYER_DIR)/Layer.cpp $(LAYER_DIR)/ExampleLayer.cpp \
		   $(LAYER_DIR)/ImGuiLayer.cpp
CFLAGS += -I$(LAYER_DIR)


EVENT_DIR = src/Event
SOURCES += $(EVENT_DIR)/Event.cpp $(EVENT_DIR)/KeyEvent.cpp \
		   $(EVENT_DIR)/MouseEvent.cpp $(EVENT_DIR)/WindowEvent.cpp



SCENE_DIR = src/Scene
SOURCES += $(SCENE_DIR)/Scene.cpp $(SCENE_DIR)/SceneStatus.cpp\
		   $(SCENE_DIR)/Entity.cpp
CFLAGS += -I$(SCENE_DIR)

STB_IMAGE_DIR = src/vendor/stb_image
SOURCES += $(STB_IMAGE_DIR)/stb_image.cpp

## ImGui STUFF
IMGUI_DIR = src/vendor/imgui
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
CFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends


OBJECTS += $(addsuffix .o, $(addprefix build/, $(basename $(notdir $(SOURCES)))))

OBJECTS += build/stb_image.o \



# linking
$(TARGET): $(OBJECTS)
		$(CC) $^ -o $(TARGET) $(LIB)

# RULES


build/main.o: src/main.cpp
		$(CC) $(CFLAGS) -o $@ $<

## MAIN_DIR
build/%.o: $(MAIN_DIR)/%.cpp $(MAIN_DIR)/%.h
		$(CC) $(CFLAGS) -o $@ $<

## EVENTS
build/%.o: $(EVENT_DIR)/%.cpp $(EVENT_DIR)/%.h
		$(CC) $(CFLAGS) -o $@ $<

## SCENE
build/%.o: $(SCENE_DIR)/%.cpp $(SCENE_DIR)/%.h
		$(CC) $(CFLAGS) -o $@ $<

## LAYER
build/%.o: $(LAYER_DIR)/%.cpp $(LAYER_DIR)/%.h
		$(CC) $(CFLAGS) -o $@ $<

## STB_IMAGE
build/%.o: $(STB_IMAGE_DIR)/%.cpp
		$(CC) $(CFLAGS) -o $@ $<

## IMGUI
build/%.o:%.cpp
		$(CC) $(CFLAGS) -c -o $@ $<

build/%.o:$(IMGUI_DIR)/%.cpp
		$(CC) $(CFLAGS) -c -o $@ $<

build/%.o:$(IMGUI_DIR)/backends/%.cpp
		$(CC) $(CFLAGS) -c -o $@ $<


clean:
		rm -f build/*.o
		rm -f $(TARGET)
