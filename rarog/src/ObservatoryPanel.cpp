#include "svpch.h"
#include "ObservatoryPanel.h"


ObservatoryPanel::ObservatoryPanel(MainLayer *l) : layer(l)
{
}


ObservatoryPanel::~ObservatoryPanel()
{
}


void ObservatoryPanel::on_imgui_render()
{
	ImGui::Begin("Observatory");

	observe_button();
	ImGui::Separator();
	// observations buttons
	if (ImGui::Button("Make lightcurve"))
		make_lightcurve(layer->model);

	ImGui::Separator();
	display_lightcurves();

	ImGui::End();
}


void ObservatoryPanel::observe_button()
{
	string run_btn_label;
	if (layer->mode == Mode::EDITOR)
		run_btn_label = "Observe";
	else if (layer->mode == Mode::RUNTIME)
		run_btn_label = "Stop observing";

	if (ImGui::Button(run_btn_label.c_str()))
	{
		layer->toggle_mode();
	}



}

void ObservatoryPanel::display_lightcurves()
{
    if (lightcurves.size() > 0)
    {
        ImGui::InputInt("Lightcurve Nr", &lc_id, 1);

        if (lc_id < 0)
            lc_id = 0;
        if (lc_id >= lightcurves.size())
            lc_id = lightcurves.size() - 1;

		float min = lightcurves[lc_id].min;
		float max = lightcurves[lc_id].max;

        ImGui::PlotLines("LC", lightcurves[lc_id].data(),
                         lightcurves[lc_id].size(), 0, NULL, min, max,
                         ImVec2(200.0f, 130.0f));

		if (ImGui::Button("Save magnitudes"))
		{
			lightcurves[lc_id].save_mag(FileDialog::save_file("*").c_str());
		}
		ImGui::SameLine();
		if (ImGui::Button("Save flux"))
		{
			lightcurves[lc_id].save_flux(FileDialog::save_file("*").c_str());
		}
    }
}

void ObservatoryPanel::make_lightcurve(Entity &target)
{
	int num_points = 360;
	int width = 256;
	int height = 256;
	float *pixel_buffer = new float[width * height];
	Lightcurve lc(num_points);
	vec4 bg_color = Application::get_bg_color();

	layer->runtime_observer.get_component<CameraComponent>().camera->update_target(
			target.get_component<Transform>().position);

	layer->set_runtime_mode();
	Application::set_bg_color(vec4(0., 0., 0., 1.));
	layer->scene.on_resize(width, height);
	layer->ms_framebuffer->resize(width, height);
	layer->framebuffer->resize(width, height);

	Transform &t = target.get_component<Transform>();

	for (int i = 0; i < num_points; i++)
	{
		layer->on_update(2*M_PI / num_points / t.rotation_speed);

		layer->framebuffer->bind();
		glReadPixels(0, 0, width, height, GL_RED, GL_FLOAT, pixel_buffer);

		double flux = 0.0;
		for (int j = 0; j < width * height; j++)
			flux += pixel_buffer[j];
		lc.push_value(flux);
	}

	lc.calculate_min();
	lc.calculate_max();
	lightcurves.push_back(lc);

	lc_id = lightcurves.size() - 1;

	layer->scene.on_resize(layer->viewport_panel_size.x, layer->viewport_panel_size.y);
	layer->ms_framebuffer->resize(layer->viewport_panel_size.x, layer->viewport_panel_size.y);
	layer->framebuffer->resize(layer->viewport_panel_size.x, layer->viewport_panel_size.y);

	Application::set_bg_color(bg_color);
	layer->set_editor_mode();
	delete[] pixel_buffer;
}
