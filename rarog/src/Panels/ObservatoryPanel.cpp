#include "svpch.h"
#include "MainLayer.h"
#include "ObservatoryPanel.h"
#include "Camera.h"

ObservatoryPanel::ObservatoryPanel(MainLayer *l, ObsStoragePack *obs)
{
    layer = l;
    obs_storage = obs;
}

ObservatoryPanel::~ObservatoryPanel()
{
}




void ObservatoryPanel::observations_panel()
{
    for (int i = 0; i < 5; i++)
        ImGui::Spacing();

    ImGui::Text("Storage:");
    ImGui::SameLine();
    ImGui::PushItemWidth(150.);
    if (ImGui::BeginCombo("##Storage", obs_storage->get_current_name().c_str()))
    {
        for (int i = 0; i < obs_storage->size(); i++)
        {
            const bool is_selected = (obs_storage->current_id == i);
            if (ImGui::Selectable(obs_storage->get_name(i).c_str(),
                                  is_selected))
            {
                obs_storage->detach_all_ghosts();
                obs_storage->current_id = i;
                layer->set_current_ghosts();
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    for (int i = 0; i < 5; i++)
        ImGui::Spacing();

    ImGuiTabBarFlags tab_bar_flags =
        ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll;

    if (ImGui::BeginTabBar("Observations", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Lightcurves"))
        {
            lightcurves_tab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("AO Images"))
        {
            ImGui::PushItemWidth(100.);
            ImGui::InputInt("AO size [px]", &ao_size, 1, 100);

            ImGui::SameLine(0.0, 20.0);
            ImGui::Text("bg color");
            ImGui::SameLine();
            ImGui::ColorEdit4("ao_bg_color", (float *)&ao_bg_color,
                              ImGuiColorEditFlags_NoInputs |
                                  ImGuiColorEditFlags_NoLabel);

            ImGui::SameLine(0.0, 20.0);
            ImGui::Checkbox("Earth tilt", &earth_tilt);

            ImGui::Spacing();

            ImageSeries *images =
                obs_storage->get_series<ImageSeries>("ao_images");

            if (ImGui::Button("Make AO image", ImVec2(150, 0)))
            {
                if (images == NULL)
                    images = obs_storage->add_series<ImageSeries>("ao_images");
                layer->make_ao_image(images, ao_size, earth_tilt, ao_bg_color);
            }

            for (int i = 0; i < 10; i++)
                ImGui::Spacing();

            if (images != NULL)
                display_images(images);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Radar Images"))
        {
            ImGui::PushItemWidth(100.);
            ImGui::InputInt("size [px]", &dd_size, 1, 100);
            ImGui::SameLine(0.0, 20.0);

            ImGui::InputFloat("ang. speed", &angular_speed);

            ImGui::Spacing();

            ImageSeries *images =
                obs_storage->get_series<ImageSeries>("radar_images");

            if (ImGui::Button("Make Radar image", ImVec2(150, 0)))
            {
                if (images == NULL)
                    images = obs_storage->add_series<ImageSeries>("radar_images");

                layer->make_radar_image(
                    images, dd_size, angular_speed);
            }

            for (int i = 0; i < 10; i++)
                ImGui::Spacing();

			if (images != NULL)
				display_images(images);

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}


void ObservatoryPanel::on_imgui_render()
{
    ImGui::Begin("Observatory");
    observations_panel();
    ImGui::End();
}



void ObservatoryPanel::lightcurves_tab()
{
    LightcurveSeries *lightcurves =
        obs_storage->get_series<LightcurveSeries>("lightcurves");


    if (ImGui::Button("Make lightcurve", ImVec2(150, 0)))
    {
        if (lightcurves == NULL)
            lightcurves =
                obs_storage->add_series<LightcurveSeries>("lightcurves");

        layer->make_lightcurve(lightcurves, lc_num_points);
    }

    ImGui::SameLine(0., 20.);
    ImGui::PushItemWidth(100.);
    ImGui::InputInt("LC num points", &lc_num_points);

    if (lc_num_points <= 0)
        lc_num_points = 1;

    for (int i = 0; i < 10; i++)
        ImGui::Spacing();

    if (lightcurves == NULL)
        return;

    ////////////////////////////////////////////////////

    Observation *current_obs = lightcurves->get_current_obs();
	if (current_obs == NULL)
		return;

    ImGui::Text("%lf", current_obs->julian_day);
    ImGui::SameLine(0., 20.);
    if (ImGui::Button("set time"))
        layer->time_panel.set_rotations(current_obs->julian_day);

    ImGui::PushItemWidth(100.);
    if (ImGui::InputInt("Lc Nr", &lightcurves->current_id, 1))
    {
        current_obs = lightcurves->get_current_obs();
        lightcurves->detach_all_ghosts();

        if (current_obs)
			layer->set_ghosts(current_obs);
    }

    ImGui::SameLine(0., 20.);
    if (ImGui::Button("set lc positions"))
        layer->set_target_and_observer(current_obs);


    if (ImGui::Button("Delete LC"))
    {
        lightcurves->delete_current_obs();
        current_obs = lightcurves->get_current_obs();

        if (current_obs)
			layer->set_ghosts(current_obs);

		return;
    }

    ImGuiCanvas canvas;

    canvas.prepare();


    float min = lightcurves->lcs_inv_mag_min,
          max = lightcurves->lcs_inv_mag_max;
    float margin = 0.1 * (max - min);
    min -= margin;
    max += margin;


    Lightcurve *lc = static_cast<Lightcurve *>(current_obs);
    float lc_size = lc->size();

    for (int i = 0; i < lc->size() - 1; i++)
        canvas.add_line((*lc)[i].phase, (*lc)[i].inv_mag,
                        (*lc)[i + 1].phase, (*lc)[i + 1].inv_mag,
					    0, 2*M_PI,
                        min, max, IM_COL32(255, 124, 14, 255));


    LightcurveSeries *obs_lightcurves =
        obs_storage->get_series<LightcurveSeries>("obs_lightcurves");

    if (obs_lightcurves != NULL)
    {
        Lightcurve *obs_lc = static_cast<Lightcurve *>(
            obs_lightcurves->get_obs(lightcurves->current_id));

        if (obs_lc != NULL)
        {
            for (int i = 0; i < obs_lc->size() - 1; i++)
                canvas.add_line(
                    (*obs_lc)[i].phase, (*obs_lc)[i].inv_mag,
                    (*obs_lc)[i + 1].phase , (*obs_lc)[i + 1].inv_mag,
					0, 2*M_PI,
                    min, max, IM_COL32(124, 124, 200, 255));
        }
    }

    canvas.draw();
}


void ObservatoryPanel::display_images(ImageSeries *images)
{
	if (images == NULL)
		return;

    if (images->size() == 0)
        return;

    Observation *current_obs = images->get_current_obs();
    if (!current_obs)
        return;


    ImGui::Text("%lf", current_obs->julian_day);

    ImGui::PushItemWidth(100.);
    if (ImGui::InputInt("Nr", &images->current_id, 1))
    {
        current_obs = images->get_current_obs();
        images->detach_all_ghosts();

        if (current_obs)
			layer->set_ghosts(current_obs);
    }

    ImGui::SameLine(0., 20.);
    if (ImGui::Button("set positions"))
        layer->set_target_and_observer(current_obs);

    long int tex_id =
        static_cast<Image *>(current_obs)->texture->get_texture_id();
    ImGui::Image((void *)tex_id, ImVec2(300, 300), ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::Button("Delete image"))
    {
        images->delete_current_obs();
        current_obs = images->get_current_obs();

        if (current_obs)
			layer->set_ghosts(current_obs);
    }

}








void ImGuiCanvas::prepare()
{
    static ImVec2 scrolling(0.0f, 0.0f);
    static bool opt_enable_grid = true;

    ImGui::Checkbox("Enable grid", &opt_enable_grid);


    // Using InvisibleButton() as a convenience 1) it will advance the
    // layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // ImDrawList API uses
                                                    // screen coordinates!
    canvas_sz = ImGui::GetContentRegionAvail();

    if (canvas_sz.x < 50.0f)
        canvas_sz.x = 50.0f;
    canvas_sz.y = 3 / 4.0 * canvas_sz.x;

    ImVec2 canvas_p1 =
        ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    // Draw border and background color
    ImGuiIO &io = ImGui::GetIO();

    draw_list = ImGui::GetWindowDrawList();

    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    origin = ImVec2(canvas_p0.x + scrolling.x,
                    canvas_p0.y + scrolling.y); // Lock scrolled origin
                                                //
    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid)
    {
        const float GRID_STEP = 64.0f;
        for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x;
             x += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y),
                               ImVec2(canvas_p0.x + x, canvas_p1.y),
                               IM_COL32(200, 200, 200, 40));
        for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y;
             y += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y),
                               ImVec2(canvas_p1.x, canvas_p0.y + y),
                               IM_COL32(200, 200, 200, 40));
    }
}


void ImGuiCanvas::add_line(double x1, double y1, double x2, double y2,
						   double x_min, double x_max,
						   double y_min, double y_max, ImU32 color)
{
    double y_range = y_max - y_min;
    double x_range = x_max - x_min;

    draw_list->AddLine(
        ImVec2(origin.x + (x1 - x_min)/x_range * canvas_sz.x,
               origin.y + canvas_sz.y - ((y1 - y_min) * canvas_sz.y / y_range)),
        ImVec2(origin.x + (x2 - x_min)/x_range * canvas_sz.x,
               origin.y + canvas_sz.y - ((y2 - y_min) * canvas_sz.y / y_range)),
        color, 2.0f);
}


void ImGuiCanvas::draw()
{
    draw_list->PopClipRect();
}
