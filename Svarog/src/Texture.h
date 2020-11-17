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




class Texture
{
public:
    Texture();
	~Texture();

	void destroy();
	unsigned int get_texture_id();

	void bind(unsigned int slot = 0);
	vec2 get_dimentions();

protected:
	unsigned int texture_id;
	string filepath;
	int width, height, bpp;
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

private:
// 	short *local_buffer;
	float *local_buffer;
};



#endif /* TEXTURE_H_ */
