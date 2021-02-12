#include "Camera.h"
#include "svpch.h"
// #include <stdio.h>
#include "ObservatoryPanel.h"

ObservatoryPanel::ObservatoryPanel(MainLayer *l) : layer(l)
{
    // pteselect target and observer from leyer and layer.scene
    vector<Entity> ents = get_scene_entities();
    for (int i = 0; i < ents.size(); i++)
    {
        if (ents[i] == layer->scene.observer)
            selected_observer_idx = i;
        if (ents[i] == layer->observer_target)
            selected_target_idx = i;
    }
}

ObservatoryPanel::~ObservatoryPanel()
{
}


void ObservatoryPanel::append_children(vector<Entity> &ents, Entity entity)
{
    for (Entity e : entity.get_children())
        ents.push_back(e);

    for (Entity e : entity.get_children())
        append_children(ents, e);
}


vector<Entity> ObservatoryPanel::get_scene_entities()
{
    vector<Entity> ents;
    append_children(ents, layer->scene.root_entity);
    return ents;
}


vector<Entity> ObservatoryPanel::get_scene_root_children()
{
    vector<Entity> ents;
    for (Entity e : layer->scene.root_entity.get_children())
        ents.push_back(e);
    return ents;
}


void ObservatoryPanel::set_target_and_observer(Entity &ghost_target,
                                               Entity &ghost_observer,
                                               Entity &target, Entity &observer)
{
    vec3 gtp = ghost_target.get_component<Transform>().position;
    vec3 gop = ghost_observer.get_component<Transform>().position;

    layer->observer_target = target;
    layer->scene.observer = observer;
    layer->scene.observer.get_component<Transform>().position = gop;
    layer->observer_target.get_component<Transform>().position = gtp;

    shared_ptr<Camera> camera =
        layer->scene.observer.get_component<CameraComponent>().camera;
    camera->position = gop;
    camera->update_target(gtp);
}


// void ObservatoryPanel::change_current_obs(ObservationSeries *obs_s)
// {
// 	obs_s->validate_current_id();
// 	obs_s->detach_all_ghosts();
//     layer->ui_scene.root_entity.add_child(obs_s->get_current_obs()->ghost_target);
//     layer->ui_scene.root_entity.add_child(obs_s->get_current_obs()->ghost_observer);
// }


