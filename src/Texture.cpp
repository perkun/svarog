#include "Texture.h"

Texture::Texture(string path)
    : texture_id(0), filepath(path), local_buffer(NULL), width(0), height(0), bpp(0)
{
	// in openGL bottom left is 0, 0. In PNG 0, 0 is top left, so we need to
	// flip the image on load
	stbi_set_flip_vertically_on_load(1);
	local_buffer = stbi_load(filepath.c_str(), &width, &height, &bpp, 4);

	if (local_buffer == NULL)
		cout << "Problem loading texture " << filepath << endl;
	cout << width << " " << height << " " << bpp << endl;

	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0);			// uwaga na numerki!
	glBindTexture(GL_TEXTURE_2D, texture_id);

	// those 4 need to be specified!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// send to openGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, local_buffer);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	if (local_buffer)
		stbi_image_free(local_buffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture_id);
}

void Texture::bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture_id);
}
