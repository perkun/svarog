#include "svpch.h"
#include "Fits.h"


Fits::Fits()
{
}

Fits::Fits(string filename)
{
	status = 0;

    fits_open_file(&fptr, filename.c_str(), 0, &status);
	if (status != 0)
	{
		char err_text[30];
		fits_get_errstatus(status, err_text);
		cout << "cannot open FITS file, status: "<< err_text << "\n";
		return;
	}

	status = 0;
    fits_get_img_param(fptr, 10, &bitpix, &num_axis, axis_dim, &status);

	if (bitpix == 32)
		datatype = TINT;
	else if (bitpix == 64)
		datatype = TLONG;
	else if (bitpix == -32)
		datatype = TFLOAT;
	else if (bitpix == -64)
		datatype = TDOUBLE;
	else
		datatype = TFLOAT;

	nelements = 1;
	for (int i = 0; i < num_axis; i++)
	{
		nelements *= axis_dim[i];
	}
}


Fits::~Fits()
{

}

void Fits::close()
{
	fits_close_file(fptr, &status);
	fptr = NULL;
}

Fits2D::Fits2D()
{

}

Fits2D::Fits2D(string filename) : Fits(filename)
{
    if (num_axis != 2)
    {
        cout << "Fits " << filename << " not a surface! Number of axes is "
             << num_axis << endl;
    }
	dim_x = axis_dim[0];
	dim_y = axis_dim[1];
}


Fits2DFloat::Fits2DFloat()
{
	datatype = TFLOAT;
}


Fits2DFloat::Fits2DFloat(string filename) : Fits2D(filename)
{
	data = new float[dim_x * dim_y];
	read_data();
}


Fits2DFloat::~Fits2DFloat()
{
	delete[] data;
}


void Fits2DFloat::read_data()
{
    if (datatype != TFLOAT)
    {
        cout << "Fits data not Float! Conversion..." << endl;
    }

    int anynul;
    fits_read_pix(fptr, datatype, fpixel, nelements, NULL, data, &anynul,
                  &status);
}


float Fits2DFloat::value_at(int x, int y)
{
	if (x >= dim_x || y >= dim_y || x < 0 || y < 0)
	{
		cout << "Fits2DFloat data: Index out of range." << endl;
		return 0;
	}
	return data[y * dim_x + x];
}

// 	fitsfile *fptr;
// 	// bitpix (32 - int, 64 - long int, -32 - float, -64 - double)
// 	int status = 0, bitpix, num_axis, anynul;
// 	long axis_dim[10], fpixel[2]={1,1};
//
// 	fits_open_file(&fptr, path.c_str(), 0, &status);
// 	if (status != 0)
// 	{
// 		char err_text[30];
// 		fits_get_errstatus(status, err_text);
// 		cout << "cannot open FITS file, status: "<< err_text << "\n";
// 		return;
// 	}
//
// 	// image info
// 	fits_get_img_param(fptr, 2, &bitpix, &num_axis, axis_dim, &status);
//
// 	fits_read_pix(fptr, TFLOAT, fpixel, data_size, NULL, fits_data,
// 			&anynul, &status);
// 	fits_close_file(fptr, &status);
