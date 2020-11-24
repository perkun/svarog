#include "ShadowsLayer.h"
#include "Components.h"


ShadowsLayer::ShadowsLayer() : SceneLayer()
{
}


ShadowsLayer::~ShadowsLayer()
{
}


void ShadowsLayer::on_attach()
{
	scene = new Scene;
    Window *window = Application::get_window();
    // 	#include "shaders/shadow.vs.include"
    // 	#include "shaders/shadow.fs.include"

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


    shader = new Shader;
    shader->create_shader((char *)((void *)tex_sha_vs),
                          (char *)((void *)tex_sha_fs));

    color_shader = new Shader;
    color_shader->create_shader((char *)((void *)color_shader_vs),
                                (char *)(void *) color_shader_fs);

    basic_shader = new Shader;
    basic_shader->create_shader((char *)((void *)basic_shader_vs),
                                (char *)(void *)basic_shader_fs);

    plane_texture = new ImgTexture("../../../data/dots4.png");
    //     plane_texture = new ImgTexture("../../../data/forest.jpg");
    asteroid_texture = new ImgTexture("../../../data/surface2.jpg");

    plane_vao =
        new VertexArrayObject(IndexedQuad(vec3(-10., -10., 0.), vec2(20.)));
    plane_vao->blend = true;

    asteroid_vao = new VertexArrayObject(
        IndexedModelObj("../../../data/model.obj", NormalIndexing::PER_FACE));

    cube_vao = new VertexArrayObject(IndexedCube());


    scene->observer = scene->create_entity("Camera");
    scene->observer.add_component<CameraComponent>(new PerspectiveCamera(
        radians(45.0), window->width / (float)window->height, 0.01, 500.0));
	scene->observer.add_component<NativeScriptComponent>().bind<CameraController>();

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

    FramebufferSpecification fb_spec;
    fb_spec.width = 1024;
    fb_spec.height = 1024;
    scene->light.add_component<FramebufferComponent>(
        new Framebuffer(fb_spec));

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
    asteroid.add_component<TextureComponent>(asteroid_texture);
    asteroid.add_component<SceneStatus>(true);
    asteroid.add_component<Material>(shader);

    Transform &tr = asteroid.get_component<Transform>();
    tr.scale = vec3(5.);
    tr.position.z = 3.;


	// create volumetric data for marchcubes
	data = new short int[dim_x * dim_y * dim_z];
	int r = 4;
	int x0 = dim_x/2, y0 = dim_y/2, z0 = dim_z/2;


	PerlinNoise pn;

	for (int k = 0; k < dim_z; k++)
		for (int j = 0; j < dim_y; j++)
			for (int i = 0; i < dim_x; i++)
			{
				double x = (double)i/((double)dim_x);
				double y = (double)j/((double)dim_y);
				double z = (double)k/((double)dim_z);

				double noise = pn.noise(5*x, 5*y, 5*z);

				data[k*dim_y*dim_x + j*dim_x + i] = floor(noise * 255);

// 				float len = (i-x0)*(i-x0) + (j-y0)*(j-y0) + (k-z0)*(k-z0);
// 				if ( abs(len - r*r) < 1.5 )
// // 				if (i > 5 && i < 9 && j > 5 && j < 9)
// 					data[k*dim_y*dim_x + j*dim_x + i] = 200;
// 				else
// 					data[k*dim_y*dim_x + j*dim_x + i] = 0;
			}

	MarchingCubes marching_cubes;
	space_vao = new VertexArrayObject(marching_cubes.polygonise_space(
		data, dim_x, dim_y, dim_z, mc_isolevel));


	space = scene->create_entity("Space");
	space.add_component<MeshComponent>(space_vao);
    space.add_component<Material>(basic_shader).uniforms_vec4["u_color"] =
		vec4(0.5, 0.3, 0.2, 1.);
;
// 	space.get_component<Transform>().position = vec3(-10, 0, 0);



	Batch cube_batch;
//
	pts.layout.elements.push_back(VertexDataType::FLOAT3);
	int count = 0;
	for (int k = 0; k < dim_z; k++)
		for (int j = 0; j < dim_y; j++)
			for (int i = 0; i < dim_x; i++)
			{
				if (data[k*dim_y*dim_x + j*dim_x + i] != 0)
				{
					cube_batch.models.push_back(
						IndexedColorCube(vec3(i - 15, j - 15, k - 15), vec3(1.),
						vec4(1., 1., 1., data[k*dim_y*dim_x + j*dim_x + i]/1024.)));
				}
			}

	cube_batch.make_batch();
	cout << cube_batch.models.size() << endl;

	points_vao = new VertexArrayObject(cube_batch.batch);
	points_vao->blend = true;
// 	points_vao = new VertexArrayObject(pts);
// 	points_vao->draw_type = GL_POINTS;
// 	points_vao->num_draw_elements = pts.vertices.size();
//
	points = scene->create_entity("points");
	points.add_component<MeshComponent>(points_vao);
    points.add_component<Material>(color_shader);
	points.get_component<Transform>().position = vec3(0, 0, 0);


//     scene->root_entity.add_child(&asteroid);
    scene->root_entity.add_child(&space);
//     scene->root_entity.add_child(&plane);
    scene->root_entity.add_child(&points);
    scene->root_entity.add_child(&scene->light);


    // depth texture on slot 1
    scene->scene_material.uniforms_int["u_depth_map"] = 1;


}

void ShadowsLayer::on_update(double time_delta)
{
	if (previous_iso != mc_isolevel)
	{
		MarchingCubes marching_cubes;
//
		delete space_vao;
		space_vao = new VertexArrayObject(marching_cubes.polygonise_space(
					data, dim_x, dim_y, dim_z, mc_isolevel));
		space.replace_component<MeshComponent>(space_vao);
	}
	previous_iso = mc_isolevel;

    plane.get_component<Transform>().position =
        scene->observer.get_component<Transform>()
            .calculate_intersection_point(vec3(0.), vec3(0., 0., 1.));

    Transform &slt = scene->light.get_component<Transform>();
    slt.update_target(vec3(0.));

	scene->on_update(time_delta);

    td = time_delta;
}

void ShadowsLayer::on_imgui_render()
{
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
    ImGui::Separator();
    ImGui::Separator();

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


	ImGui::DragInt("isolevel", &mc_isolevel, 1., 0, 255);
	if (ImGui::Button("Update isolevel"))
	{
		MarchingCubes marching_cubes;
//
		delete space_vao;
		space_vao = new VertexArrayObject(marching_cubes.polygonise_space(
					data, dim_x, dim_y, dim_z, mc_isolevel));
		space.replace_component<MeshComponent>(space_vao);
	}

    ImGui::End();


//     ImGui::Begin("depth map");
// //     long int tex_id = shadow_fb->get_color_attachment_id();
//     long int tex_id = shadow_fb->get_depth_attachment_id();
//     ImGui::Image((void *)tex_id, ImVec2(256, 256), ImVec2(0, 1),
//                  ImVec2(1, 0));
//     ImGui::End();

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

	delete data;
}

