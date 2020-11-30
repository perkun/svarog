#include "ShadowsLayer.h"

ShadowsLayer::ShadowsLayer() : SceneLayer()
{
}

ShadowsLayer::~ShadowsLayer()
{
}

void ShadowsLayer::on_attach()
{
    Window *window = Application::get_window();

#include "shaders/tex_sha.vs.include"
#include "shaders/tex_sha.fs.include"
    tex_sha_vs[tex_sha_vs_len] = 0;
    tex_sha_fs[tex_sha_fs_len] = 0;

#include "shaders/basic_shader.vs.include"
#include "shaders/basic_shader.fs.include"
    basic_shader_vs[basic_shader_vs_len] = 0;
    basic_shader_fs[basic_shader_fs_len] = 0;

#include "shaders/color_shader.vs.include"
#include "shaders/color_shader.fs.include"
    color_shader_vs[color_shader_vs_len] = 0;
    color_shader_fs[color_shader_fs_len] = 0;

#include "shaders/displacement.vs.include"
#include "shaders/normals.gs.include"
#include "shaders/displacement.fs.include"
    displacement_vs[displacement_vs_len] = 0;
    normals_gs[normals_gs_len] = 0;
    displacement_fs[displacement_fs_len] = 0;

	displacement_shader = new Shader;
	displacement_shader->create_shader((char *)((void *)displacement_vs),
									   (char *)((void *)normals_gs),
                          			   (char *)((void *)displacement_fs));


    shader = new Shader;
    shader->create_shader((char *)((void *)tex_sha_vs),
                          (char *)((void *)tex_sha_fs));

    color_shader = new Shader;
    color_shader->create_shader((char *)((void *)color_shader_vs),
                                (char *)(void *)color_shader_fs);

    basic_shader = new Shader;
    basic_shader->create_shader((char *)((void *)basic_shader_vs),
                                (char *)(void *)basic_shader_fs);

    plane_texture = new ImgTexture("../../../data/dots4.png");
    //     plane_texture = new ImgTexture("../../../data/forest.jpg");
    asteroid_texture = new ImgTexture("../../../data/surface2.jpg");



	// perlin noise texture
	TextureSpec pts;
	pts.target = GL_TEXTURE_3D;
	pts.internal_format = GL_RED;
	pts.width = 100;
	pts.height = 100;
	pts.depth = 100;
	pts.format = GL_RED;
	pts.type = GL_FLOAT;

	perlin_tex = new Texture(pts);


	pts.target = GL_TEXTURE_2D;
	pts.internal_format = GL_RED;
	pts.width = 512;
	pts.height = 512;
	pts.format = GL_RED;
	pts.type = GL_FLOAT;

	perlin_tex_flat = new Texture(pts);

	perlin_data = new SurfaceData<float>(512, 512);
	perlin_data->fill_with_perlin_noise(size_factor, 1.0);

	perlin_tex_flat->update(perlin_data->data);


    plane_vao =
        new VertexArrayObject(IndexedQuad(vec3(-10., -10., 0.), vec2(20.)));
    plane_vao->blend = true;

    asteroid_vao = new VertexArrayObject(
        IndexedModelObj("../../../data/model.obj", NormalIndexing::PER_VERTEX));

    cube_vao = new VertexArrayObject(IndexedCube());

    scene->observer = scene->create_entity("Camera");
    scene->observer.add_component<CameraComponent>(new PerspectiveCamera(
        radians(45.0), window->width / (float)window->height, 0.01, 500.0));
    scene->observer.add_component<NativeScriptComponent>()
        .bind<CameraController>();

    Transform &sot = scene->observer.get_component<Transform>();
    sot.position = vec3(0., -50., 50.);
    sot.update_target(vec3(0., 0., 0.));
    sot.speed = 8.;

    scene->light = scene->create_entity("Light");
    scene->light.add_component<MeshComponent>(cube_vao);
    scene->light.add_component<SceneStatus>(true);
    scene->light.add_component<Material>(basic_shader)
        .uniforms_vec4["u_color"] = vec4(0.3, 0.7, 0.12, 1.);

    scene->light.add_component<CameraComponent>(
        new OrthogonalCamera(100., 1., 0.01, 100.));

    FramebufferSpec fb_spec;
    fb_spec.width = 1024;
    fb_spec.height = 1024;
	fb_spec.flags |= DEPTH_ATTACHMENT;
    scene->light.add_component<FramebufferComponent>(new Framebuffer(fb_spec));

    Transform &slt = scene->light.get_component<Transform>();
    slt.position = vec3(-30., -30., 30.);
    slt.update_target(vec3(0., 0., 0.));

    plane = scene->create_entity("Plain");
    plane.add_component<MeshComponent>(plane_vao);
    plane.add_component<TextureComponent>(plane_texture);
    plane.add_component<SceneStatus>(false);
    plane.add_component<Material>(shader);


    asteroid = scene->create_entity("Aster");
    asteroid.add_component<MeshComponent>(asteroid_vao);
    asteroid.add_component<TextureComponent>(perlin_tex);
    asteroid.add_component<SceneStatus>(true);
// 	asteroid.add_component<Material>(shader);
// 	asteroid.add_component<TextureComponent>(asteroid_texture);
    asteroid.add_component<Material>(displacement_shader).uniforms_float["u_displacement_factor"] = 3.0;
	asteroid.get_component<Material>().uniforms_float["u_texture_z"] = 0.5;

    Transform &tr = asteroid.get_component<Transform>();
    tr.scale = vec3(5.);
    tr.position.z = 3.;

    // create volumetric data for marchcubes

	vol_data = new VolumetricData<float>(100, 100, 100);
	vol_data->fill_with_perlin_noise(size_factor, 1.0, true, seed);

	perlin_tex->update(vol_data->data);

    space_vao =
        new DynamicVertexArrayObject(MarchingCubes::polygonise_space<float>(
            vol_data, mc_isolevel));

    space = scene->create_entity("Space");
    space.add_component<MeshComponent>(space_vao);
    space.add_component<Material>(basic_shader).uniforms_vec4["u_color"] =
        vec4(0.5, 0.3, 0.2, 1.);

    // 	space.get_component<Transform>().position = vec3(-10, 0, 0);

    scene->root_entity.add_child(&asteroid);
    scene->root_entity.add_child(&scene->light);
//     scene->root_entity.add_child(&space);
//     scene->root_entity.add_child(&plane);

    // depth texture on slot 1
    scene->scene_material.uniforms_int["u_depth_map"] = 1;


    // SCENE FRAMEBUFFER
    fb_spec.width = Application::get_window()->width;
    fb_spec.height = Application::get_window()->height;
	fb_spec.flags = 0;
	fb_spec.flags = fb_spec.flags | COLOR_ATTACHMENT | DEPTH_ATTACHMENT;

//     scene->framebuffer = new Framebuffer(fb_spec);
// 	scene->render_to_framebuffer = true;
}

