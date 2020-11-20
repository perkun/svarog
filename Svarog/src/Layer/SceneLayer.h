#ifndef SCENELAYER_H_
#define SCENELAYER_H_

#include <iostream>
#include <stdio.h>
#include "Layer.h"
#include "Event.h"
#include "Scene.h"
#include "Framebuffer.h"
#include "imgui.h"

using namespace std;

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
	Framebuffer *framefuffer;
protected:
	void on_window_resize_event(WindowResizeEvent&);
// 	void on_mouse_scrolled_event(MouseScrolledEvent&);
};

#endif /* SCENELAYER_H_ */
