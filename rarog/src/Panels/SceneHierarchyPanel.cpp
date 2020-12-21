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
	draw_entity_node(scene->root_entity);

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



	// deleting
	if (entity_to_delete)
	{
		if (scene->selected_entity == entity_to_delete)
			scene->selected_entity = Entity();
		entity_to_delete.destroy();
		entity_to_delete = Entity();
	}

}


void SceneHierarchyPanel::draw_entity_node(Entity &entity)
{
	string &tag = entity.get_component<TagComponent>().tag;

    ImGuiTreeNodeFlags flags =
        ((scene->selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0)
		| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
		| ((entity == scene->root_entity) ? ImGuiTreeNodeFlags_DefaultOpen : 0);

    bool opened =
        ImGui::TreeNodeEx((void *)(entity.get_handle()), flags, tag.c_str());

	// Drag and Drop
	if (entity != scene->root_entity && ImGui::BeginDragDropSource())
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
	if (ImGui::BeginPopupContextItem())
	{
		sprintf(buff, "%s Add Empty Entity", "\xef\x81\xa7");
		if (ImGui::MenuItem(buff))
		{
			Entity e = scene->create_entity("Unnamed Entity");
			entity.add_child(e);
		}

		if (entity != scene->root_entity)
		{
			sprintf(buff, "%s Add Mesh Component", "\xef\x83\x86");
			if (!entity.has_component<MeshComponent>() && ImGui::BeginMenu(buff))
			{
				if (ImGui::MenuItem("Cube"))
				{
					entity.add_component<MeshComponent>(
						make_shared<VertexArrayObject>(IndexedCube())
					);
				}

				ImGui::EndMenu();
			}

			sprintf(buff, "%s Add Material Component", "\xef\x83\x86");
			if (!entity.has_component<Material>() && ImGui::BeginMenu(buff))
			{
				for (pair<string, Shader*> element: scene->shaders)
				{
					if (ImGui::MenuItem(element.first.c_str()))
					{
						entity.add_component<Material>(element.second);
					}
				}
					ImGui::EndMenu();
			}

			sprintf(buff, "%s Add Texture Component", "\xef\x83\x86");
			if (!entity.has_component<TextureComponent>() && ImGui::BeginMenu(buff))
			{
				if (ImGui::MenuItem("From File"))
				{
					load_texture_from_file(entity);
				}
				ImGui::EndMenu();
			}

			ImGui::Spacing(); ImGui::Separator();
			sprintf(buff, "%s Delete Entity", "\xef\x80\x8d");
			if (ImGui::MenuItem(buff))
			{
				entity_to_delete = entity;
			}
		}

		ImGui::EndPopup();
	}


    if (opened)
	{
		// list components
		list_component<MeshComponent>(entity, "Mesh Component", "\xef\x83\x86",
			[](Entity &e){});
		list_component<TextureComponent>(entity, "Texture Component", "\xef\x83\x86",
			[](Entity &e)
			{
				if (e.has_component<Material>())
					e.get_component<Material>().uniforms_int["u_has_texture"] = 0;
			});
		list_component<Material>(entity, "Material Component", "\xef\x83\x86",
			[](Entity &e){});
		list_component<CameraComponent>(entity, "Camera Component", "\xef\x83\x86",
			[](Entity &e){});
		list_component<NativeScriptComponent>(entity, "Native Script Component", "\xef\x83\x86",
			[](Entity &e){});

		// list children
		for (Entity &child: entity.get_component<SceneGraphComponent>().children)
			draw_entity_node(child);

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



void SceneHierarchyPanel::load_texture_from_file(Entity &entity)
{
    string filename = FileDialog::open_file("*.jpg *.png *.jpeg");
	if (filename.empty())
		return;

    INFO("Loading texture {}", filename);

    if (entity.has_component<TextureComponent>())
	{
		delete entity.get_component<TextureComponent>().texture;
		entity.replace_component<TextureComponent>(new ImgTexture(filename));
	}
    else
        entity.add_component<TextureComponent>(new ImgTexture(filename));

	if (entity.has_component<Material>())
    	entity.get_component<Material>().uniforms_int["u_has_texture"] = 1;
}
