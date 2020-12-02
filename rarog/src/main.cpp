#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "Application.h"
#include "MainLayer.h"


using namespace std;


#define WIN_W 800
#define WIN_H 600


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   MAIN   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	Application::init(WIN_W, WIN_H, "Application", false);
	Application::get_window()->set_cursor_normal();

	Application::push_layer(new MainLayer(argc, argv));

	// RENDER LOOP
	Application::rendering_loop(GlfwEventMethod::POLL);
	Application::destroy();
    return 0;
}