void ObservatoryPanel::target_selection_panel(vector<Entity> &ents)
{
    if (ents.size() == 0)
        return;

    if (ImGui::BeginCombo(
            "Target",
            ents[selected_target_idx].get_component<TagComponent>().tag.c_str(),
            0))
    {
        for (int n = 0; n < ents.size(); n++)
        {
            if (ents[n] == layer->scene.observer)
                continue;

            const bool is_selected = (selected_target_idx == n);
            if (ImGui::Selectable(
                    ents[n].get_component<TagComponent>().tag.c_str(),
                    is_selected))
            {
                selected_target_idx = n;
                layer->observer_target = ents[n];
                layer->scene.observer.get_component<CameraComponent>()
                    .camera->update_target(
                        ents[n].get_component<Transform>().position);
            }
            // Set the initial focus when opening the combo (scrolling +
            // keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}


void ObservatoryPanel::observer_selection_panel(vector<Entity> &ents)
{
    if (ents.size() == 0)
        return;

    if (ImGui::BeginCombo("Observer",
                          ents[selected_observer_idx]
                              .get_component<TagComponent>()
                              .tag.c_str(),
                          0))
    {
        for (int n = 0; n < ents.size(); n++)
        {
            if (!ents[n].has_component<CameraComponent>())
                continue;

            if (ents[n] == layer->observer_target)
                continue;

            const bool is_selected = (selected_observer_idx == n);
            if (ImGui::Selectable(
                    ents[n].get_component<TagComponent>().tag.c_str(),
                    is_selected))
            {
                selected_observer_idx = n;
                layer->scene.observer = ents[n];
            }
            // Set the initial focus when opening the combo (scrolling +
            // keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();

        }
        ImGui::EndCombo();
    }

	// observer camera fov
	auto cam = dynamic_pointer_cast<OrthograficCamera>(
			layer->scene.observer.get_component<CameraComponent>().camera);
	ImGui::PushItemWidth(150.);
	ImGui::DragFloat("camera fov", &(cam->size_x), 0.02, 0.1, 100.);
}


void ObservatoryPanel::observations_panel()
{
    ImGuiTabBarFlags tab_bar_flags =
        ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Lightcurves"))
        {
            // observations buttons
            if (ImGui::Button("Make lightcurve", ImVec2(150, 0)))
                make_lightcurve(layer->observer_target, layer->scene.observer);
            ImGui::SameLine(0., 20.);
            ImGui::PushItemWidth(100.);
            ImGui::InputInt("LC num points", &lc_num_points);
            for (int i = 0; i < 10; i++)
                ImGui::Spacing();

            display_lightcurves();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("AO images"))
        {
            if (ImGui::Button("Make AO image", ImVec2(150, 0)))
                make_ao_image(layer->observer_target, layer->scene.observer);
            ImGui::SameLine(0.0, 20.0);
            ImGui::PushItemWidth(100.);
            ImGui::InputInt("AO size [px]", &ao_size, 1, 100);

            ImGui::Text("bg color");
            ImGui::SameLine();
            ImGui::ColorEdit4("ao_bg_color", (float *)&ao_bg_color,
                              ImGuiColorEditFlags_NoInputs |
                                  ImGuiColorEditFlags_NoLabel);
			ImGui::Checkbox("Earth tilt", &earth_tilt);

            for (int i = 0; i < 10; i++)
                ImGui::Spacing();

            display_ao_images();

            ImGui::EndTabItem();
        }

		if (ImGui::BeginTabItem("Radar images"))
		{
            if (ImGui::Button("Make Radar image", ImVec2(150, 0)))
                make_radar_image(layer->observer_target, layer->scene.observer);

			ImGui::InputFloat("ang. speed", &angular_speed);

            for (int i = 0; i < 10; i++)
                ImGui::Spacing();

            display_radar_images();

            ImGui::EndTabItem();

		}
        ImGui::EndTabBar();
    }
}


void ObservatoryPanel::on_imgui_render()
{
    ImGui::Begin("Observatory");

    observe_button();
    ImGui::Separator();
    for (int i = 0; i < 10; i++)
        ImGui::Spacing();

    vector<Entity> ents = get_scene_root_children();

    target_selection_panel(ents);

    for (int i = 0; i < 5; i++)
        ImGui::Spacing();

    observer_selection_panel(ents);

    for (int i = 0; i < 10; i++)
        ImGui::Spacing();

    observations_panel();

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

	Lightcurve *current_obs = lightcurves.get_current_obs();

    ImGui::PushItemWidth(100.);
    if (ImGui::InputInt("Lc Nr", &lightcurves.current_id, 1))
    {
		current_obs = lightcurves.get_current_obs();
        lightcurves.detach_all_ghosts();
		if (current_obs)
		{
			layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
			layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
		}
    }

    ImGui::SameLine(0., 20.);
    if (ImGui::Button("set lc positions"))
        set_target_and_observer(current_obs->ghost_target,
                                current_obs->ghost_observer, current_obs->target,
                                current_obs->observer);

    ImGui::PlotLines("LC", current_obs->inv_mag_data(),
                     current_obs->size(), 0, NULL, lightcurves.lcs_min,
                     lightcurves.lcs_max, ImVec2(200.0f, 130.0f));

    if (ImGui::Button("Delete LC"))
	{
    	lightcurves.delete_current_obs();
		current_obs = lightcurves.get_current_obs();
		if (current_obs)
		{
			layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
			layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
		}
	}

    if (ImGui::Button("Save magnitudes"))
        lightcurves.save_current_mag(FileDialog::save_file("*").c_str());

    ImGui::SameLine();
    if (ImGui::Button("Save flux"))
        lightcurves.save_current_flux(FileDialog::save_file("*").c_str());
}


void ObservatoryPanel::display_ao_images()
{
    if (ao_images.size() == 0)
        return;

	AoImage *current_obs = ao_images.get_current_obs();

    ImGui::PushItemWidth(100.);
    if (ImGui::InputInt("AO Nr", &ao_images.current_id, 1))
    {
		current_obs = ao_images.get_current_obs();
        ao_images.detach_all_ghosts();
		if (current_obs)
		{
			layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
			layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
		}
    }

    ImGui::SameLine(0., 20.);
    if (ImGui::Button("set ao positions"))
        set_target_and_observer(
            current_obs->ghost_target, current_obs->ghost_observer,
            current_obs->target, current_obs->observer);

    long int tex_id = current_obs->texture->get_texture_id();
    ImGui::Image((void *)tex_id, ImVec2(300, 300), ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::Button("Delete AO image"))
	{
		ao_images.delete_current_obs();
		current_obs = ao_images.get_current_obs();
		if (current_obs)
		{
			layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
			layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
		}
	}

    if (ImGui::Button("Save to png"))
        ao_images.save(FileDialog::save_file("*.png").c_str());
}


void ObservatoryPanel::display_radar_images()
{
	if (radar_images.size() == 0)
		return;

	RadarImage *current_obs = radar_images.get_current_obs();

    ImGui::PushItemWidth(100.);
    if (ImGui::InputInt("Radar Nr", &radar_images.current_id, 1))
	{
		current_obs = radar_images.get_current_obs();
        radar_images.detach_all_ghosts();
		if (current_obs)
		{
			layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
			layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
		}
	}

    ImGui::SameLine(0., 20.);
    if (ImGui::Button("set radar positions"))
        set_target_and_observer(
            current_obs->ghost_target, current_obs->ghost_observer,
            current_obs->target, current_obs->observer);
    long int tex_id = current_obs->texture->get_texture_id();
    ImGui::Image((void *)tex_id, ImVec2(300, 300), ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::Button("Delete Radar image"))
	{
		radar_images.delete_current_obs();
		current_obs = radar_images.get_current_obs();
		if (current_obs)
		{
			layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
			layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
		}
	}

    if (ImGui::Button("Save to png"))
        radar_images.save(FileDialog::save_file("*.png").c_str());
}


void ObservatoryPanel::add_ghosts(Observation *obs, Entity &target,
                                  Entity &observer, string obs_type,
                                  vec4 ghost_color)
{
    obs->ghost_observer =
        layer->ui_scene.create_entity(obs_type + " ghost observer");
    obs->ghost_observer
        .add_component<Material>(Application::shaders["flat_shader"])
        .uniforms_vec4["u_color"] = ghost_color;
    obs->ghost_observer.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedIcoSphere(vec3(0.), vec3(0.03))));
    Transform &got = obs->ghost_observer.get_component<Transform>();
    got.position = observer.get_component<Transform>().position;

    obs->ghost_target =
        layer->ui_scene.create_entity(obs_type + " ghost target");
    obs->ghost_target
        .add_component<Material>(Application::shaders["flat_shader"])
        .uniforms_vec4["u_color"] = ghost_color;
    obs->ghost_target.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedCube(vec3(-.025), vec3(0.05))));
    Transform &gtt = obs->ghost_target.get_component<Transform>();
    gtt.position = target.get_component<Transform>().position;
}


