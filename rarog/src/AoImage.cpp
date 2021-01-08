#include "svpch.h"
#include "AoImage.h"


AoImage::AoImage(int width, int height)
{
	TextureSpec specs;

	specs.target = GL_TEXTURE_2D;
	specs.level = 0;
	specs.internal_format = GL_RGB32F;
	specs.width = width;
	specs.height = height;
	specs.depth = 1;
	specs.border = 0;
	specs.format = GL_RGB;
	specs.type = GL_FLOAT;

	texture = make_shared<Texture>(specs);
}


AoImage::~AoImage()
{
}
