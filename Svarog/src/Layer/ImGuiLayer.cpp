#include "ImGuiLayer.h"
#include "Application.h"

ImGuiLayer::ImGuiLayer()
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::on_attach()
{
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable
    // Gamepad Controls
//     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
//     io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport
                                                        // / Platform Windows
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;


//     io.Fonts->AddFontFromFileTTF("../data/DroidSans.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/TTF/Inconsolata-Medium.ttf", 18.0f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF(
//         "../data/DroidSans.ttf", 18.0f);
        "/usr/share/fonts/TTF/Inconsolata-Medium.ttf", 18.0f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0;

//     if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//     {
//         style.WindowRounding = 0.0f;
//         style.Colors[ImGuiCol_WindowBg].w = 1.0f;
//     }

//     SetDarkThemeColors();

    Window *window = Application::get_window();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window->winptr, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiLayer::on_detach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

void ImGuiLayer::on_event(Event &event)
{
}



void ImGuiLayer::begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


}


void ImGuiLayer::end()
{
	ImGuiIO& io = ImGui::GetIO();
    Window *window = Application::get_window();
	io.DisplaySize = ImVec2((float)window->width, (float)window->height);

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

// 	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
// 	{
// 		GLFWwindow* backup_current_context = glfwGetCurrentContext();
// 		ImGui::UpdatePlatformWindows();
// 		ImGui::RenderPlatformWindowsDefault();
// 		glfwMakeContextCurrent(backup_current_context);
// 	}

}