void ObservatoryPanel::make_lightcurve(Entity &target, Entity &observer)
{
    Transform &t = target.get_component<Transform>();
	OrbitalComponent &oc = target.get_component<OrbitalComponent>();

    int num_points = lc_num_points;
    int width = 256;
    int height = 256;
    float *pixel_buffer = new float[width * height];
    Lightcurve *lc = new Lightcurve(num_points);
    vec4 bg_color = Application::get_bg_color();

    observer.get_component<CameraComponent>().camera->update_target(
        target.get_component<Transform>().position);

    Mode prev_mode = layer->mode;
    layer->set_runtime_mode();

    Application::set_bg_color(vec4(0., 0., 0., 1.));
    layer->scene.on_resize(width, height);
    layer->ms_framebuffer->resize(width, height);
    layer->framebuffer->resize(width, height);

    for (int i = 0; i < num_points; i++)
    {
		if (oc.rotation_speed != 0)
			layer->on_update(2 * M_PI / num_points / oc.rotation_speed);
		else
			layer->on_update(2 * M_PI / num_points );

        layer->framebuffer->bind();
        glReadPixels(0, 0, width, height, GL_RED, GL_FLOAT, pixel_buffer);

        double flux = 0.0;
        for (int j = 0; j < width * height; j++)
            flux += pixel_buffer[j];
        lc->push_flux(flux);
    }

    add_ghosts(lc, target, observer, "LC",
               vec4(32 / 256., 172 / 256., 64 / 256., 0.2));

    lc->target = target;
    lc->observer = observer;

    lightcurves.push(lc);
	lightcurves.detach_all_ghosts();

	layer->ui_scene.root_entity.add_child(lc->ghost_target);
	layer->ui_scene.root_entity.add_child(lc->ghost_observer);


    layer->scene.on_resize(layer->viewport_panel_size.x,
                           layer->viewport_panel_size.y);
    layer->ms_framebuffer->resize(layer->viewport_panel_size.x,
                                  layer->viewport_panel_size.y);
    layer->framebuffer->resize(layer->viewport_panel_size.x,
                               layer->viewport_panel_size.y);

    Application::set_bg_color(bg_color);
    if (prev_mode == Mode::EDITOR)
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

    Mode prev_mode = layer->mode;
    layer->set_runtime_mode();

    Application::set_bg_color(ao_bg_color);
    layer->scene.on_resize(ao_width, ao_height);
    layer->ms_framebuffer->resize(ao_width, ao_height);
    layer->framebuffer->resize(ao_width, ao_height);

    if (earth_tilt)
        observer.get_component<CameraComponent>().camera->up =
            vec3(0., 0.39774, 0.917498);

    // make image
    layer->on_update(0.);
    layer->framebuffer->bind();
    glReadPixels(0, 0, ao_width, ao_height, GL_RED, GL_FLOAT, pixel_buffer_r);
    glReadPixels(0, 0, ao_width, ao_height, GL_GREEN, GL_FLOAT, pixel_buffer_g);
    glReadPixels(0, 0, ao_width, ao_height, GL_BLUE, GL_FLOAT, pixel_buffer_b);

    AoImage *ao = new AoImage(ao_width, ao_height);
    ao->texture->update(pixel_buffer_r, pixel_buffer_g, pixel_buffer_b);

    add_ghosts(ao, target, observer, "AO",
               vec4(237 / 256., 84 / 256., 84 / 256., 0.2));

    ao->target = target;
    ao->observer = observer;

    ao_images.push(ao);
	ao_images.detach_all_ghosts();

	layer->ui_scene.root_entity.add_child(ao->ghost_target);
	layer->ui_scene.root_entity.add_child(ao->ghost_observer);


    layer->scene.on_resize(layer->viewport_panel_size.x,
                           layer->viewport_panel_size.y);
    layer->ms_framebuffer->resize(layer->viewport_panel_size.x,
                                  layer->viewport_panel_size.y);
    layer->framebuffer->resize(layer->viewport_panel_size.x,
                               layer->viewport_panel_size.y);

    Application::set_bg_color(bg_color);

    if (prev_mode == Mode::EDITOR)
        layer->set_editor_mode();

    delete[] pixel_buffer_r;
    delete[] pixel_buffer_g;
    delete[] pixel_buffer_b;
}


