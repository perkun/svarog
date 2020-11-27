#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "Application.h"
#include "MainLayer.h"
#include <cmdlineargs/ArgumentHandler.h>


using namespace std;


#define WIN_W 800
#define WIN_H 600


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   MAIN   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    cout << "This is Sandbox app" << endl;

// 	ArgumentHandler ah;
// 	ah.addRequiredArgument("m", "model");
// 	ah.addRequiredArgument("p", "plane");
// 	ah.addRequiredArgument("t", "texture");
//
// 	ah.parseArguments(argc, argv);

//     Application app;
	Application::init(WIN_W, WIN_H, "Application", false);
	Application::get_window()->set_cursor_normal();

	Application::push_layer(new MainLayer);

	// RENDER LOOP
	Application::rendering_loop(GlfwEventMethod::WAIT);
	Application::destroy();

    return 0;
}


