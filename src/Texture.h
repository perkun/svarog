#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <iostream>
#include <stdio.h>
#include <string>
// #include <GL/glew.h>
#include <GL/gl.h>
#include "vendor/stb_image/stb_image.h"

using namespace std;

class Texture
{
public:
    Texture(string);
	~Texture();

	void destroy();

	void bind(unsigned int slot = 0);

private:
	unsigned int texture_id;
	string filepath;
	unsigned char *local_buffer;
	int width, height, bpp;

};

#endif /* TEXTURE_H_ */
