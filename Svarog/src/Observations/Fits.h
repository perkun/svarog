#ifndef FITS_H_
#define FITS_H_

#include "FitsHeader.h"

using namespace std;

class Fits
{
public:
	Fits();
	Fits(string filename);
	~Fits();
	void close();


	int read_header_key_int(string key) {
		if (fptr != NULL)
			return header.read_int_key(fptr, key);
		else
			return 0;
	}

	float read_header_key_float(string key) {
		if (fptr != NULL)
			return header.read_float_key(fptr, key);
		else
			return 0;
	}

	double read_header_key_double(string key) {
		if (fptr != NULL)
			return header.read_double_key(fptr, key);
		else
			return 0;
	}

	string read_header_key_string(string key) {
		if (fptr != NULL)
			return header.read_string_key(fptr, key);
		else
			return string("");
	}

    int num_axis;
    long axis_dim[10];

	FitsHeader header;
protected:
	int datatype = TFLOAT;
	int bitpix;
    fitsfile *fptr = NULL;
    int status = 0;
	long fpixel[10]={1,1,1,1,1,1,1,1,1,1};
	long long nelements;
};


class Fits2D : public Fits
{
public:
	Fits2D();
	Fits2D(string filename);


	int dim_x, dim_y;
protected:
};

class Fits2DFloat : public Fits2D
{
public:
	Fits2DFloat();
	Fits2DFloat(string filename);
	~Fits2DFloat();

	void read_data();
	float value_at(int x, int y);

	float *data;
};

#endif /* FITS_H_ */
