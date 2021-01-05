#include "svpch.h"
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

	// load shaders
	#include "../shaders/basic.vs.include"
	#include "../shaders/basic.fs.include"
    basic_vs[basic_vs_len-1] = 0;
    basic_fs[basic_fs_len-1] = 0;

	#include "../shaders/color.fs.include"
    color_fs[color_fs_len-1] = 0;

	#include "../shaders/vert_col.vs.include"
	#include "../shaders/vert_col.fs.include"
    vert_col_vs[vert_col_vs_len-1] = 0;
    vert_col_fs[vert_col_fs_len-1] = 0;

    shared_ptr<Shader> basic_shader = make_shared<Shader>();
    basic_shader->create_shader((char *)(void *)basic_vs,
                                (char *)(void *)basic_fs);

    shared_ptr<Shader> color_shader = make_shared<Shader>();
    color_shader->create_shader((char *)(void *)basic_vs,
                                (char *)(void *)color_fs);

    shared_ptr<Shader> line_shader = make_shared<Shader>();
    line_shader->create_shader((char *)(void *)vert_col_vs,
                               (char *)(void *)vert_col_fs);


	Application::shaders["basic_shader"] = basic_shader;
	Application::shaders["color_shader"] = color_shader;
	Application::shaders["line_shader"] = line_shader;


	Application::push_layer(new MainLayer(argc, argv));


	// RENDER LOOP
	Application::run(GlfwEventMethod::POLL);
	Application::destroy();

    return 0;
}