void ObservatoryPanel::make_radar_image(Entity &target, Entity &observer)
{
    int frame_width = 600;
    int frame_height = 600;

    float *pixel_buffer_r = new float[frame_width * frame_height];
    float *pixel_buffer_normal = new float[frame_width * frame_height];
    float *pixel_buffer_depth = new float[frame_width * frame_height];

    vec4 bg_color = Application::get_bg_color();

    Material &mat = target.get_component<Material>();
    shared_ptr<Shader> tmp_shader = mat.shader;
    mat.shader = Application::shaders["radar"];
    mat.uniforms_float["u_factor"] = angular_speed;

	CameraComponent &camera_comp = observer.get_component<CameraComponent>();
    shared_ptr<Camera> tmp_cam = camera_comp.camera;
	camera_comp.camera = make_shared<OrthograficCamera>(1., 1., 0.1, 1.);


    Mode prev_mode = layer->mode;
    layer->set_runtime_mode();

    layer->scene.on_resize(frame_width, frame_height);

	layer->multisampling = false;
    layer->framebuffer->resize(frame_width, frame_height);
    Application::set_bg_color(vec4(0, 0, 0, 0));

	layer->framebuffer->clear();

    layer->on_update(0.);
    layer->framebuffer->bind();
    glReadPixels(0, 0, frame_width, frame_height, GL_RED, GL_FLOAT,
                 pixel_buffer_r);
    glReadPixels(0, 0, frame_width, frame_height, GL_GREEN, GL_FLOAT,
                 pixel_buffer_normal);
    glReadPixels(0, 0, frame_width, frame_height, GL_BLUE, GL_FLOAT,
                 pixel_buffer_depth);
//     glReadPixels(0, 0, radar_width, radar_height, GL_DEPTH_COMPONENT, GL_FLOAT,
//                  pixel_buffer_depth);

    RadarImage *rimg =
        construct_delay_doppler(pixel_buffer_r, pixel_buffer_depth,
                                pixel_buffer_normal, frame_width, frame_height);

	add_ghosts(rimg, target, observer, "Radar", vec4(255/256., 209/255.,0., 0.2));

	rimg->target = target;
	rimg->observer = observer;

	radar_images.push(rimg);
	radar_images.detach_all_ghosts();

	layer->ui_scene.root_entity.add_child(rimg->ghost_target);
	layer->ui_scene.root_entity.add_child(rimg->ghost_observer);

    target.get_component<Material>().shader = tmp_shader;
    Application::set_bg_color(bg_color);
    layer->scene.on_resize(layer->viewport_panel_size.x,
                           layer->viewport_panel_size.y);
    layer->ms_framebuffer->resize(layer->viewport_panel_size.x,
                                  layer->viewport_panel_size.y);
    layer->framebuffer->resize(layer->viewport_panel_size.x,
                               layer->viewport_panel_size.y);

    if (prev_mode == Mode::EDITOR)
        layer->set_editor_mode();

	layer->multisampling = true;

	camera_comp.camera = tmp_cam;

    delete[] pixel_buffer_r;
    delete[] pixel_buffer_depth;
	delete[] pixel_buffer_normal;
}


