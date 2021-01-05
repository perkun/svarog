#include "svpch.h"
#include "Light.h"



Light::Light(shared_ptr<Framebuffer>  fb, shared_ptr<Camera>  cam)
{
	shadow_framebuffer = fb;
	camera = cam;
}


Light::~Light()
{
}
