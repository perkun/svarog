#ifndef SCENEHIERARCHYPANEL_H_
#define SCENEHIERARCHYPANEL_H_

#include "Scene.h"
#include "Utils/FileDialog.h"
#include "Batch.h"
#include "Utils/Time.h"

using namespace std;

class SceneHierarchyPanel
{
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(Scene*, double*);
	~SceneHierarchyPanel();

	void on_imgui_render();
	static void add_asteroid_from_file(Entity&, string);
	Entity get_selected_entity() { return selected_entity; }
	void set_selected_entity(Entity entity);

private:
	void draw_entity_node(Entity&);
	void draw_selected_properties(Entity&);
	void load_texture_from_file(Entity&);

	template<typename T, typename OnRemoveFunction>
	void list_component(Entity&, const char*, const char*, OnRemoveFunction);

	Scene *scene;
	double *julian_day;

	char buff[256];
	char u_name[50];

	Entity entity_to_delete, selected_entity;

	long int id = 123141232;
};


template<typename T, typename OnRemoveFunction>
void SceneHierarchyPanel::list_component(Entity &entity, const char *label,
										 const char *icon, OnRemoveFunction orf)
{
	ImGuiTreeNodeFlags flags =
		ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	if (entity.has_component<T>())
	{
		ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, "%s  %s",
						  icon, label);

		if (ImGui::BeginPopupContextItem())
		{
			sprintf(buff, "%s  Remove", "\xef\x80\x8d");
			if (ImGui::MenuItem(buff))
			{
				orf(entity);
				entity.remove_component<T>();
			}
			ImGui::EndPopup();
		}
// 		if (ImGui::IsItemClicked())
// 		{
//
// 		}
	}


	id++;

}

#endif /* SCENEHIERARCHYPANEL_H_ */
