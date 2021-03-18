#ifndef RadarLayer_H_
#define RadarLayer_H_

#include <iostream>
#include <stdio.h>
#include "Layer.h"
#include "Scene.h"
#include "ObservationSeries.h"

using namespace std;


class RadarLayer : public Layer
{
public:
    RadarLayer(int dd_size);
	~RadarLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_update(double ts) override;

	void load_model(string filename);
	void make_radar_image(ImageSeries *radar_images, double jd);

	// 	virtual void on_imgui_render() override;
	// 	virtual void on_event(Event&) override;
	// 	void on_key_released_event(KeyReleasedEvent&);
	// 	void on_window_resize_event(WindowResizeEvent&);

	Entity model, observer;
private:
	Scene scene;
	Framebuffer *framebuffer;

	int dd_size = 200;
	int frame_width, frame_height;

    float *pixel_buffer_r;
    float *pixel_buffer_normal;
    float *pixel_buffer_depth;
};

#endif /* RadarLayer_H_ */
