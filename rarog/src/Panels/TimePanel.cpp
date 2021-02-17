#include "svpch.h"
#include "TimePanel.h"


TimePanel::TimePanel(Scene *s) : scene(s)
{
	julian_day = Time::julian_day_now();
	date = Time::jd_to_date(Time::julian_day_now());

}


TimePanel::~TimePanel()
{
}


void TimePanel::on_imgui_render()
{
	ImGui::Begin("Time");

	draw_time_selection();

	ImGui::End();
}

void TimePanel::draw_time_selection()
{

    if (ImGui::Button("Reset"))
		set_rotations(julian_day);

    // set JD
    ImGui::SetNextItemWidth(150);
    if (ImGui::InputDouble("JD", &julian_day))
		set_rotations(julian_day);

    ImGui::SetNextItemWidth(150);
    if (ImGui::InputInt("Year", &date.year))
		set_rotations(date);

    ImGui::SetNextItemWidth(150);
    if (ImGui::InputInt("Month", &date.month))
		set_rotations(date);

    ImGui::SetNextItemWidth(150);
    if (ImGui::InputInt("Day", &date.day))
		set_rotations(date);

    ImGui::SetNextItemWidth(150);
    if (ImGui::InputInt("Hour", &date.hour))
		set_rotations(date);

    ImGui::SetNextItemWidth(150);
    if (ImGui::InputInt("Min.", &date.minute))
		set_rotations(date);

    ImGui::SetNextItemWidth(150);
    if (ImGui::InputFloat("Sec.", &date.second))
		set_rotations(date);
}


void TimePanel::set_rotations(double julian_day)
{
	this->julian_day = julian_day;
	this->date = Time::jd_to_date(julian_day);

	scene->registry.each([&](auto entityID)
	{
		Entity entity(entityID, &scene->registry);
		if (entity.has_component<OrbitalComponent>())
		{
			OrbitalComponent &oc = entity.get_component<OrbitalComponent>();
			oc.calculate_rot_phase(julian_day);
			entity.get_component<Transform>().rotation = oc.xyz_from_lbg();
		}

	});
}


void TimePanel::set_rotations(Time::CalendarDate date)
{
	date.normalize();
	julian_day = Time::julian_day(date);
	this->date = date;

	set_rotations(julian_day);
}
