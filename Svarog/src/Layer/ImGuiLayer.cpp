#include "ImGuiLayer.h"
#include "Application.h"
#include "ImGuizmo.h"
#include "svpch.h"

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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    //     io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable
    //     Multi-Viewport
    // / Platform Windows
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    //     io.Fonts->AddFontFromFileTTF("../data/DroidSans.ttf", 18.0f);
    //
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX =
        18.0f; // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = {0xe000, 0xfd47, 0};



#ifdef SV_CONFIG_FONT_PATH
    char bold_font_path[] = SV_CONFIG_FONT_PATH "/Inconsolata-Bold.ttf";
    char main_font_path[] =
        SV_CONFIG_FONT_PATH "/InconsolataGo Nerd Font Complete.ttf";
#else
    char bold_font_path[] = "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf";
    char main_font_path[] = "/usr/share/fonts/TTF/DejaVuSans.ttf";

//     char bold_font_path[] = "/usr/share/fonts/TTF/Inconsolata-Bold.ttf";
//     char main_font_path[] = "/usr/share/fonts/TTF/InconsolataGo Nerd Font Complete.ttf";
#endif

    io.Fonts->AddFontFromFileTTF(main_font_path, 18.0f);
    io.Fonts->AddFontFromFileTTF(main_font_path, 18.0f,
                                                  &config, icon_ranges);
    io.Fonts->AddFontFromFileTTF(bold_font_path, 18.0f);

//     io.FontDefault = io.Fonts->AddFontFromFileTTF(main_font_path, 18.0f,
//                                                   &config, icon_ranges);

// 	io.Fonts->Build();



    // 		"/usr/share/fonts/TTF/Inconsolata-Medium.ttf", 18.0f);
    //     io.Fonts->AddFontFromFileTTF("/usr/share/fonts/nerd-fonts-complete/TTF/InconsolataGo
    //     Nerd Font Complete.ttf", 18.0f); io.FontDefault =
    //     io.Fonts->AddFontFromFileTTF(
    // 		"/usr/share/fonts/nerd-fonts-complete/TTF/InconsolataGo Nerd
    // Font Complete.ttf", 18.0f);

    //         "../data/DroidSans.ttf", 18.0f);

    // COLORS
    set_dark_theme_colors();
    // Setup Dear ImGui style
    //     ImGui::StyleColorsDark();
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

    ImGuizmo::BeginFrame();
}

void ImGuiLayer::end()
{
    ImGuiIO &io = ImGui::GetIO();
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

void ImGuiLayer::set_dark_theme_colors()
{
    //     ImGui::StyleColorsDark();
    auto &colors = ImGui::GetStyle().Colors;
    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.5f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.5f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Window
    colors[ImGuiCol_WindowBg] = ImVec4{0.16f, 0.18f, 0.188f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}
