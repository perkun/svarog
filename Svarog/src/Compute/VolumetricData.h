#ifndef VOLUMETRICDATA_H_
#define VOLUMETRICDATA_H_

#include <iostream>
#include <stdio.h>
#include "PerlinNoise.h"
#include <math.h>

using namespace std;

template<typename T>
class VolumetricData
{
public:
    VolumetricData(unsigned int dim_x, unsigned int dim_y, unsigned int dim_z)
		: dim_x(dim_x), dim_y(dim_y), dim_z(dim_z)
	{
    	data = new T[dim_x * dim_y * dim_z];
	}

	~VolumetricData() {
		delete data;
	}

	void fill_with_perlin_noise(double size_factor, T max_val, bool cylindrical_projection, int seed);

	T* get_data() {
		return data;
	}

	unsigned int dim_x, dim_y, dim_z;

// private:
	T *data;

};

template<typename T>
void VolumetricData<T>::fill_with_perlin_noise(double size_factor, T max_val, bool cylindrical_projection,  int seed)
{
    PerlinNoise pn(seed);

    for (unsigned int k = 0; k < dim_z; k++)
        for (unsigned int j = 0; j < dim_y; j++)
		{
            for (unsigned int i = 0; i < dim_x; i++)
            {
                double x = (double)i / ((double)dim_x);
                double y = (double)j / ((double)dim_y);
                double z = (double)k / ((double)dim_z);

				if (cylindrical_projection)
				{
					x -= 0.5;
					x = x * cos( (y-0.5) * M_PI );
					x += 0.5;
				}

				x *= size_factor * 2.;
				y *= size_factor;
				z *= size_factor;

                double noise = pn.noise(x,  y,  z);
// 				cout << floor(noise * 255) << "  ";

                data[k * dim_y * dim_x + j * dim_x + i] = (T)(noise * max_val);
			}
// 			cout << endl;
		}

// 	cout << "-----------------------------------------" << endl;
}
#endif /* VOLUMETRICDATA_H_ */
