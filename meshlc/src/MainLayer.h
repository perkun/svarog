#ifndef MAINLAYER_H_
#define MAINLAYER_H_

#include "Parser.h"
#include "Layer.h"
#include "Scene.h"
#include "Utils/FileDialog.h"
#include "ObservationSeries.h"
// #include "ObservationStorage.h"



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

    void print_screen();

    Args args;
    Scene scene;

    Framebuffer *ms_framebuffer, *framebuffer;


    bool display_header_info = true;
    bool display_axes = false;
    bool display_help = false;

//     vector<shared_ptr<Shader>> shader_ptrs;


    vec3 init_model_pos;
    vec2 viewport_panel_size;

	float position_mult = 100.f;


	int lc_num_points = 90;
	LightcurveSeries *lightcurves;

    // defaults:
    float cam_size_x = 2.05;
    vector<vec4> bg_colors;
    int currnet_bg_color = 0;


	void make_lightcurve(LightcurveSeries *lightcurves, int num_points);

};


#endif
