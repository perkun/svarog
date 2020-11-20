#ifndef SHADOWSLAYER_H_
#define SHADOWSLAYER_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "SceneLayer.h"
#include "VertexArrayObject.h"
#include "VertexLayout.h"
#include "Batch.h"
#include "IndexedModel.h"
#include "Components.h"
#include "Input.h"
#include "ScriptableEntity.h"
#include "CameraController.h"

using namespace std;

class ShadowsLayer : public SceneLayer
{
public:
    ShadowsLayer();
	~ShadowsLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	void on_update(double ts) override;
	virtual void on_imgui_render() override;

private:
	double td;
	VertexArrayObject *plane_vao, *asteroid_vao, *cube_vao;
	ImgTexture *plane_texture, *asteroid_texture;
	Shader *shader, *color_shader;
	Entity plane, asteroid;//, scene_camera, scene_light;
};

#endif