RadarImage* ObservatoryPanel::construct_delay_doppler(float *radial_vel_buffer,
													 float *depth_bufer,
													 float *normal_buffer,
													 int width, int height)
{
    int radar_width = 200;
    int radar_height = 200;
    RadarImage *rimg = new RadarImage(radar_width, radar_height);

    // find min and max vr and d
    float d_max = depth_bufer[0];
    float d_min = depth_bufer[0];
    float vr_min = radial_vel_buffer[0];
    float vr_max = radial_vel_buffer[0];

    for (int i = 0; i < width * height; i++)
    {
        if (radial_vel_buffer[i] > vr_max)
            vr_max = radial_vel_buffer[i];
        if (radial_vel_buffer[i] < vr_min)
            vr_min = radial_vel_buffer[i];

        if (depth_bufer[i] > d_max)
            d_max = depth_bufer[i];
        if (depth_bufer[i] < d_min)
            d_min = depth_bufer[i];
    }


    TRACE("vr min/max, d min/max: {}, {} | {}, {}", vr_min, vr_max, d_min,
          d_max);

    vr_min = -1.;
    vr_max = 1.0;

    d_min = 0.;
    d_max = 1.;


    float *delay_doppler = new float[radar_width * radar_height];
    for (int i = 0; i < radar_width * radar_height; i++)
        delay_doppler[i] = 0.;

    for (int i = 0; i < width * height; i++)
    {
        if (depth_bufer[i] == 0.) // background
            continue;

        int vr = floor(radar_width * (radial_vel_buffer[i] - vr_min) /
                       (vr_max - vr_min));
        int d =
            floor(radar_height * (depth_bufer[i] - d_min) / (d_max - d_min));

        // 		if (i == 200*400 + 170)
        // 			TRACE("vr, d: {}, {}", vr, d);

        if (d < 0 || d >= radar_height)
            continue;
        if (vr < 0 || vr >= radar_width)
            continue;

        delay_doppler[d * radar_width + vr] += normal_buffer[i];
    }

    // normalize delay_doppler
    float dd_max = delay_doppler[0];
    for (int i = 0; i < radar_width * radar_height; i++)
        if (delay_doppler[i] > dd_max)
            dd_max = delay_doppler[i];
    for (int i = 0; i < radar_width * radar_height; i++)
        delay_doppler[i] /= dd_max;

    rimg->texture->update(delay_doppler, delay_doppler, delay_doppler);

    delete[] delay_doppler;
    return rimg;
}
