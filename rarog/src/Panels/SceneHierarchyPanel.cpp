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

// 	scene->registry.each([&](auto entityID)
// 	{
// 		Entity entity(entityID, &scene->registry);
// // 		if (entity.parent == NULL)  // root
// 			draw_entity_node(entity);
//
// 	});
	draw_entity_node(scene->root_entity, false);

	// deselect when clicked elsewhere
	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		scene->selected_entity = Entity();
	}


	ImGui::End();


	ImGui::Begin("Properties");
	if (scene->selected_entity && scene->selected_entity.get_parent())
	{
		 draw_selected_properties(scene->selected_entity);
	}
	ImGui::End();

	if (entity_to_delete)
	{
		scene->selected_entity = Entity();
		entity_to_delete.destroy();
		entity_to_delete = Entity();
	}

}


void SceneHierarchyPanel::draw_entity_node(Entity &entity, bool drag_source)
{
	string &tag = entity.get_component<TagComponent>().tag;

    ImGuiTreeNodeFlags flags =
        ((scene->selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
		| ImGuiTreeNodeFlags_DefaultOpen;

    bool opened =
        ImGui::TreeNodeEx((void *)(entity.get_handle()), flags, tag.c_str());

	// Drag and Drop
	if (drag_source && ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("_TREENODE", (void*)(&entity), 256);
		ImGui::Text("Move Entity");

		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
		{
			Entity payload_entity = *(const Entity*)payload->Data;
			payload_entity.detatch();
			entity.add_child(payload_entity);
		}
		ImGui::EndDragDropTarget();
	}

    if (ImGui::IsItemClicked())
    {
		scene->selected_entity = entity;
    }



	// left click popup
	if (ImGui::BeginPopupContextItem()) // not over any item
	{
		char txt[50];
		sprintf(txt, "%s Create Child Entity", "\xef\x81\xa7");
		if (ImGui::MenuItem(txt))
		{
			Entity e = scene->create_entity("Unnamed Entity");
			entity.add_child(e);
		}

		if (entity != scene->root_entity)
		{
			ImGui::Spacing(); ImGui::Separator();
			sprintf(txt, "%s Delete Entity", "\xef\x80\x8d");
			if (ImGui::MenuItem(txt))
			{
				entity_to_delete = entity;
			}
		}

		ImGui::EndPopup();
	}


    if (opened)
	{
		// list components
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		if (entity.has_component<MeshComponent>())
		{
			ImGui::TreeNodeEx((void*)1231231, flags, "Mesh Component");
			// 		if (ImGui::IsItemClicked())
		}
		if (entity.has_component<TextureComponent>())
		{
			ImGui::TreeNodeEx((void*)1231231, flags, "Texture Component");
			// 		if (ImGui::IsItemClicked())
		}
		if (entity.has_component<ShaderComponent>())
		{
			ImGui::TreeNodeEx((void*)1231231, flags, "Shader Component");
			// 		if (ImGui::IsItemClicked())
		}
		if (entity.has_component<CameraComponent>())
		{
			ImGui::TreeNodeEx((void*)1231231, flags, "Camera Component");
			// 		if (ImGui::IsItemClicked())
		}
		if (entity.has_component<NativeScriptComponent>())
		{
			ImGui::TreeNodeEx((void*)1231231, flags, "Native Script Component");
			// 		if (ImGui::IsItemClicked())
		}

		// list children
		for (Entity &child: entity.get_component<SceneGraphComponent>().children)
			draw_entity_node(child, true);

		ImGui::TreePop();
	}

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
		// TODO: convertions rad -> deg, and positions * multiplicant
		Transform &t = entity.get_component<Transform>();
		glm::vec3 rot_deg;
		rot_deg = (float)(180./M_PI) * t.rotation ;
		ImGui::DragFloat3("Position", glm::value_ptr(t.position), 0.1);
		ImGui::DragFloat3("Rotation [deg]", glm::value_ptr(rot_deg), 0.1);
		ImGui::DragFloat3("Scale", glm::value_ptr(t.scale), 0.1);
		t.rotation = rot_deg * (float)(M_PI / 180.0);
	}
}
