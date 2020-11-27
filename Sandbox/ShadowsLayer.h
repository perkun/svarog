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
// #include "CameraControllerFPP.h"
#include "SceneSerializer.h"
#include "Utils/FileDialog.h"
#include "Compute/MarchingCubes.h"
#include "Compute/PerlinNoise.h"
#include "Compute/VolumetricData.h"
#include "Compute/SurfaceData.h"

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
	SurfaceData<float> *perlin_data;
	IndexedModel pts;
	VolumetricData<float> *vol_data;

	int mc_isolevel  = 128;
	int previous_iso = 128;

	float size_factor = 10;
	float previous_size_factor = 2;
	int seed = 0;

	double td;
	DynamicVertexArrayObject *space_vao;
	VertexArrayObject *plane_vao, *asteroid_vao, *cube_vao,  *points_vao;
	ImgTexture *plane_texture, *asteroid_texture;
	Texture *perlin_tex;
	Texture *perlin_tex_flat;
	Shader *shader, *color_shader, *basic_shader, *displacement_shader;
	Entity plane, asteroid, space, points; //, scene_camera, scene_light;
};

#endif
