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
#include "SceneSerializer.h"
#include "Utils/FileDialog.h"
#include "Compute/MarchingCubes.h"
#include "Compute/PerlinNoise.h"
#include "Compute/VolumetricData.h"

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
	IndexedModel pts;
	VolumetricData<int> *vol_data;

	int mc_isolevel  = 128;
	int previous_iso = 128;

	float size_factor = 2;
	float previous_size_factor = 2;
	int seed = 0;

	double td;
	DynamicVertexArrayObject *space_vao;
	VertexArrayObject *plane_vao, *asteroid_vao, *cube_vao,  *points_vao;
	ImgTexture *plane_texture, *asteroid_texture;
	Shader *shader, *color_shader, *basic_shader;
	Entity plane, asteroid, space, points; //, scene_camera, scene_light;
};

#endif
