#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
	destroy();
}

void Texture::destroy()
{
	glDeleteTextures(1, &texture_id);
}

void Texture::bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

unsigned int Texture::get_texture_id()
{
	return texture_id;
}


vec2 Texture::get_dimentions()
{
	return vec2(width, height);
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


	float *fits_data;
	fits_data = (float*)malloc(sizeof(float) * data_size);

	fits_read_pix(fptr, TFLOAT, fpixel, data_size, NULL, fits_data,
			&anynul, &status);
	fits_close_file(fptr, &status);

	// normalize texture
	short max = fits_data[0];
	for (int i = 0; i < data_size; i++)
	{
		if (fits_data[i] > max)
			max = fits_data[i];
	}

	for (int i = 0; i < data_size; i++)
	{
		local_buffer[i * 4 + 0] = fits_data[i]/max;
		local_buffer[i * 4 + 1] = fits_data[i]/max;
		local_buffer[i * 4 + 2] = fits_data[i]/max;
		local_buffer[i * 4 + 3] = 1.;
	}

	delete fits_data;


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
				 GL_FLOAT, local_buffer);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}


FitsTexture::~FitsTexture()
{
	delete local_buffer;
	destroy();
}
