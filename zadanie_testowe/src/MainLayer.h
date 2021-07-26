#ifndef MAINLAYER_H_
#define MAINLAYER_H_

#include "Layer.h"
#include "Scene.h"
#include "Batch.h"
#include "Utils/FileDialog.h"


class Line
{
public:
	Line() {}
	Line(vec3 start, vec3 stop)
	{
		this->start = start;
		this->stop = stop;
		update();
	}
	Line(vec3 start, vec3 stop, vec4 color)
	{
		this->start = start;
		this->stop = stop;
		this->color = color;
		update();
	}

	~Line() {}

	void update() {
		indexed_model = IndexedLine(start, stop, color);
	}

	vec3 start, stop;
	vec4 color = vec4(0.8, 0.2, 0.2, 1.0);
	IndexedModel indexed_model;

private:

};




class MainLayer : public Layer
{
public:
    MainLayer();
    ~MainLayer();

    virtual void on_attach() override;
    virtual void on_detach() override;
    virtual void on_update(double ts) override;
    virtual void on_imgui_render() override;
    virtual void on_event(Event &) override;

private:
    void on_key_released_event(KeyReleasedEvent &event);
    void on_key_pressed_event(KeyPressedEvent &event);
    void on_mouse_moved_event(MouseMovedEvent &event);
	void on_mouse_button_pressed_event(MouseButtonPressedEvent &event);

	vec2 get_mouse_pos();

	vector<Line> lines;
	Line new_line;

	Entity lines_ent, new_line_ent;

    Scene scene;

	bool context_menu_open = false;

    Framebuffer *ms_framebuffer, *framebuffer;

    vector<shared_ptr<Shader>> shader_ptrs;

    vec2 viewport_panel_size;
	vec2 vieport_bounds[2];
    vec2 last_cursor_pos;

    // defaults:
    float cam_size_x = 1.0;
    vector<vec4> bg_colors;
};


#endif
