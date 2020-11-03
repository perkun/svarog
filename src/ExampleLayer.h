#ifndef EXAMPLELAYER_H_
#define EXAMPLELAYER_H_

#include <iostream>
#include <stdio.h>
#include "Application.h"
#include "vendor/imgui/imgui.h"
// #include "Layer.h"
// #include "Scene/Entity.h"
// #include "Scene/Scene.h"
// #include "Event/WindowEvent.h"

using namespace std;


class ExampleLayer : public Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;

	void on_update(double ts) override;
	virtual void on_imgui_render() override;
	void on_event(Event& e) override;
	Scene *active_scene;
private:
	void on_window_resize_event(WindowResizeEvent&);
	void on_key_pressed_event(KeyPressedEvent&);
	void on_key_released_event(KeyReleasedEvent&);
	void on_mouse_button_pressed_event(MouseButtonPressedEvent&);
	void on_mouse_button_released_event(MouseButtonReleasedEvent&);
	void on_mouse_scrolled_event(MouseScrolledEvent&);
	void on_curosr_moved_event(MouseMovedEvent&);

	map<int, function<void(ExampleLayer*)> > key_pressed_map;
	map<int, function<void(ExampleLayer*)> > key_released_map;
	map<int, function<void(ExampleLayer*)> > mouse_button_pressed_map;
	map<int, function<void(ExampleLayer*)> > mouse_button_released_map;

	function<void(ExampleLayer*, vec2 cursor_shift)> mouse_cursor_action = NULL;
	function<void(ExampleLayer*, vec2 offset)> mouse_scrolled_action = NULL;

	Entity scene_camera, metis, plane, arrow;
// 	Mesh *plane_mesh;
// 	Texture *tex_1;
// 	Texture *tex_2;
// 	Shader *basic_shader;
// 	Camera *camera;


	float tmp[4];
};

#endif /* EXAMPLELAYER_H_ */
