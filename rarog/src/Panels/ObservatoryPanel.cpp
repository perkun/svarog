#include "svpch.h"
#include "MainLayer.h"
#include "ObservatoryPanel.h"
#include "Camera.h"

ObservatoryPanel::ObservatoryPanel(MainLayer *l, double *julian_day)
{
    layer = l;
	ObservationStorage *obs = new ObservationStorage;
    obs_storage.push_back(obs);
	this->julian_day = julian_day;
}

ObservatoryPanel::~ObservatoryPanel()
{
	for (int i = 0; i < obs_storage.size(); i++)
	{
		delete obs_storage[i];
	}
}




void ObservatoryPanel::add_obs_storage(string filename)
{
    ObservationStorage *storage = new ObservationStorage;
    if (storage->load(filename))
    {
        for (int i = 0; i < obs_storage.size(); i++)
            if (obs_storage[i]->name == storage->name)
                storage->name = storage->name + " (c)";

        obs_storage[current_storage]->detach_all_ghosts();
        obs_storage.emplace_back(storage);
        current_storage = obs_storage.size() - 1;
		set_current_ghosts(obs_storage[current_storage]);
    }
    else
        cout << "Adding Storage Failed" << endl;
}



void ObservatoryPanel::set_current_ghosts(ObservationStorage *obs)
{
    // lc
    if (obs->lightcurves->get_current_obs())
    {
        layer->ui_scene.root_entity.add_child(
            obs->lightcurves->get_current_obs()->ghost_target);
        layer->ui_scene.root_entity.add_child(
            obs->lightcurves->get_current_obs()->ghost_observer);
    }

    // ao
    if (obs->ao_images->get_current_obs())
    {
        layer->ui_scene.root_entity.add_child(
            obs->ao_images->get_current_obs()->ghost_target);
        layer->ui_scene.root_entity.add_child(
            obs->ao_images->get_current_obs()->ghost_observer);
    }
    // radar
    if (obs->radar_images->get_current_obs())
    {
        layer->ui_scene.root_entity.add_child(
            obs->radar_images->get_current_obs()->ghost_target);
        layer->ui_scene.root_entity.add_child(
            obs->radar_images->get_current_obs()->ghost_observer);
    }
}


void ObservatoryPanel::set_target_and_observer(Observation *obs)
{
    vec3 gtp = obs->target_transform.position;
    vec3 gop = obs->observer_transform.position;

    layer->observer_target = obs->target;
    layer->scene.observer = obs->observer;
    layer->observer_target.get_component<Transform>() = obs->target_transform;
    layer->scene.observer.get_component<Transform>() = obs->observer_transform;
    if (layer->observer_target.has_component<OrbitalComponent>())
        layer->observer_target.get_component<OrbitalComponent>() =
            obs->target_orbital_component;

    shared_ptr<Camera> camera =
        layer->scene.observer.get_component<CameraComponent>().camera;
    camera->position = gop;
    camera->update_target(gtp);
}


