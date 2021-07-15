#include "svpch.h"
#include "Overlay.h"


Overlay::Overlay(int corner, float alpha, const char *imgui_name)
    : corner(corner), alpha(alpha)
{
	timeout = default_timeout;
	sprintf(this->imgui_name, "%s", imgui_name);
}


Overlay::~Overlay()
{
}


void Overlay::display()
{
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
		ImGuiWindowFlags_NoMove;

	float DISTANCE = 10.f;


	if (corner != -1) {
		window_flags |= ImGuiWindowFlags_NoMove;
		ImGuiViewport *viewport = ImGui::GetMainViewport();
		ImVec2 work_area_pos =
			viewport->GetWorkPos(); // Instead of using viewport->Pos we use
		// GetWorkPos() to avoid menu bars, if any!
		ImVec2 work_area_size = viewport->GetWorkSize();
		ImVec2 window_pos =
			ImVec2((corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE)
					: (work_area_pos.x + DISTANCE),
					(corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE)
					: (work_area_pos.y + DISTANCE));
		ImVec2 window_pos_pivot =
			ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowViewport(viewport->ID);
	}

	ImGui::SetNextWindowBgAlpha(alpha); // Transparent background
	bool p_open;
	if (ImGui::Begin(imgui_name, &p_open, window_flags))
	{
		ImGui::Text("%s", text.c_str());
	}
	ImGui::End();
}


void Overlay::display(string new_text)
{
	text = new_text;
	display();
}


void Overlay::display_timeout()
{
	if (timeout > 0.)
		display();
}


void Overlay::display_timeout(string new_text)
{
	text = new_text;
	display_timeout();
}


void Overlay::reset(string new_text)
{
	text = new_text;
}


void Overlay::reset_timeout(string new_text)
{
	text = new_text;
	reset_timeout();
}


void Overlay::reset_timeout()
{
	timeout = default_timeout;
}
