#include "MainLayer.h"


MainLayer::MainLayer()
{
}


MainLayer::~MainLayer()
{
}


void MainLayer::on_attach()
{
    Window *window = Application::get_window();
	scene->observer = scene->create_entity("Observer");
	scene->light = scene->create_entity("Light");

	frame.load_fits("../../../data/fits3.fits", TFLOAT);

	TextureSpec tspec;
	tspec.target = GL_TEXTURE_2D;
	tspec.internal_format = GL_RGB32F;
	tspec.width = frame.width;
	tspec.height = frame.height;
	tspec.format = GL_RGB;
	tspec.type = GL_FLOAT;

	frame_tex = new Texture(tspec);
	frame_tex->update<float>(frame.data, frame.data, frame.data);

	frame_max = frame.get_max();
	frame_min = frame.get_min();
}

void MainLayer::on_update(double time_delta)
{
}

void MainLayer::on_imgui_render()
{
	ImGui::Begin("Fits");

	long int tex_id = frame_tex->get_texture_id();
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImGui::Image((void *)tex_id, ImVec2(frame.width, frame.height), ImVec2(0, 1), ImVec2(1, 0), tint_col);

	float step = (frame.get_max() - frame.get_min())/100.;
	if (ImGui::DragFloat("max", &frame_max, step, frame.get_min(), 10000.))
	{
		frame.set_max(frame_max);
		frame_tex->update<float>(frame.data, frame.data, frame.data);
	}

	if (ImGui::DragFloat("min", &frame_min, step, 0., frame.get_max()))
	{
		frame.set_min(frame_min);
		frame_tex->update<float>(frame.data, frame.data, frame.data);
	}


	ImGui::End();
}

void MainLayer::on_detach()
{
	delete frame_tex;
}

