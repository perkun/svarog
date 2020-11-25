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
    Texture(int w, int h);
	~Texture();

	void destroy();
	unsigned int get_texture_id();

	void update();

	virtual void update_texture();
	virtual void multiply_data(float factor);

	void bind(unsigned int slot = 0);
	vec2 get_dimentions();
	long get_size();

protected:
	Texture() {}

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

	void update_texture() override;
	void multiply_data(float factor) override;
	void normalize_data();

	float *local_buffer;
	float *fits_data;

	float min, max;
private:
};



#endif /* TEXTURE_H_ */
