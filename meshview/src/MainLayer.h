#ifndef MAINLAYER_H_
#define MAINLAYER_H_

#include "Parser.h"
#include "Layer.h"
#include "Scene.h"
#include "Utils/FileDialog.h"
#include "Overlay.h"



class MainLayer : public Layer
{
public:
    MainLayer(Args args);
    ~MainLayer();

    virtual void on_attach() override;
    virtual void on_detach() override;
    virtual void on_update(double ts) override;
    virtual void on_imgui_render() override;
    virtual void on_event(Event &) override;

private:
    void on_key_released_event(KeyReleasedEvent &event);
    void on_key_pressed_event(KeyPressedEvent &event);
    void on_mouse_moved_event(MouseMovedEvent &event);

	void make_projection_and_exit();
    void print_screen(std::string filename);

    Overlay header_overlay, help_overlay, info_overlay;

    Args args;
    Scene scene;

    Entity axes, model;

    Framebuffer *ms_framebuffer, *framebuffer;

    bool display_header_info = true;
    bool display_axes = false;
    bool display_help = false;

    vector<shared_ptr<Shader>> shader_ptrs;


    vec3 init_model_pos;
    vec2 viewport_panel_size;

    vec2 last_cursor_pos;

    // defaults:
    float cam_size_x = 2.05;
    vector<vec4> bg_colors;
    int currnet_bg_color = 0;
};


#endif
