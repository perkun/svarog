#ifndef BOXLAYER_H_
#define BOXLAYER_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "EventLayer.h"
#include "VertexArrayObject.h"
#include "VertexLayout.h"
#include "Batch.h"
#include "IndexedModel.h"

using namespace std;

class BoxLayer : public EventLayer
{
public:
    BoxLayer();
	~BoxLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	void on_update(double ts) override;
	virtual void on_imgui_render() override;

private:
	Entity cube, scene_camera, plane;
	Entity cube_array[20][20][20];

	double td;
};

#endif /* BOXLAYER_H_ */