void ObservatoryPanel::observations_panel()
{

    for (int i = 0; i < 5; i++)
        ImGui::Spacing();

	ImGui::Text("Storage:");
	ImGui::SameLine();
    ImGui::PushItemWidth(150.);
    if (ImGui::BeginCombo("##Storage",
                          obs_storage[current_storage]->name.c_str()))
    {
        for (int i = 0; i < obs_storage.size(); i++)
        {
            const bool is_selected = (current_storage == i);
            if (ImGui::Selectable(obs_storage[i]->name.c_str(), is_selected))
            {
                obs_storage[current_storage]->detach_all_ghosts();
                current_storage = i;
                set_current_ghosts(obs_storage[current_storage]);
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

	if (obs_storage[current_storage]->file_loaded)
	{
		ImGui::Text("%d points defined in file.",
					(int)obs_storage[current_storage]->points.size());

	}

    for (int i = 0; i < 5; i++)
        ImGui::Spacing();

    ImGuiTabBarFlags tab_bar_flags =
        ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll;

    if (ImGui::BeginTabBar("Observations", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Lightcurves"))
        {
            // observations buttons
            if (ImGui::Button("Make lightcurve", ImVec2(150, 0)))
                make_lightcurve(layer->observer_target, layer->scene.observer,
                                obs_storage[current_storage]->lightcurves);
            ImGui::SameLine(0., 20.);
            ImGui::PushItemWidth(100.);
            ImGui::InputInt("LC num points", &lc_num_points);
            for (int i = 0; i < 10; i++)
                ImGui::Spacing();

            display_lightcurves(obs_storage[current_storage]->lightcurves);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("AO images"))
        {
            if (ImGui::Button("Make AO image", ImVec2(150, 0)))
                make_ao_image(layer->observer_target, layer->scene.observer,
                              obs_storage[current_storage]->ao_images);
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

            display_images(obs_storage[current_storage]->ao_images);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Radar images"))
        {
            if (ImGui::Button("Make Radar image", ImVec2(150, 0)))
                make_radar_image(layer->observer_target, layer->scene.observer,
                                 obs_storage[current_storage]->radar_images);

            ImGui::InputFloat("ang. speed", &angular_speed);

            for (int i = 0; i < 10; i++)
                ImGui::Spacing();

            display_images(obs_storage[current_storage]->radar_images);

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}


void ObservatoryPanel::on_imgui_render()
{
	ImGui::Begin("Observatory");

    ImGui::BeginChild("Observations panel",
                      ImVec2(ImGui::GetWindowContentRegionWidth(), 900),
                      false, ImGuiWindowFlags_MenuBar);

	menu_bar();
    observations_panel();
    ImGui::EndChild();

    ImGui::End();
}


void ObservatoryPanel::menu_bar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            if (ImGui::MenuItem("Import positions"))
            {
                add_obs_storage(FileDialog::open_file("*.yaml"));
            }

			bool enabled = (obs_storage[current_storage]->points.size() > 0);

            if (ImGui::MenuItem("make observations", NULL, false, enabled))
            {
                if (!layer->observer_target.has_component<OrbitalComponent>())
                {
                    cout << "Target does not have an Orbital Component" << endl;
                }
                else
                {
					observe_points(obs_storage[current_storage]);
                }
            }

			if (ImGui::MenuItem("save to file"))
			{
				ObservationStorage *storage = obs_storage[current_storage];

				string filename = FileDialog::save_file("*.yaml");

				storage->save(filename);

				for (int i = 0; i < obs_storage.size(); i++)
				{
					if (i == current_storage)
						continue;
					if (obs_storage[i]->name == storage->name)
						storage->name = storage->name + " (c)";
				}

			}

            ImGui::Separator();
            if (ImGui::MenuItem("delete all observations", NULL, false, true))
            {
				obs_storage[current_storage]->delete_all_observations();
            }


            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }


}


void ObservatoryPanel::observe_points(ObservationStorage* obs)
{
    OrbitalComponent &oc =
        layer->observer_target.get_component<OrbitalComponent>();
    double tmp_rotation_phase = oc.rotation_phase;

    vec3 &target_rotation =
        layer->observer_target.get_component<Transform>().rotation;
    vec3 tmp_rotation = target_rotation;
    vec3 &target_pos =
        layer->observer_target.get_component<Transform>().position;
    vec3 tmp_target_pos = target_pos;
    vec3 &observer_pos =
        layer->scene.observer.get_component<Transform>().position;
    vec3 tmp_observer_pos = observer_pos;

	int tmp_ao_size = ao_size;
	int tmp_lc_num_points = lc_num_points;


    for (YamlPoint p : obs->points)
    {
        oc.calculate_rot_phase(p.jd);
        target_rotation = oc.xyz_from_lbg();

        target_pos = p.target;
        observer_pos = p.observer;

		if (p.obs_types & ObsType::LC)
		{
			lc_num_points = p.lc_num_points;
			make_lightcurve(layer->observer_target, layer->scene.observer,
							obs->lightcurves);
		}

		if (p.obs_types & ObsType::AO)
		{
			ao_size = p.ao_size;
			make_ao_image(layer->observer_target, layer->scene.observer,
						  obs->ao_images);
		}

		if (p.obs_types & ObsType::RADAR)
		{
			make_radar_image(layer->observer_target, layer->scene.observer,
							 obs->radar_images);
		}
    }
    target_pos = tmp_target_pos;
    observer_pos = tmp_observer_pos;
    target_rotation = tmp_rotation;

	ao_size = tmp_ao_size;
	lc_num_points = tmp_lc_num_points;
}


void ObservatoryPanel::display_lightcurves(LightcurveSeries *lightcurves)
{
    if (lightcurves->size() == 0)
        return;

    Observation *current_obs = lightcurves->get_current_obs();

    ImGui::PushItemWidth(100.);
    if (ImGui::InputInt("Lc Nr", &lightcurves->current_id, 1))
    {
        current_obs = lightcurves->get_current_obs();
        lightcurves->detach_all_ghosts();
        if (current_obs)
        {
            layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
            layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
        }
    }

    ImGui::SameLine(0., 20.);
    if (ImGui::Button("set lc positions"))
        set_target_and_observer(current_obs);

    ImGui::PlotLines(
        "LC", static_cast<Lightcurve *>(current_obs)->inv_mag_data(),
        static_cast<Lightcurve *>(current_obs)->size(), 0, NULL,
        lightcurves->lcs_min, lightcurves->lcs_max, ImVec2(300.0f, 230.0f));

    if (ImGui::Button("Delete LC"))
    {
        lightcurves->delete_current_obs();
        current_obs = lightcurves->get_current_obs();
        if (current_obs)
        {
            layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
            layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
        }
    }

    if (ImGui::Button("Save magnitudes"))
        lightcurves->save_current_mag(FileDialog::save_file("*").c_str());

    ImGui::SameLine();
    if (ImGui::Button("Save flux"))
        lightcurves->save_current_flux(FileDialog::save_file("*").c_str());
}


void ObservatoryPanel::display_images(ImageSeries *images)
{
    if (images->size() == 0)
        return;

    Observation *current_obs = images->get_current_obs();
	if (!current_obs)
		cout << "IMG not valid!!!" << endl;

    ImGui::PushItemWidth(100.);
    if (ImGui::InputInt("Nr", &images->current_id, 1))
    {
        current_obs = images->get_current_obs();
        images->detach_all_ghosts();
        if (current_obs)
        {
            layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
            layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
        }
    }

    ImGui::SameLine(0., 20.);
    if (ImGui::Button("set positions"))
        set_target_and_observer(current_obs);

    long int tex_id =
        static_cast<Image *>(current_obs)->texture->get_texture_id();
    ImGui::Image((void *)tex_id, ImVec2(300, 300), ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::Button("Delete image"))
    {
        images->delete_current_obs();
        current_obs = images->get_current_obs();
        if (current_obs)
        {
            layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
            layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
        }
    }

    if (ImGui::Button("Save to png"))
        images->save(FileDialog::save_file("*.png").c_str());

    if (ImGui::Button("Save all"))
        images->save_all(FileDialog::save_file("*.png").c_str());
}


void ObservatoryPanel::make_lightcurve(Entity &target, Entity &observer,
                                       LightcurveSeries *lightcurves)
{
    Transform &tt = target.get_component<Transform>();
    OrbitalComponent &oc = target.get_component<OrbitalComponent>();

    int num_points = lc_num_points;
    int width = 256;
    int height = 256;
    float *pixel_buffer = new float[width * height];
    Lightcurve *lc = new Lightcurve(target, observer, num_points);
    vec4 bg_color = Application::get_bg_color();

    observer.get_component<CameraComponent>().camera->update_target(
        tt.position);

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
            layer->on_update(2 * M_PI / num_points);

        layer->framebuffer->bind();
        glReadPixels(0, 0, width, height, GL_RED, GL_FLOAT, pixel_buffer);

        double flux = 0.0;
        for (int j = 0; j < width * height; j++)
            flux += pixel_buffer[j];
        lc->push_flux(flux);
    }

    Entity ghost_observer = layer->ui_scene.create_entity("ghost observer");
    Entity ghost_target = layer->ui_scene.create_entity("ghost target");
    lc->add_ghosts(ghost_observer, ghost_target);
	lc->julian_day = *julian_day;


    lightcurves->push(lc);
    lightcurves->detach_all_ghosts();

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


void ObservatoryPanel::make_ao_image(Entity &target, Entity &observer,
                                     ImageSeries *ao_images)
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

    AoImage *ao = new AoImage(target, observer, ao_width, ao_height);
    ao->texture->update(pixel_buffer_r, pixel_buffer_g, pixel_buffer_b);

    Entity ghost_observer = layer->ui_scene.create_entity("ghost observer");
    Entity ghost_target = layer->ui_scene.create_entity("ghost target");
    ao->add_ghosts(ghost_observer, ghost_target);
	ao->julian_day = *julian_day;

    ao_images->push(ao);
    ao_images->detach_all_ghosts();

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


void ObservatoryPanel::make_radar_image(Entity &target, Entity &observer,
                                        ImageSeries *radar_images)
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

    RadarImage *rimg = new RadarImage(target, observer, 200, 200);
    rimg->construct_delay_doppler(pixel_buffer_r, pixel_buffer_depth,
                                  pixel_buffer_normal, frame_width,
                                  frame_height);


    Entity ghost_observer = layer->ui_scene.create_entity("ghost observer");
    Entity ghost_target = layer->ui_scene.create_entity("ghost target");
    rimg->add_ghosts(ghost_observer, ghost_target);
	rimg->julian_day = *julian_day;

    radar_images->push(rimg);
    radar_images->detach_all_ghosts();

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
