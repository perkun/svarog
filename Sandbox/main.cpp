#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "Application.h"
//#include "ExampleLayer.h"
#include "MyLayer.h"
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

	ArgumentHandler ah;
	ah.addRequiredArgument("i", "input");

	ah.parseArguments(argc, argv);

//     Application app;
	Application::init(WIN_W, WIN_H, "Application", false);
	Application::get_window()->set_cursor_normal();

	MyLayer *my_layer = new MyLayer();
	my_layer->model_filename = ah.args["input"].to_str();

	Application::push_layer(my_layer);
// 	Application::push_layer(new MyLayer);

	// RENDER LOOP
	Application::rendering_loop(GlfwEventMethod::WAIT);
	Application::destroy();
    return 0;
}

