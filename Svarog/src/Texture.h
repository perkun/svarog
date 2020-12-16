#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "stb_image.h"

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
    Texture(TextureSpec &ts);
	~Texture();

	void destroy();
	unsigned int get_texture_id();

	void update(void *data, int xoffset = 0, int yoffset= 0);

	template <typename T>
	void update(T *data_r, T *data_g, T *data_b, int xoffset = 0, int yoffset = 0);

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

template <typename T>
void Texture::update(T *data_r, T *data_g, T *data_b, int xoffset, int yoffset)
{
	T data[specs.width * specs.height * 3];

	for (int i = 0; i < specs.width * specs.height; i++)
	{
		data[i * 3 + 0] = data_r[i];
		data[i * 3 + 1] = data_g[i];
		data[i * 3 + 2] = data_b[i];
	}

    bind();
	if (specs.target == GL_TEXTURE_2D)
		glTexSubImage2D(specs.target, specs.level, xoffset, yoffset, specs.width,
						specs.height, specs.format, specs.type, &data[0]);
// 	else if (specs.target == GL_TEXTURE_3D)
// 		glTexImage3D(specs.target, specs.level, specs.internal_format, specs.width,
// 				specs.height, specs.depth, specs.border, specs.format, specs.type, &data[0]);

}




class ImgTexture : public Texture
{
public:
	ImgTexture(string);
	~ImgTexture();

private:
	unsigned char *local_buffer;
};


// class FitsTexture : public Texture
// {
// public:
// 	FitsTexture(string);
// 	~FitsTexture();
//
// 	void update(int xoffset = 0, int yoffset = 0) override;
// 	void multiply_data(float factor) override;
// 	void normalize_data();
//
// 	float *local_buffer;
// 	float *fits_data;
//
// 	float min, max;
// private:
// };



#endif /* TEXTURE_H_ */
