#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "Application.h"
// #include "MyLayer.h"
// #include "BoxLayer.h"
#include "ShadowsLayer.h"
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

// 	MyLayer *my_layer = new MyLayer();
// 	my_layer->model_filename = ah.args["model"].to_str();
// 	my_layer->plane_filename = ah.args["plane"].to_str();
// 	my_layer->texture_filename = ah.args["texture"].to_str();

// 	Application::push_layer(my_layer);
// 	Application::push_layer(new MyLayer);
// 	Application::push_layer(new BoxLayer);
	Application::push_layer(new ShadowsLayer);

	// RENDER LOOP
	Application::rendering_loop(GlfwEventMethod::POLL);
	Application::destroy();
    return 0;
}

