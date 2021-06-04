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
	void on_key_pressed_event(KeyPressedEvent &event);
	void on_mouse_moved_event(MouseMovedEvent &event);

	void info_overlay();
	void help_overlay();

	void print_screen();

	Args args;
	Scene scene;

	Entity axes;

	Framebuffer *ms_framebuffer, *framebuffer;

	bool overlay_open = true;
	bool display_axes = false;
	bool display_help = false;

	vector<shared_ptr<Shader> > shader_ptrs;

	char overlay_text[5000];

	vec3 init_model_pos;
	vec2 viewport_panel_size;

	vec2 last_cursor_pos;
};

#endif