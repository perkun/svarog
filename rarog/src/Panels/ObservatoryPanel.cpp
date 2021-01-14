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

    ImGui::PushItemWidth(100.);
    if (ImGui::InputInt("Lc Nr", &lc_id, 1))
    {
        if (lc_id < 0)
            lc_id = 0;
        if (lc_id >= lightcurves.size())
            lc_id = lightcurves.size() - 1;

        for (Lightcurve &lc : lightcurves)
        {
            lc.ghost_target.detatch();
            lc.ghost_observer.detatch();
        }

        layer->ui_scene.root_entity.add_child(lightcurves[lc_id].ghost_target);
        layer->ui_scene.root_entity.add_child(
            lightcurves[lc_id].ghost_observer);
    }

    ImGui::SameLine(0., 20.);
    if (ImGui::Button("set lc positions"))
    {
        set_target_and_observer(
            lightcurves[lc_id].ghost_target, lightcurves[lc_id].ghost_observer,
            lightcurves[lc_id].target, lightcurves[lc_id].observer);
    }

    ImGui::PlotLines("LC", lightcurves[lc_id].inv_mag_data(),
                     lightcurves[lc_id].size(), 0, NULL, lcs_min, lcs_max,
                     ImVec2(200.0f, 130.0f));

    if (ImGui::Button("Delete LC"))
    {
        lightcurves[lc_id].ghost_observer.destroy();
        lightcurves[lc_id].ghost_target.destroy();
        lightcurves.erase(lightcurves.begin() + lc_id);
        lc_id = lightcurves.size() - 1;
        if (lc_id >= 0)
        {
            layer->ui_scene.root_entity.add_child(
                lightcurves[lc_id].ghost_target);
            layer->ui_scene.root_entity.add_child(
                lightcurves[lc_id].ghost_observer);
        }
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

    ImGui::PushItemWidth(100.);
    if (ImGui::InputInt("AO Nr", &ao_id, 1))
    {
        if (ao_id < 0)
            ao_id = 0;
        if (ao_id >= ao_images.size())
            ao_id = ao_images.size() - 1;

        for (AoImage &ao : ao_images)
        {
            ao.ghost_observer.detatch();
            ao.ghost_target.detatch();
        }

        layer->ui_scene.root_entity.add_child(ao_images[ao_id].ghost_target);
        layer->ui_scene.root_entity.add_child(ao_images[ao_id].ghost_observer);
    }

    ImGui::SameLine(0., 20.);
    if (ImGui::Button("set ao positions"))
    {
        set_target_and_observer(
            ao_images[ao_id].ghost_target, ao_images[ao_id].ghost_observer,
            ao_images[ao_id].target, ao_images[ao_id].observer);
    }
    long int tex_id = ao_images[ao_id].texture->get_texture_id();
    ImGui::Image((void *)tex_id, ImVec2(300, 300), ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::Button("Delete AO image"))
    {
        ao_images[ao_id].ghost_observer.destroy();
        ao_images[ao_id].ghost_target.destroy();
        ao_images.erase(ao_images.begin() + ao_id);
        ao_id = ao_images.size() - 1;
        if (ao_id >= 0)
        {
            layer->ui_scene.root_entity.add_child(
                ao_images[ao_id].ghost_target);
            layer->ui_scene.root_entity.add_child(
                ao_images[ao_id].ghost_observer);
        }
    }

    if (ImGui::Button("Save to png"))
        ao_images[ao_id].texture->save(FileDialog::save_file("*.png").c_str());
}


