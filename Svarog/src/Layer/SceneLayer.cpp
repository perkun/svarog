#include "SceneLayer.h"

SceneLayer::SceneLayer() : Layer("SceneLayer")
{
    // FRAMEBUFFER
    FramebufferSpecification fb_spec;
    fb_spec.width = Application::get_window()->width;
    fb_spec.height = Application::get_window()->height;

    framefuffer = new Framebuffer(fb_spec);
    scene = new Scene();
}

SceneLayer::~SceneLayer()
{
    delete scene;
    delete framefuffer;
}

void SceneLayer::on_attach()
{
}

void SceneLayer::on_detach()
{
}

void SceneLayer::on_update(double ts)
{
}

void SceneLayer::on_imgui_render()
{
}

void SceneLayer::on_event(Event &event)
{
    EventDispacher dispatcher(event);

    dispatcher.dispatch<WindowResizeEvent>(
        bind(&SceneLayer::on_window_resize_event, this, placeholders::_1));

//     if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
//     {
//         dispatcher.dispatch<MouseScrolledEvent>(
//             bind(&SceneLayer::on_mouse_scrolled_event, this, placeholders::_1));
//     }

	// update all the controllers
	scene->registry.view<NativeScriptComponent>().each([&event](auto entity, auto&nsc)
	{
		if (!nsc.instance)
			return;

		nsc.instance->on_event(event);
	});

}


void SceneLayer::on_window_resize_event(WindowResizeEvent &event)
{
    ivec2 size = event.get_size();
    scene->on_resize(size.x, size.y);
	framefuffer->resize(size.x, size.y);
}

