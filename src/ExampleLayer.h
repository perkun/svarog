#ifndef EXAMPLELAYER_H_
#define EXAMPLELAYER_H_

#include <iostream>
#include <stdio.h>
#include "Application.h"
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
// 	virtual void OnImGuiRender() override;
	void on_event(Event& e) override;
	Scene *active_scene;
private:
	Entity scene_camera, metis, plane;
	void on_window_resize_event(WindowResizeEvent&);
// 	Mesh *plane_mesh;
// 	Texture *tex_1;
// 	Texture *tex_2;
// 	Shader *basic_shader;
// 	Camera *camera;
};

#endif /* EXAMPLELAYER_H_ */
