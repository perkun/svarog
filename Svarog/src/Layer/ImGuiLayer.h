#ifndef IMGUILAYER_H_
#define IMGUILAYER_H_

#include "Layer.h"

using namespace std;

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
	~ImGuiLayer();

	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_event(Event&) override;
	void begin();
	void end();

	void set_dark_theme_colors();

};

#endif /* IMGUILAYER_H_ */
