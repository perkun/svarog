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

	#include "../shaders/flat.fs.include"
    flat_fs[flat_fs_len-1] = 0;

	#include "../shaders/vert_col.vs.include"
	#include "../shaders/vert_col.fs.include"
    vert_col_vs[vert_col_vs_len-1] = 0;
    vert_col_fs[vert_col_fs_len-1] = 0;

	#include "../shaders/tex_sha.vs.include"
	#include "../shaders/tex_sha.fs.include"
    tex_sha_vs[tex_sha_vs_len-1] = 0;
    tex_sha_fs[tex_sha_fs_len-1] = 0;

	#include "../shaders/radar.vs.include"
	#include "../shaders/radar.fs.include"
    radar_vs[radar_vs_len-1] = 0;
    radar_fs[radar_fs_len-1] = 0;

    shared_ptr<Shader> basic_shader = make_shared<Shader>();
    basic_shader->create_shader((char *)(void *)basic_vs,
                                (char *)(void *)basic_fs);

    shared_ptr<Shader> color_shader = make_shared<Shader>();
    color_shader->create_shader((char *)(void *)basic_vs,
                                (char *)(void *)color_fs);

    shared_ptr<Shader> flat_shader = make_shared<Shader>();
    flat_shader->create_shader((char *)(void *)basic_vs,
                                (char *)(void *)flat_fs);

    shared_ptr<Shader> line_shader = make_shared<Shader>();
    line_shader->create_shader((char *)(void *)vert_col_vs,
                               (char *)(void *)vert_col_fs);

    shared_ptr<Shader> tex_sha = make_shared<Shader>();
    tex_sha->create_shader((char *)(void *)tex_sha_vs,
                           (char *)(void *)tex_sha_fs);

    shared_ptr<Shader> radar = make_shared<Shader>();
    radar->create_shader((char *)(void *)radar_vs,
                         (char *)(void *)radar_fs);

	Application::shaders["basic_shader"] = basic_shader;
	Application::shaders["color_shader"] = color_shader;
	Application::shaders["flat_shader"] = flat_shader;
	Application::shaders["line_shader"] = line_shader;
	Application::shaders["tex_sha"] = tex_sha;
	Application::shaders["radar"] = radar;


	Application::push_layer(new MainLayer(argc, argv));


	// RENDER LOOP
	Application::run(GlfwEventMethod::POLL);
	Application::destroy();

    return 0;
}

