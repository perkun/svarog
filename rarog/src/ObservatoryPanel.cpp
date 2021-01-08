#include "svpch.h"
#include "ObservatoryPanel.h"


ObservatoryPanel::ObservatoryPanel(MainLayer *l) : layer(l)
{
}


ObservatoryPanel::~ObservatoryPanel()
{
}


void ObservatoryPanel::on_imgui_render()
{
	ImGui::Begin("Observatory");

	observe_button();
	ImGui::Separator();
	// observations buttons
	if (ImGui::Button("Make lightcurve"))
		make_lightcurve(layer->model, layer->runtime_observer);


	ImGui::InputInt("LC num points", &lc_num_points);


	if (ImGui::Button("Make AO image"))
		make_ao_image(layer->model, layer->runtime_observer);

	ImGui::InputInt("AO size [px]", &ao_size, 1, 100);

	ImGui::Separator();

	auto cam = dynamic_pointer_cast<OrthograficCamera>(
			layer->scene.observer.get_component<CameraComponent>().camera);

	ImGui::DragFloat("camera fov", &(cam->size_x), 0.2, 0.5, 100.);

	ImGui::Separator();
	display_lightcurves();

	ImGui::Separator();
	display_ao_images();

	ImGui::End();
}


void ObservatoryPanel::observe_button()
{
	string run_btn_label;
	if (layer->mode == Mode::EDITOR)
		run_btn_label = "Observe";
	else if (layer->mode == Mode::RUNTIME)
		run_btn_label = "Stop observing";

	if (ImGui::Button(run_btn_label.c_str()))
	{
		layer->toggle_mode();
	}
}


void ObservatoryPanel::display_lightcurves()
{
    if (lightcurves.size() == 0)
        return;

    ImGui::InputInt("Lightcurve Nr", &lc_id, 1);

    if (lc_id < 0)
        lc_id = 0;
    if (lc_id >= lightcurves.size())
        lc_id = lightcurves.size() - 1;

    float min = lightcurves[lc_id].min;
    float max = lightcurves[lc_id].max;

	if (ImGui::Button("set lc positions"))
	{
		layer->scene.observer.get_component<Transform>().position =
			lightcurves[lc_id].ghost_observer.get_component<Transform>().position;
		layer->scene.observer.get_component<CameraComponent>().camera->position =
			lightcurves[lc_id].ghost_observer.get_component<Transform>().position;
	}

    ImGui::PlotLines("LC", lightcurves[lc_id].data(), lightcurves[lc_id].size(),
                     0, NULL, min, max, ImVec2(200.0f, 130.0f));

	if (ImGui::Button("Delete LC"))
	{
		lightcurves[lc_id].ghost_observer.destroy();
		lightcurves.erase(lightcurves.begin() + lc_id);
	}


    if (ImGui::Button("Save magnitudes"))
    {
        lightcurves[lc_id].save_mag(FileDialog::save_file("*").c_str());
    }
    ImGui::SameLine();
    if (ImGui::Button("Save flux"))
    {
        lightcurves[lc_id].save_flux(FileDialog::save_file("*").c_str());
    }
}

void ObservatoryPanel::display_ao_images()
{
    if (ao_images.size() == 0)
        return;

    ImGui::InputInt("AO img Nr", &ao_id, 1);

    if (ao_id < 0)
        ao_id = 0;
    if (ao_id >= ao_images.size())
        ao_id = ao_images.size() - 1;

	if (ImGui::Button("set ao positions"))
	{
		layer->scene.observer.get_component<Transform>().position =
			ao_images[ao_id].ghost_observer.get_component<Transform>().position;
		layer->scene.observer.get_component<CameraComponent>().camera->position =
			ao_images[ao_id].ghost_observer.get_component<Transform>().position;
	}
    long int tex_id = ao_images[ao_id].texture->get_texture_id();
    ImGui::Image((void *)tex_id, ImVec2(300, 300), ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::Button("Delete AO image"))
	{
		ao_images[ao_id].ghost_observer.destroy();
		ao_images.erase(ao_images.begin() + ao_id);
	}

	if (ImGui::Button("Save to png"))
		ao_images[ao_id].texture->save(FileDialog::save_file("*.png").c_str());
}


