#include "svpch.h"
#include "MainLayer.h"
#include "ObservatoryPanel.h"
#include "Camera.h"

ObservatoryPanel::ObservatoryPanel(MainLayer *l, double *julian_day,
                                   ObsStoragePack *obs)
{
    layer = l;
    obs_storage = obs;
    this->julian_day = julian_day;
}

ObservatoryPanel::~ObservatoryPanel()
{
}


void ObservatoryPanel::load_obs_storage(string filepath)
{
    if (filepath == "")
        return;

	vector<ObsPoint> obs_points = ObsStoragePack::import_obs_points(filepath);
	if (obs_points.size() > 0)
	{
		obs_storage->add_new(File::remove_extension(File::file_base(filepath)));
		observe_obs_points(obs_points);
	}
	else
        cout << "Adding Storage Failed" << endl;


//     if (obs_storage->load(filepath) != -1)
//     {
//         set_current_ghosts(obs_storage);
//     }
//     else
//         cout << "Adding Storage Failed" << endl;
}


void ObservatoryPanel::set_current_ghosts(ObsStoragePack *obs)
{
    // lc
    Observation *current_obs =
        obs->get_current_lightcurves()->get_current_obs();
    if (current_obs)
    {
        layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
        layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
    }

    // ao
    current_obs = obs->get_current_ao_images()->get_current_obs();
    if (current_obs)
    {
        layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
        layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
    }
    // radar
    current_obs = obs->get_current_radar_images()->get_current_obs();
    if (current_obs)
    {
        layer->ui_scene.root_entity.add_child(current_obs->ghost_target);
        layer->ui_scene.root_entity.add_child(current_obs->ghost_observer);
    }
}