void ObservatoryPanel::make_lightcurve(Entity &target, Entity &observer)
{
    Transform &t = target.get_component<Transform>();
	OrbitalComponent &oc = target.get_component<OrbitalComponent>();


    int num_points = lc_num_points;
    int width = 256;
    int height = 256;
    float *pixel_buffer = new float[width * height];
    Lightcurve lc(num_points);
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
        lc.push_flux(flux);
    }

    lc.make_average_zero();
    float min = lc.calculate_min_inv_mag();
    float max = lc.calculate_max_inv_mag();
    if (min < lcs_min)
        lcs_min = min;
    if (max > lcs_max)
        lcs_max = max;

    lc.ghost_observer = layer->ui_scene.create_entity("LC ghost observer");
    lc.ghost_observer
        .add_component<Material>(Application::shaders["flat_shader"])
        .uniforms_vec4["u_color"] = vec4(32 / 256., 172 / 256., 64 / 256., 0.2);
    lc.ghost_observer.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedIcoSphere(vec3(0.), vec3(0.03))));
    Transform &got = lc.ghost_observer.get_component<Transform>();
    got.position = observer.get_component<Transform>().position;

    lc.ghost_target = layer->ui_scene.create_entity("LC ghost target");
    lc.ghost_target.add_component<Material>(Application::shaders["flat_shader"])
        .uniforms_vec4["u_color"] = vec4(32 / 256., 172 / 256., 64 / 256., 0.2);
    lc.ghost_target.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedCube(vec3(-.025), vec3(0.05))));
    Transform &gtt = lc.ghost_target.get_component<Transform>();
    gtt.position = target.get_component<Transform>().position;

    for (Lightcurve &lc : lightcurves)
    {
        lc.ghost_target.detatch();
        lc.ghost_observer.detatch();
    }
    layer->ui_scene.root_entity.add_child(lc.ghost_observer);
    layer->ui_scene.root_entity.add_child(lc.ghost_target);

    lc.target = target;
    lc.observer = observer;

    lightcurves.push_back(lc);
    lc_id = lightcurves.size() - 1;

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
			vec3(0.,0.39774,0.917498);

    // make image
    layer->on_update(0.);
    layer->framebuffer->bind();
    glReadPixels(0, 0, ao_width, ao_height, GL_RED, GL_FLOAT, pixel_buffer_r);
    glReadPixels(0, 0, ao_width, ao_height, GL_GREEN, GL_FLOAT, pixel_buffer_g);
    glReadPixels(0, 0, ao_width, ao_height, GL_BLUE, GL_FLOAT, pixel_buffer_b);

    AoImage ao(ao_width, ao_height);
    ao.texture->update(pixel_buffer_r, pixel_buffer_g, pixel_buffer_b);

    ao.ghost_observer = layer->ui_scene.create_entity("AO ghost observer");
    ao.ghost_observer
        .add_component<Material>(Application::shaders["flat_shader"])
        .uniforms_vec4["u_color"] = vec4(237 / 256., 84 / 256., 84 / 256., 0.2);
    ao.ghost_observer.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedIcoSphere(vec3(0.), vec3(0.03))));
    Transform &got = ao.ghost_observer.get_component<Transform>();
    got.position = observer.get_component<Transform>().position;

    ao.ghost_target = layer->ui_scene.create_entity("AO ghost target");
    ao.ghost_target.add_component<Material>(Application::shaders["flat_shader"])
        .uniforms_vec4["u_color"] = vec4(237 / 256., 84 / 256., 84 / 256., 0.2);
    ao.ghost_target.add_component<MeshComponent>(
        make_shared<VertexArrayObject>(IndexedCube(vec3(-0.05), vec3(0.05))));
    Transform &gtt = ao.ghost_target.get_component<Transform>();
    gtt.position = target.get_component<Transform>().position;

    for (AoImage &ao : ao_images)
    {
        ao.ghost_observer.detatch();
        ao.ghost_target.detatch();
    }
    layer->ui_scene.root_entity.add_child(ao.ghost_observer);
    layer->ui_scene.root_entity.add_child(ao.ghost_target);

    ao.target = target;
    ao.observer = observer;

    ao_images.push_back(ao);

    ao_id = ao_images.size() - 1;

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
