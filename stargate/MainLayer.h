#ifndef MAINLAYER_H_
#define MAINLAYER_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fitsio.h>
#include "EventLayer.h"
#include "VertexArrayObject.h"
#include "VertexLayout.h"
#include "Batch.h"
#include "IndexedModel.h"
#include "Components.h"

using namespace std;

class MainLayer : public EventLayer
{
public:
    MainLayer();
	~MainLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	void on_update(double ts) override;
	virtual void on_imgui_render() override;

private:
	Entity plane;

	Texture* input_image;
	Texture* fits_texture;

	VertexArrayObject* plane_vao;

	Shader *shader;
};

#endif /* BOXLAYER_H_ */


