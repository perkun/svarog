#ifndef EventLayer_H_
#define EventLayer_H_

#include <iostream>
#include <stdio.h>
#include <map>
#include "Layer.h"
#include "Event.h"
#include "WindowEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "Scene.h"
#include "Framebuffer.h"
#include "Application.h"
#include "imgui.h"

using namespace std;


class EventLayer : public Layer
{
public:
	EventLayer();
	virtual ~EventLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	void on_update(double ts) override;
	void on_event(Event& e) override;
	virtual void on_imgui_render() override;

	Scene *scene;
	Framebuffer *framefuffer;
protected:
	void on_window_resize_event(WindowResizeEvent&);
	void on_key_pressed_event(KeyPressedEvent&);
	void on_key_released_event(KeyReleasedEvent&);
	void on_mouse_button_pressed_event(MouseButtonPressedEvent&);
	void on_mouse_button_released_event(MouseButtonReleasedEvent&);
	void on_mouse_scrolled_event(MouseScrolledEvent&);
	void on_curosr_moved_event(MouseMovedEvent&);

	map<int, function<void(EventLayer*)> > key_pressed_map;
	map<int, function<void(EventLayer*)> > key_released_map;
	map<int, function<void(EventLayer*)> > mouse_button_pressed_map;
	map<int, function<void(EventLayer*)> > mouse_button_released_map;

	function<void(EventLayer*, vec2 cursor_shift)> mouse_cursor_action = NULL;
	function<void(EventLayer*, vec2 offset)> mouse_scrolled_action = NULL;
};

#endif /* EventLayer_H_ */