void ShadowsLayer::on_update(double time_delta)
{
    if (previous_iso != mc_isolevel)
    {
//         space_vao->update_buffer(MarchingCubes::polygonise_space<int>(
//             vol_data, mc_isolevel));
    }
    previous_iso = mc_isolevel;

	if (previous_size_factor != size_factor)
	{
		vol_data->fill_with_perlin_noise(size_factor, 1.0, true, seed);
		perlin_data->fill_with_perlin_noise(size_factor, 1.0);
//         space_vao->update_buffer(MarchingCubes::polygonise_space<int>(
//             vol_data, mc_isolevel));

		perlin_tex->update(vol_data->data);
		perlin_tex_flat->update(perlin_data->data);
// 		perlin_data->perlin_noise(size_factor);
// 		perlin_tex->update(perlin_data->data);
	}
	previous_size_factor = size_factor;

    plane.get_component<Transform>().position =
        scene->observer.get_component<Transform>().calculate_intersection_point(
            vec3(0.), vec3(0., 0., 1.));

    Transform &slt = scene->light.get_component<Transform>();
    slt.update_target(vec3(0.));

    scene->on_update(time_delta);

    td = time_delta;
}

void ShadowsLayer::on_imgui_render()
{
	ImGui::DockSpaceOverViewport();

    Transform &ptr = plane.get_component<Transform>();
    Transform &atr = asteroid.get_component<Transform>();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save scene"))
            {
                SceneSerializer scene_serializer(scene);
                string filename = FileDialog::save_file("*.scene");
                scene_serializer.serialize(filename);
                cout << "saving scene" << endl;
            }

            if (ImGui::MenuItem("Load scene"))
            {
                SceneSerializer scene_serializer(scene);
                string filename = FileDialog::open_file("*.scene");
                scene_serializer.deserialize(filename);
                cout << "loading scene" << endl;
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("FPS");
    ImGui::Text("FPS: %.3lf", 1. / td);
    ImGui::Separator(); ImGui::Separator();

    ImGui::SliderAngle("alpha", &ptr.alpha, 0., 360.);
    ImGui::SliderAngle("beta", &ptr.beta, -90., 90.);
    ImGui::SliderAngle("gamma", &ptr.gamma, 0., 360.);
    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    ImGui::Text("Asteroid");
    ImGui::SliderAngle("a alpha", &atr.alpha, 0., 360.);
    ImGui::SliderAngle("a beta", &atr.beta, -90., 90.);
    ImGui::SliderAngle("a gamma", &atr.gamma, 0., 360.);
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    ImGui::DragFloat("a x", &atr.position.x, 0.2);
    ImGui::DragFloat("a y", &atr.position.y, 0.2);
    ImGui::DragFloat("a z", &atr.position.z, 0.2);
    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    ImGui::Text("Light");
    Transform &slt = scene->light.get_component<Transform>();
    ImGui::DragFloat("l pos x", &slt.position.x, 0.2);
    ImGui::DragFloat("l pos y", &slt.position.y, 0.2);
    ImGui::DragFloat("l pos z", &slt.position.z, 0.2);
    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    ImGui::Text("Camera");
    Transform &sct = scene->observer.get_component<Transform>();
    ImGui::DragFloat("c pos x", &sct.position.x, 0.2);
    ImGui::DragFloat("c pos y", &sct.position.y, 0.2);
    ImGui::DragFloat("c pos z", &sct.position.z, 0.2);

    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    ImGui::DragFloat("size factor", &size_factor, 0.1, 1, 30);
    ImGui::DragInt("isolevel", &mc_isolevel, 1., 0, 255);

    ImGui::DragFloat("displacement factor", &asteroid.get_component<Material>().uniforms_float["u_displacement_factor"], 0.01, 0.01, 10.);


	ImGui::DragFloat("tex z", &asteroid.get_component<Material>().uniforms_float["u_texture_z"], 0.01, 0., 1.);

	ImGui::InputInt("Seed", &seed);
    if (ImGui::Button("Update seed"))
    {
		vol_data->fill_with_perlin_noise(size_factor, 1.0, true, seed);

        space_vao->update_buffer(MarchingCubes::polygonise_space<float>(
            vol_data, mc_isolevel));
		perlin_tex->update(vol_data->data);
    }
    ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0., 0.));
    ImGui::Begin("Perlin noise");
	ImVec2 vps = ImGui::GetContentRegionAvail();
	long int tex_id = perlin_tex_flat->get_texture_id();
    ImGui::Image((void *)tex_id, ImVec2(vps.x, vps.x), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();


	if (scene->render_to_framebuffer)
	{
		ImGui::Begin("Scene");
		vps = ImGui::GetContentRegionAvail();
		if (vps.x != viewport_panel_size.x || vps.y != viewport_panel_size.y)
		{
			viewport_panel_size.x = vps.x;
			viewport_panel_size.y = vps.y;
			scene->on_resize(viewport_panel_size.x, viewport_panel_size.y);
			//
			ASSERT(scene->framebuffer != NULL)
				scene->framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
		}
		// 	tex_id = scene->light.get_component<FramebufferComponent>().framebuffer->get_color_attachment_id();
		tex_id = scene->framebuffer->get_color_attachment_id();
		ImGui::Image((void *)tex_id, ImVec2(viewport_panel_size.x, viewport_panel_size.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
	ImGui::PopStyleVar();
}

void ShadowsLayer::on_detach()
{

    // 	delete points_vao;
    delete space_vao;
    delete plane_vao;
    delete asteroid_vao;
    delete cube_vao;

    delete plane_texture;
    delete asteroid_texture;

    delete shader;
    delete basic_shader;
    delete color_shader;
	delete displacement_shader;

    delete vol_data;
	delete perlin_tex;
	delete perlin_data;
}
