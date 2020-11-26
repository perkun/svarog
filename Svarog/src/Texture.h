#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <fitsio.h>
// #include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "vendor/stb_image/stb_image.h"

using namespace std;
using namespace glm;

struct TextureSpec
{
	int target = GL_TEXTURE_2D;
	int level = 0;
	int internal_format = GL_RGBA32F;
	int width = 1;
	int height = 1;
	int depth = 1;
	int border = 0;
	int format = GL_RGBA;
	int type = GL_UNSIGNED_BYTE;
};



class Texture
{
public:
    Texture(TextureSpec ts);
	~Texture();

	void destroy();
	unsigned int get_texture_id();

	void update(void *data, int xoffset = 0, int yoffset= 0);

	virtual void update(int xoffset = 0, int yoffset = 0);
	virtual void multiply_data(float factor);

	void bind(unsigned int slot = 0);
	vec2 get_dimentions();
	long get_size();
protected:
	Texture() {}

	unsigned int texture_id;
	string filepath;
	int width, height, bpp;

	TextureSpec specs;

};


class ImgTexture : public Texture
{
public:
	ImgTexture(string);
	~ImgTexture();

private:
	unsigned char *local_buffer;
};


class FitsTexture : public Texture
{
public:
	FitsTexture(string);
	~FitsTexture();

	void update(int xoffset = 0, int yoffset = 0) override;
	void multiply_data(float factor) override;
	void normalize_data();

	float *local_buffer;
	float *fits_data;

	float min, max;
private:
};



#endif /* TEXTURE_H_ */
