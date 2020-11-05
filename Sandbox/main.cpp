#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "Application.h"
#include "ExampleLayer.h"
// #include "MyLayer.h"


using namespace std;


#define WIN_W 800
#define WIN_H 600


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   MAIN   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main()
{
    cout << "This is Application!" << endl;

//     Application app;
	Application::init(WIN_W, WIN_H, "Application", false);
	Application::get_window()->set_cursor_normal();


	Application::push_layer(new ExampleLayer);
// 	Application::push_layer(new MyLayer);

	// RENDER LOOP
	Application::rendering_loop(GlfwEventMethod::WAIT);
	Application::destroy();
    return 0;
}