void ObservatoryPanel::make_lightcurve(Entity &target, Entity &observer)
{
    int num_points = lc_num_points;
    int width = 256;
    int height = 256;
    float *pixel_buffer = new float[width * height];
    Lightcurve lc(num_points);
    vec4 bg_color = Application::get_bg_color();

    observer.get_component<CameraComponent>().camera->update_target(
        target.get_component<Transform>().position);

    layer->set_runtime_mode();

    Application::set_bg_color(vec4(0., 0., 0., 1.));
    layer->scene.on_resize(width, height);
    layer->ms_framebuffer->resize(width, height);
    layer->framebuffer->resize(width, height);

    Transform &t = target.get_component<Transform>();

    for (int i = 0; i < num_points; i++)
    {
        layer->on_update(2 * M_PI / num_points / t.rotation_speed);

        layer->framebuffer->bind();
        glReadPixels(0, 0, width, height, GL_RED, GL_FLOAT, pixel_buffer);

        double flux = 0.0;
        for (int j = 0; j < width * height; j++)
            flux += pixel_buffer[j];
        lc.push_value(flux);
	}
	layer->on_update(2 * M_PI / num_points / t.rotation_speed); // powrot

    lc.calculate_min();
    lc.calculate_max();


	Entity ghost_observer = layer->ui_scene.create_entity("LC ghost");
	ghost_observer.add_component<Material>(Application::shaders["color_shader"])
		.uniforms_vec4[ "u_color"] = vec4(32/256., 172/256., 64/256., 0.2);
	ghost_observer.add_component<MeshComponent>(make_shared<VertexArrayObject>(
				IndexedIcoSphere(vec3(0.), vec3(0.3))));
	Transform &got = ghost_observer.get_component<Transform>();
	got.position = observer.get_component<Transform>().position;

	layer->ui_scene.root_entity.add_child(ghost_observer);

	lc.ghost_observer = ghost_observer;

    lightcurves.push_back(lc);
    lc_id = lightcurves.size() - 1;

    layer->scene.on_resize(layer->viewport_panel_size.x,
                           layer->viewport_panel_size.y);
    layer->ms_framebuffer->resize(layer->viewport_panel_size.x,
                                  layer->viewport_panel_size.y);
    layer->framebuffer->resize(layer->viewport_panel_size.x,
                               layer->viewport_panel_size.y);

    Application::set_bg_color(bg_color);
    layer->set_editor_mode();
    delete[] pixel_buffer;
}


void ObservatoryPanel::make_ao_image(Entity &target, Entity &observer)
{
	int ao_width = ao_size;
	int ao_height = ao_size;

    float *pixel_buffer_r = new float[ao_width * ao_height];
    float *pixel_buffer_g = new float[ao_width * ao_height];
    float *pixel_buffer_b = new float[ao_width * ao_height];


// 	AoImage ao_image(width, height);
    vec4 bg_color = Application::get_bg_color();

    observer.get_component<CameraComponent>().camera->update_target(
        target.get_component<Transform>().position);

    layer->set_runtime_mode();

    Application::set_bg_color(vec4(0.0, 0.0, 0.0, 1.));
    layer->scene.on_resize(ao_width, ao_height);
    layer->ms_framebuffer->resize(ao_width, ao_height);
    layer->framebuffer->resize(ao_width, ao_height);


	// make image
	layer->on_update(0.);
	layer->framebuffer->bind();
	glReadPixels(0, 0, ao_width, ao_height, GL_RED, GL_FLOAT, pixel_buffer_r);
	glReadPixels(0, 0, ao_width, ao_height, GL_GREEN, GL_FLOAT, pixel_buffer_g);
	glReadPixels(0, 0, ao_width, ao_height, GL_BLUE, GL_FLOAT, pixel_buffer_b);


	ao_images.push_back(AoImage(ao_width, ao_height));
	ao_images.back().texture->update(pixel_buffer_r, pixel_buffer_g, pixel_buffer_b);


	Entity ghost_observer = layer->ui_scene.create_entity("AO ghost");
	ghost_observer.add_component<Material>(Application::shaders["color_shader"])
		.uniforms_vec4[ "u_color"] = vec4(237/256., 84/256., 84/256., 0.2);
	ghost_observer.add_component<MeshComponent>(make_shared<VertexArrayObject>(
				IndexedIcoSphere(vec3(0.), vec3(0.3))));
	Transform &got = ghost_observer.get_component<Transform>();
	got.position = observer.get_component<Transform>().position;

	layer->ui_scene.root_entity.add_child(ghost_observer);

	ao_images.back().ghost_observer = ghost_observer;


	ao_id = ao_images.size() - 1;

    layer->scene.on_resize(layer->viewport_panel_size.x,
                           layer->viewport_panel_size.y);
    layer->ms_framebuffer->resize(layer->viewport_panel_size.x,
                                  layer->viewport_panel_size.y);
    layer->framebuffer->resize(layer->viewport_panel_size.x,
                               layer->viewport_panel_size.y);

    Application::set_bg_color(bg_color);
    layer->set_editor_mode();
	delete[] pixel_buffer_r;
	delete[] pixel_buffer_g;
	delete[] pixel_buffer_b;
}
