#ifndef MYLAYER_H_
#define MYLAYER_H_

#include <iostream>
#include <stdio.h>
#include "EventLayer.h"

using namespace std;

class MyLayer : public EventLayer
{
public:
    MyLayer();
	~MyLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	void on_update(double ts) override;
	virtual void on_imgui_render() override;

	string model_filename;
private:
	Entity model, scene_camera, light;

};

#endif /* LAYER/MYLAYER_H_ */
