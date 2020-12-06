#ifndef SCENELAYER_H_
#define SCENELAYER_H_

#include "Layer.h"
#include "Event/WindowEvent.h"

using namespace std;

class Scene;

class SceneLayer : public Layer
{
public:
    SceneLayer();
	virtual ~SceneLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	void on_update(double ts) override;
	void on_event(Event& e) override;
	virtual void on_imgui_render() override;

	Scene *scene;
protected:
	void on_window_resize_event(WindowResizeEvent&);
// 	void on_mouse_scrolled_event(MouseScrolledEvent&);
};

#endif /* SCENELAYER_H_ */
