#include "svpch.h"
#include "ObservePanel.h"
#include "MainLayer.h"


ObservePanel::ObservePanel(MainLayer *layer) : layer(layer)
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


ObservePanel::~ObservePanel()
{
}

void ObservePanel::on_imgui_render()
{
    vector<Entity> ents = get_scene_root_children();
    auto cam = dynamic_pointer_cast<OrthograficCamera>(
        layer->scene.observer.get_component<CameraComponent>().camera);

    ImGui::Begin("Observe");
    observe_button();
	ImGui::SameLine();
    target_selection_panel(ents);
	ImGui::SameLine();
    observer_selection_panel(ents);

    // observer camera fov
    ImGui::PushItemWidth(150.);
	ImGui::SameLine();
	ImGui::Text("FOV:");
	ImGui::SameLine();
    ImGui::DragFloat("##camera fov", &(cam->size_x), 0.02, 0.1, 100.);
	ImGui::End();

}

void ObservePanel::observe_button()
{
    string run_btn_label;
    if (layer->mode == Mode::EDITOR)
        run_btn_label = "Observe";
    else if (layer->mode == Mode::RUNTIME)
        run_btn_label = "Stop observing";

    ImGui::PushItemWidth(150.);
    if (ImGui::Button(run_btn_label.c_str()))
    {
        layer->toggle_mode();
    }
}

void ObservePanel::append_children(vector<Entity> &ents, Entity entity)
{
    for (Entity e : entity.get_children())
        ents.push_back(e);

    for (Entity e : entity.get_children())
        append_children(ents, e);
}


vector<Entity> ObservePanel::get_scene_entities()
{
    vector<Entity> ents;
    append_children(ents, layer->scene.root_entity);
    return ents;
}


vector<Entity> ObservePanel::get_scene_root_children()
{
    vector<Entity> ents;
    for (Entity e : layer->scene.root_entity.get_children())
        ents.push_back(e);
    return ents;
}


void ObservePanel::target_selection_panel(vector<Entity> &ents)
{
    if (ents.size() == 0)
        return;

	ImGui::Text("Target:");
	ImGui::SameLine();
    ImGui::PushItemWidth(150.);
    if (ImGui::BeginCombo(
            "##Target",
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


void ObservePanel::observer_selection_panel(vector<Entity> &ents)
{
    if (ents.size() == 0)
        return;

	ImGui::Text("Observer:");
	ImGui::SameLine();
    ImGui::PushItemWidth(150.);
    if (ImGui::BeginCombo("##Observer",
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
}


