#include "svpch.h"
#include "Components.h"


MeshComponent::MeshComponent(shared_ptr<VertexArrayObject> vao)
{
	this->vao = vao;
}


MeshComponent::~MeshComponent()
{
}


TextureComponent::TextureComponent(shared_ptr<Texture> tex)
{
	this->texture = tex;
}

TextureComponent::~TextureComponent()
{
}


SceneStatus::SceneStatus() : active(false)
{
}

SceneStatus::SceneStatus(bool active, bool render)
    : active(active), render(render)
{
}

SceneStatus::~SceneStatus()
{
}

CameraComponent::CameraComponent(shared_ptr<Camera> c)
{
	camera = c;
}

CameraComponent::~CameraComponent()
{
}

FramebufferComponent::FramebufferComponent(shared_ptr<Framebuffer> fb)
{
	framebuffer = fb;
}

FramebufferComponent::~FramebufferComponent()
{
}

