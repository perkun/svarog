#include <cppargs/Parser.h>
#include "svpch.h"
#include "Application.h"
#include "MainLayer.h"


using namespace std;


#define WIN_W 256
#define WIN_H 256


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   MAIN   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{

    Parser parser;


    parser.add_positional("model", "path to OBJ/SHP model", 1);

    parser.add_option('t', "texture", "texture file", false, "");

    parser.add_vec_option("model-pos", "xyz of the model", 3, true);
    parser.add_vec_option("observer-pos", "xyz of the observer", 3, true);

    parser.add_flag("header", "load model parameters from obj header");

    parser.add_vec_option("spin", "specify lambda, beta, gamma", 3, false);
    parser.add_option("jd", "julian day of observation", false, "");
    parser.add_option("jd0", "julian day of gamma_0", false, "");
    parser.add_option("period", "rotation periond in hours", false, "");
    parser.add_option('n', "num-points", "specify number of lightcurve points",
                      false, "90");
	parser.add_option('o', "out", "output filename", false, "lc.dat");
	parser.add_option("LSL", "Lambert to Lommel-Seeliger ratio", false, "0.1");

	parser.add_flag("debug", "debug mode");

    Args args = parser.parse_args(argc, argv);


    if (args["jd"] && !args["header"])
        if (!(args["jd0"] && args["period"] && args["spin"]))
        {
            cout << "You need to specify jd0, spin and period" << endl;
            exit(1);
        }


    Application::init(WIN_W, WIN_H, "meshview", false, args["debug"]);
    Application::get_window()->set_cursor_normal();

//     auto &io = ImGui::GetIO();
//     // 	io.IniFilename = NULL;
//      	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//     // 	io.ConfigFlags |= ImGuiDockNodeFlags_AutoHideTabBar;
//     //io.ConfigDockingAlwaysTabBar = false;

// load shaders
#include "../shaders/basic.vs.include"
#include "../shaders/basic.fs.include"
    basic_vs[basic_vs_len - 1] = 0;
    basic_fs[basic_fs_len - 1] = 0;

#include "../shaders/color.fs.include"
    color_fs[color_fs_len - 1] = 0;

#include "../shaders/flat.fs.include"
    flat_fs[flat_fs_len - 1] = 0;

#include "../shaders/vert_col.vs.include"
#include "../shaders/vert_col.fs.include"
    vert_col_vs[vert_col_vs_len - 1] = 0;
    vert_col_fs[vert_col_fs_len - 1] = 0;

#include "../shaders/tex_sha.vs.include"
#include "../shaders/tex_sha.fs.include"
    tex_sha_vs[tex_sha_vs_len - 1] = 0;
    tex_sha_fs[tex_sha_fs_len - 1] = 0;

#include "../shaders/tex_sha_ls.fs.include"
    tex_sha_ls_fs[tex_sha_ls_fs_len - 1] = 0;

#include "../shaders/radar.vs.include"
#include "../shaders/radar.fs.include"
    radar_vs[radar_vs_len - 1] = 0;
    radar_fs[radar_fs_len - 1] = 0;

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

    shared_ptr<Shader> tex_sha_ls = make_shared<Shader>();
    tex_sha_ls->create_shader((char *)(void *)tex_sha_vs,
                              (char *)(void *)tex_sha_ls_fs);

    shared_ptr<Shader> radar = make_shared<Shader>();
    radar->create_shader((char *)(void *)radar_vs, (char *)(void *)radar_fs);

    Application::shaders["basic_shader"] = basic_shader;
    Application::shaders["color_shader"] = color_shader;
    Application::shaders["flat_shader"] = flat_shader;
    Application::shaders["line_shader"] = line_shader;
    Application::shaders["tex_sha"] = tex_sha;
    Application::shaders["tex_sha_ls"] = tex_sha_ls;
    Application::shaders["radar"] = radar;


    MainLayer *main_layer = new MainLayer(args);
    Application::push_layer(main_layer);


    // RENDER LOOP
    Application::run(GlfwEventMethod::POLL);
    Application::pop_layer(main_layer);
    Application::destroy();

    return 0;
}
