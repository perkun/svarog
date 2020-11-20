#ifndef INPUT_H_
#define INPUT_H_

#include "Application.h"
#include "imgui.h"
#include <glm/glm.hpp>
#include <iostream>
#include <stdio.h>
#include "Application.h"

using namespace std;
using namespace glm;

namespace Input
{

inline bool is_imgui_window_hovered()
{
	return ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
}

inline bool is_key_pressed(int keykode)
{
    GLFWwindow *glfw_win = Application::get_window()->winptr;
    int state = glfwGetKey(glfw_win, keykode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

inline bool is_mouse_button_pressed(int button)
{
    GLFWwindow *glfw_win = Application::get_window()->winptr;
    int state = glfwGetMouseButton(glfw_win, button);
    return state == GLFW_PRESS;
}

inline vec2 get_mouse_position()
{
    GLFWwindow *glfw_win = Application::get_window()->winptr;
    double xpos, ypos;
    glfwGetCursorPos(glfw_win, &xpos, &ypos);

    return vec2(xpos, ypos);
}

// inline vec2 get_mouse_position_shift()
// {
//     return Application::get_cursor_shift();
// }
//
// inline float get_mouse_scroll_offset()
// {
// 	return Application::get_mouse_scroll_offset();
// }

} // namespace Input

#endif /* INPUT_H_ */
