#ifndef IMGUILAYER_H_
#define IMGUILAYER_H_

#include <iostream>
#include <stdio.h>
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backends/imgui_impl_glfw.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"
#include "Layer.h"

using namespace std;

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
	~ImGuiLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_event(Event&) override;
	void begin();
	void end();

};

#endif /* IMGUILAYER_H_ */
