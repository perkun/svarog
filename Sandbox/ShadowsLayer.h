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

using namespace std;

struct FloatData2D {
	float *data;
	int width, height;

	FloatData2D(int w, int h) {
		width = w; height= h;
		data = new float[w*h];
	}
	~FloatData2D() { delete data; }

	void perlin_noise(float size_factor)
	{
		PerlinNoise pn(1);
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				double x = (double)i / ((double)width);
				double y = (double)j / ((double)height);
				double z = 0.5;

				x -= 0.5;
				x = x * cos( (y-0.5) * M_PI );
				x += 0.5;

				x *= size_factor * 2.;
				y *= size_factor;
				z *= size_factor;

				double noise = pn.noise(x, y, z);
				data[j * width + i] = noise;
			}
	}
};

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
	FloatData2D *perlin_data;
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
	Shader *shader, *color_shader, *basic_shader, *displacement_shader;
	Entity plane, asteroid, space, points; //, scene_camera, scene_light;
};

#endif
