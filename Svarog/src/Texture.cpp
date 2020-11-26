#include "Texture.h"

Texture::Texture(TextureSpec ts) : specs(ts)
{
    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0); // uwaga na numerki!
    glBindTexture(specs.target, texture_id);

    // those 4 need to be specified!
    glTexParameteri(specs.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(specs.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(specs.target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(specs.target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (specs.target == GL_TEXTURE_3D)
		glTexParameteri(specs.target, GL_TEXTURE_WRAP_R, GL_REPEAT);
    // 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // send to openGL
	if (specs.target == GL_TEXTURE_2D)
		glTexImage2D(specs.target, specs.level, specs.internal_format, specs.width,
				specs.height, specs.border, specs.format, specs.type, NULL);
	else if (specs.target == GL_TEXTURE_3D)
		glTexImage3D(specs.target, specs.level, specs.internal_format, specs.width,
				specs.height, specs.depth, specs.border, specs.format, specs.type, NULL);

    // unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	destroy();
}

void Texture::destroy()
{
	glDeleteTextures(1, &texture_id);

}

void Texture::update(int xoffset, int yoffset)
{
}

void Texture::update(void *data, int xoffset, int yoffset)
{
    bind();
	if (specs.target == GL_TEXTURE_2D)
		glTexSubImage2D(specs.target, specs.level, xoffset, yoffset, specs.width,
						specs.height, specs.format, specs.type, data);
	else if (specs.target == GL_TEXTURE_3D)
		glTexImage3D(specs.target, specs.level, specs.internal_format, specs.width,
				specs.height, specs.depth, specs.border, specs.format, specs.type, data);
}

void Texture::bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(specs.target, texture_id);
}

unsigned int Texture::get_texture_id()
{
	return texture_id;
}


vec2 Texture::get_dimentions()
{
	return vec2(width, height);
}

long Texture::get_size()
{
	return width * height;
}


void Texture::multiply_data(float factor)
{
}




ImgTexture::ImgTexture(string path)
{
	texture_id = 0;
	filepath = path;
	local_buffer = NULL;
	width = 0;
	height = 0;
	bpp = 0;

	// in openGL bottom left is 0, 0. In PNG 0, 0 is top left, so we need to
	// flip the image on load
	stbi_set_flip_vertically_on_load(1);
	local_buffer = stbi_load(filepath.c_str(), &width, &height, &bpp, 4);

	if (local_buffer == NULL)
		cout << "Problem loading texture " << filepath << endl;
// 	cout << width << " " << height << " " << bpp << endl;

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, local_buffer);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	if (local_buffer)
		stbi_image_free(local_buffer);
}

ImgTexture::~ImgTexture()
{
	destroy();
}



FitsTexture::FitsTexture(string path)
{
	texture_id = 0;
	filepath = path;
	local_buffer = NULL;
	width = 0;
	height = 0;
	bpp = 0;

	fitsfile *fptr;
	// bitpix (32 - int, 64 - long int, -32 - float, -64 - double)
	int status = 0, bitpix, num_axis, anynul;
	long axis_dim[10], fpixel[2]={1,1};

	fits_open_file(&fptr, path.c_str(), 0, &status);
	if (status != 0)
	{
		char err_text[30];
		fits_get_errstatus(status, err_text);
		cout << "cannot open FITS file, status: "<< err_text << "\n";
		return;
	}

	// image info
	fits_get_img_param(fptr, 2, &bitpix, &num_axis, axis_dim, &status);

	width = axis_dim[0];
	height = axis_dim[1];

	size_t data_size = 1;
	for (int i = 0; i < num_axis && i < 10; i++)
		data_size *= axis_dim[i];

	local_buffer = (float*)malloc(sizeof(float) * data_size * 4);


	cout << data_size << "\t"<<axis_dim[0]<<"\t"<<axis_dim[1] <<endl;
	cout << bitpix << endl;


	fits_data = (float*)malloc(sizeof(float) * data_size);

	fits_read_pix(fptr, TFLOAT, fpixel, data_size, NULL, fits_data,
			&anynul, &status);
	fits_close_file(fptr, &status);

	// normalize texture
	max = -1E100;
	for (int i = 0; i < data_size; i++)
		if (fits_data[i] > max)
			max = fits_data[i];

	min = 1E100;
	for (int i = 0; i < data_size; i++)
		if (fits_data[i] < min)
			min = fits_data[i];

	normalize_data();



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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
				 GL_FLOAT, NULL);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	update();
}


FitsTexture::~FitsTexture()
{
	delete local_buffer;
	delete fits_data;
	destroy();
}


void FitsTexture::update(int xoffset, int yoffset)
{

    bind(0);
    // 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
    // 				 GL_UNSIGNED_BYTE, local_buffer);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT,
                    local_buffer);
}

void FitsTexture::multiply_data(float factor)
{
	for (int i = 0; i < width * height; i++)
	{
		fits_data[i] *= factor;
	}

}

void FitsTexture::normalize_data()
{
// 	for (int i = 0; i < width * height; i++)
// 	{
// 		fits_data[i] = (fits_data[i] - min) / (max - min);
// 	}
	for (int i = 0; i < width * height; i++)
	{
		local_buffer[i * 4 + 0] = (fits_data[i] - min) / (max - min);
		local_buffer[i * 4 + 1] = (fits_data[i] - min) / (max - min);
		local_buffer[i * 4 + 2] = (fits_data[i] - min) / (max - min);
		local_buffer[i * 4 + 3] = 1.;
	}
}