void ObservatoryPanel::set_target_and_observer(Observation *obs)
{
    vec3 gtp = obs->target_transform.position;
    vec3 gop = obs->observer_transform.position;

    layer->scene.target = obs->target;
    layer->scene.observer = obs->observer;

    layer->scene.target.get_component<Transform>() = obs->target_transform;
    layer->scene.observer.get_component<Transform>() = obs->observer_transform;

    if (layer->scene.observer.has_component<OrbitalComponent>())
        layer->scene.observer.get_component<OrbitalComponent>() =
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
    if (ImGui::BeginCombo("##Storage", obs_storage->get_current_name().c_str()))
    {
        for (int i = 0; i < obs_storage->size(); i++)
        {
            const bool is_selected = (obs_storage->current_id == i);
            if (ImGui::Selectable(obs_storage->get_name(i).c_str(),
                                  is_selected))
            {
                obs_storage->detach_current_ghosts();
                obs_storage->current_id = i;
                set_current_ghosts(obs_storage);
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
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
                make_lightcurve(layer->scene.target, layer->scene.observer,
                                obs_storage->get_current_lightcurves(),
                                lc_num_points);
            ImGui::SameLine(0., 20.);
            ImGui::PushItemWidth(100.);
            ImGui::InputInt("LC num points", &lc_num_points);
            for (int i = 0; i < 10; i++)
                ImGui::Spacing();

            display_lightcurves(obs_storage->get_current_lightcurves());

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("AO Images"))
        {
            if (ImGui::Button("Make AO image", ImVec2(150, 0)))
                make_ao_image(layer->scene.target, layer->scene.observer,
                              obs_storage->get_current_ao_images(), ao_size);
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

            display_images(obs_storage->get_current_ao_images());

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Radar Images"))
        {
            if (ImGui::Button("Make Radar image", ImVec2(150, 0)))
                make_radar_image(layer->scene.target, layer->scene.observer,
                                 obs_storage->get_current_radar_images(), dd_size);
            ImGui::SameLine(0.0, 20.0);
            ImGui::PushItemWidth(100.);
            ImGui::InputInt("size [px]", &dd_size, 1, 100);

            ImGui::InputFloat("ang. speed", &angular_speed);

            for (int i = 0; i < 10; i++)
                ImGui::Spacing();

            display_images(obs_storage->get_current_radar_images());

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}


void ObservatoryPanel::on_imgui_render()
{
    ImGui::Begin("Observatory");
    observations_panel();
    ImGui::End();
}


void ObservatoryPanel::observe_obs_points(const vector<ObsPoint> obs_points)
{
    OrbitalComponent &oc =
        layer->scene.target.get_component<OrbitalComponent>();
    double tmp_rotation_phase = oc.rotation_phase;

    vec3 &target_rotation =
        layer->scene.target.get_component<Transform>().rotation;
    vec3 tmp_rotation = target_rotation;
    vec3 &target_pos =
        layer->scene.target.get_component<Transform>().position;
    vec3 tmp_target_pos = target_pos;
    vec3 &observer_pos =
        layer->scene.observer.get_component<Transform>().position;
    vec3 tmp_observer_pos = observer_pos;

	double tmp_julian_day = *julian_day;

    int tmp_ao_size = ao_size;
    int tmp_lc_num_points = lc_num_points;


    for (ObsPoint p : obs_points)
    {
		*julian_day = p.jd;

        oc.calculate_rot_phase(p.jd);
        target_rotation = oc.xyz_from_lbg();

        target_pos = p.target_pos;
        observer_pos = p.observer_pos;

        if (p.obs_types & ObsType::LC)
        {
            lc_num_points = p.lc_num_points;
            make_lightcurve(layer->scene.target, layer->scene.observer,
                            obs_storage->get_current_lightcurves(),
                            lc_num_points);
        }

        if (p.obs_types & ObsType::AO)
        {
            ao_size = p.ao_size;
            make_ao_image(layer->scene.target, layer->scene.observer,
                          obs_storage->get_current_ao_images(), ao_size);
        }

        if (p.obs_types & ObsType::RADAR)
        {
            make_radar_image(layer->scene.target, layer->scene.observer,
                             obs_storage->get_current_radar_images(), radar_size);
        }
    }
    target_pos = tmp_target_pos;
    observer_pos = tmp_observer_pos;
    target_rotation = tmp_rotation;

	*julian_day = tmp_julian_day;

    ao_size = tmp_ao_size;
    lc_num_points = tmp_lc_num_points;
}


void ObservatoryPanel::display_lightcurves(LightcurveSeries *lightcurves)
{
    if (lightcurves->size() == 0)
        return;

    Observation *current_obs = lightcurves->get_current_obs();

	ImGui::Text("%lf", current_obs->julian_day);

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

//     if (ImGui::Button("Save magnitudes"))
//         lightcurves->save_current_mag(FileDialog::save_file("*").c_str());
//
//     ImGui::SameLine();
//     if (ImGui::Button("Save flux"))
//         lightcurves->save_current_flux(FileDialog::save_file("*").c_str());
}


void ObservatoryPanel::display_images(ImageSeries *images)
{
    if (images->size() == 0)
        return;

    Observation *current_obs = images->get_current_obs();
    if (!current_obs)
        cout << "IMG not valid!!!" << endl;


    ImGui::Text("%lf", current_obs->julian_day);

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

//     ImGui::Text("Save");
//     ImGui::SameLine();
//     if (ImGui::Button("single png"))
//         images->save_png(FileDialog::save_file("*.png").c_str());
//
//     ImGui::SameLine();
//     if (ImGui::Button("all pngs"))
//         images->save_all_png(FileDialog::save_file("*.png").c_str());
//
//
//     ImGui::Text("Save");
//     ImGui::SameLine();
//     if (ImGui::Button("single fits"))
//         images->save_fits_greyscale(FileDialog::save_file("*.fits").c_str());
//
//     ImGui::SameLine();
//     if (ImGui::Button("all fits"))
//         images->save_all_fits(FileDialog::save_file("*.fits").c_str());
}


void ObservatoryPanel::make_lightcurve(Entity &target, Entity &observer,
                                       LightcurveSeries *lightcurves,
                                       int num_points)
{
    Transform &tt = target.get_component<Transform>();
	if (!target.has_component<OrbitalComponent>())
	{
		cout << "target does not have orbital component" << endl;
		return;
	}

    OrbitalComponent &oc = target.get_component<OrbitalComponent>();

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
                                     ImageSeries *ao_images, int ao_size)
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
//     ao->texture->update(pixel_buffer_r, pixel_buffer_g, pixel_buffer_b);
	ao->update_data(pixel_buffer_r, pixel_buffer_g, pixel_buffer_b);
	ao->update_texture();

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
                                        ImageSeries *radar_images,
                                        int dd_size)
{

    int frame_width = dd_size * 5;
    int frame_height = dd_size * 5;

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


    // create view box that fits the target perfectly
    Transform &tt = target.get_component<Transform>();
    Transform &ot = observer.get_component<Transform>();
    MeshComponent &mc = target.get_component<MeshComponent>();

    float max_scale = glm::max(tt.scale.x, tt.scale.y);
    max_scale = glm::max(max_scale, tt.scale.z);

    float near =
        glm::length(tt.position - ot.position) - (max_scale * mc.r_max);
    float far = glm::length(tt.position - ot.position) + (max_scale * mc.r_max);

//    	TRACE("near: {}, far: {}, rmax: {}", near, far, mc.r_max);
    camera_comp.camera =
        make_shared<OrthograficCamera>(max_scale * mc.r_max, 1., near, far);


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

    RadarImage *rimg =
        new RadarImage(target, observer, dd_size, dd_size, near, far, -1, 1);
    rimg->construct_delay_doppler(pixel_buffer_r, pixel_buffer_depth,
                                  pixel_buffer_normal, frame_width,
                                  frame_height);
    rimg->update_texture();

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
