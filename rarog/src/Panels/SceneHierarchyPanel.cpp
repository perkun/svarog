#include "svpch.h"
#include "SceneHierarchyPanel.h"
#include "Components.h"

#include <glm/gtc/type_ptr.hpp>

SceneHierarchyPanel::SceneHierarchyPanel(Scene *s)
{
	scene = s;
}


SceneHierarchyPanel::~SceneHierarchyPanel()
{
}

void SceneHierarchyPanel::on_imgui_render()
{
	ImGui::Begin("Scene Hierarchy");

	scene->registry.each([&](auto entityID)
	{
		Entity entity(entityID, &scene->registry);
// 		if (entity.parent == NULL)  // root
			draw_entity_node(entity);

	});

	// deselect when clicked elsewhere
	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		scene->selected_entity = Entity();
	}


	ImGui::End();


	ImGui::Begin("Properties");

	if (scene->selected_entity)
	{
		 draw_selected_properties(scene->selected_entity);
	}

	ImGui::End();
}


void SceneHierarchyPanel::draw_entity_node(Entity &entity)
{
    string tag = entity.get_component<TagComponent>().tag;

    ImGuiTreeNodeFlags flags =
        ((scene->selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
        ImGuiTreeNodeFlags_OpenOnArrow;

    bool opened =
        ImGui::TreeNodeEx((void *)(entity.get_handle()), flags, tag.c_str());

    if (ImGui::IsItemClicked())
    {
		scene->selected_entity = entity;
    }

    if (opened)
        ImGui::TreePop();

    // 	ImGui::Text("%s", tag.c_str());
}

void SceneHierarchyPanel::draw_selected_properties(Entity &entity)
{
	if (entity.has_component<TagComponent>())
	{
		// renameing entity
		string &tag = entity.get_component<TagComponent>().tag;
		memset(buff, 0, sizeof(buff));
		strcpy(buff, tag.c_str());
		if (ImGui::InputText("Tag", buff, sizeof(buff)))
			tag = string(buff);
	}


	if (entity.has_component<Transform>())
	{
		Transform &t = entity.get_component<Transform>();
		ImGui::DragFloat3("Position", glm::value_ptr(t.position), 0.1);
		ImGui::DragFloat3("Scale", glm::value_ptr(t.scale), 0.1);
		ImGui::DragFloat3("Rotation", glm::value_ptr(t.rotation), 0.1);
	}


}
