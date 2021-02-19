#ifndef OBSERVEPANEL_H_
#define OBSERVEPANEL_H_

#include "Entity.h"

class MainLayer;

using namespace std;

class ObservePanel
{
public:
	ObservePanel() = default;
	ObservePanel(MainLayer*);
	~ObservePanel();

	void on_imgui_render();
private:
	MainLayer *layer;
	int selected_target_idx = 0;
	int selected_observer_idx = 0;
	int selected_light_idx = 0;

	void observe_button();
	vector<Entity> get_scene_entities();
	vector<Entity> get_scene_root_children();
	void append_children(vector<Entity> &ents, Entity entity);
	void target_selection_panel(vector<Entity>&);
	void observer_selection_panel(vector<Entity>&);
	void light_selection_panel(vector<Entity> &ents);

	void preselect();

};

#endif /* OBSERVEPANEL_H_ */
