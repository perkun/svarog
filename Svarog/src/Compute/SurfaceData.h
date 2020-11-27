#ifndef SURFACEDATA_H_
#define SURFACEDATA_H_

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fitsio.h>
#include "PerlinNoise.h"

using namespace std;

template <typename T>
class SurfaceData
{
public:
	SurfaceData() {}
	SurfaceData(int w, int h)
	{
		width = w; height= h;
		data = new T[w*h];
		data_created = true;
	}

	~SurfaceData()
	{
		if (data_created)
		{
			delete data;
			delete orig_data;
		}
	}

	void fill_with_perlin_noise(double size_factor, T max_val, bool cylindrical_projection = false, int seed = 0);
	void load_fits(string filename, int f_type = TFLOAT);

	void set_max(T new_max);
	void set_min(T new_min);

	void normalize_data();

	T get_min() { return min; }
	T get_max() { return max; }

	T *data;
	int width, height;
private:
	T *orig_data;
	bool data_created = false;
	T min, max;
};


template <typename T>
void SurfaceData<T>::load_fits(string filename, int f_type)
{
	fitsfile *fptr;
	// bitpix (32 - int, 64 - long int, -32 - float, -64 - double)
	int status = 0, bitpix, num_axis, anynul;
	long axis_dim[10], fpixel[2]={1,1};

	fits_open_file(&fptr, filename.c_str(), 0, &status);
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

	if (data_created)
	{
		delete data;
		delete orig_data;
	}

	data = new T[width * height];
	orig_data = new T[width * height];

	fits_read_pix(fptr, f_type, fpixel, width * height, NULL, orig_data,
				  &anynul, &status);
	fits_close_file(fptr, &status);

	normalize_data();
}

template <typename T>
void SurfaceData<T>::set_max(T new_max)
{
	max = new_max;
	for (int i = 0; i < width * height; i++)
	{
		data[i] = (orig_data[i] - min) / (max - min);
	}

}


template <typename T>
void SurfaceData<T>::set_min(T new_min)
{
	min = new_min;
	for (int i = 0; i < width * height; i++)
	{
		data[i] = (orig_data[i] - min) / (max - min);
	}

}


template <typename T>
void SurfaceData<T>::normalize_data()
{
	max = -1E100;
	for (int i = 0; i < width * height; i++)
		if (orig_data[i] > max)
			max = orig_data[i];

	min = 1E100;
	for (int i = 0; i < width * height; i++)
		if (orig_data[i] < min)
			min = orig_data[i];

	for (int i = 0; i < width * height; i++)
	{
		data[i] = (orig_data[i] - min) / (max - min);
	}


// 	for (int i = 0; i < width * height; i++)
// 	{
// 		local_buffer[i * 4 + 0] = (fits_data[i] - min) / (max - min);
// 		local_buffer[i * 4 + 1] = (fits_data[i] - min) / (max - min);
// 		local_buffer[i * 4 + 2] = (fits_data[i] - min) / (max - min);
// 		local_buffer[i * 4 + 3] = 1.;
// 	}
}


template <typename T>
void SurfaceData<T>::fill_with_perlin_noise(double size_factor, T max_val, bool cylindrical_projection, int seed)
{
	PerlinNoise pn(seed);
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			double x = (double)i / ((double)width);
			double y = (double)j / ((double)height);
			double z = 0.5;

			if (cylindrical_projection)
			{
				x -= 0.5;
				x = x * cos( (y-0.5) * M_PI );
				x += 0.5;
			}

			x *= size_factor * 2.;
			y *= size_factor;
			z *= size_factor;

			double noise = pn.noise(x, y, z);
			data[j * width + i] = (T)(noise * max_val);
		}
}

#endif /* SURFACEDATA_H_ */

