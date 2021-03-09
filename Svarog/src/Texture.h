#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "stb_image.h"
#include "stb_image_write.h"

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


class FitsHeaderEntry
{
public:
	FitsHeaderEntry() {}
	~FitsHeaderEntry() {}

	string key, comment;
	virtual void write(fitsfile *file) {}
};

class FitsHeaderEntryFloat : public FitsHeaderEntry
{
public:
	FitsHeaderEntryFloat(string k, float v, string c)
	{
		key = k;
		value = v;
		comment = c;
	}

	float value;
	virtual void write(fitsfile *file) override {
		int status = 0;
		fits_update_key_fixflt(file, key.c_str(),
				value, 6, comment.c_str(), &status);
	}
};


class FitsHeaderEntryDouble : public FitsHeaderEntry
{
public:
	FitsHeaderEntryDouble(string k, double v, string c)
	{
		key = k;
		value = v;
		comment = c;
	}

	double value;

	virtual void write(fitsfile *file) override {
		int status = 0;
		fits_update_key_fixdbl(file, key.c_str(),
				value, 15, comment.c_str(), &status);
	}
};

class FitsHeader
{
public:
	void push(string key, float value, string comment)
	{
		auto hef = make_shared<FitsHeaderEntryFloat>(key, value, comment);
		entries.emplace_back(hef);
	}

	void push(string key, double value, string comment)
	{
		auto hef = make_shared<FitsHeaderEntryDouble>(key, value, comment);
		entries.emplace_back(hef);
	}

        void write(fitsfile *file)
	{
		for (auto he : entries)
			he->write(file);
	}

private:
	vector< shared_ptr<FitsHeaderEntry> > entries;
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

	void save_png(const char *filename);
	void save_fits_greyscale(const char *filename, FitsHeader& header);


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
	T *data = new T[specs.width * specs.height * 3];

	for (int i = 0; i < specs.width * specs.height; i++)
	{
		data[i * 3 + 0] = data_r[i];
		data[i * 3 + 1] = data_g[i];
		data[i * 3 + 2] = data_b[i];
	}

    bind();
	if (specs.target == GL_TEXTURE_2D)
		glTexSubImage2D(specs.target, specs.level, xoffset, yoffset, specs.width,
						specs.height, specs.format, specs.type, data);
// 	else if (specs.target == GL_TEXTURE_3D)
// 		glTexImage3D(specs.target, specs.level, specs.internal_format, specs.width,
// 				specs.height, specs.depth, specs.border, specs.format, specs.type, &data[0]);

	delete[] data;

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
