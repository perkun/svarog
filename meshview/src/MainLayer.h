#ifndef MAINLAYER_H_
#define MAINLAYER_H_

#include <cppargs/Parser.h>
#include "Layer.h"
#include "Scene.h"
#include "Utils/FileDialog.h"

class MainLayer : public Layer
{
public:
    MainLayer(Args args);
	~MainLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_update(double ts) override;
	virtual void on_imgui_render() override;
	virtual void on_event(Event&) override;

private:
	void on_key_released_event(KeyReleasedEvent &event);
	void on_mouse_moved_event(MouseMovedEvent &event);


	Args args;
	Scene scene;

	Framebuffer *ms_framebuffer, *framebuffer;

	vec3 init_model_pos;
	vec2 viewport_panel_size;

	vec2 last_cursor_pos;
};

#endif
