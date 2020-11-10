#include "Components.h"


MeshComponent::MeshComponent(VertexArrayObject *vao)
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
{}


ShaderComponent::ShaderComponent(Shader *s)
{
	this->shader = s;
}

ShaderComponent::~ShaderComponent()
{}

