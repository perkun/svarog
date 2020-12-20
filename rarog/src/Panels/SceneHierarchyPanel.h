#ifndef SCENEHIERARCHYPANEL_H_
#define SCENEHIERARCHYPANEL_H_

#include "Scene.h"
#include "Utils/FileDialog.h"

using namespace std;

class SceneHierarchyPanel
{
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(Scene*);
	~SceneHierarchyPanel();

	void on_imgui_render();

private:
	void draw_entity_node(Entity&);
	void draw_selected_properties(Entity&);
	void load_texture_from_file(Entity&);

	Scene *scene;

	char buff[256];

	Entity entity_to_delete;
};

#endif /* SCENEHIERARCHYPANEL_H_ */
