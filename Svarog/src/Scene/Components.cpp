#include "svpch.h"
#include "Components.h"


MeshComponent::MeshComponent(shared_ptr<VertexArrayObject> vao)
{
	this->vao = vao;
}


MeshComponent::~MeshComponent()
{
}


TextureComponent::TextureComponent(Texture *tex)
{
	this->texture = tex;
}

TextureComponent::~TextureComponent()
{
}


ShaderComponent::ShaderComponent(Shader *s)
{
	this->shader = s;
}

ShaderComponent::~ShaderComponent()
{}


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

CameraComponent::CameraComponent(Camera *c)
{
	camera = c;
}

CameraComponent::~CameraComponent()
{
}

FramebufferComponent::FramebufferComponent(Framebuffer *fb)
{
	framebuffer = fb;
}

FramebufferComponent::~FramebufferComponent()
{
}
