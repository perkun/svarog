#include "svpch.h"
#include "Lightcurve.h"

Lightcurve::Lightcurve(unsigned int size)
{
    fluxes.reserve(size);
}

Lightcurve::~Lightcurve()
{
}

void Lightcurve::shift(float val)
{
    for (float &f : fluxes)
        f += val;
}

void Lightcurve::make_average_zero()
{
    float avg = 0.;
    for (float f : fluxes)
        avg += f;
    avg /= fluxes.size();

    shift(-avg);
}

void Lightcurve::make_average_zero(vector<float> &data)
{
    float avg = 0.;
    for (float f : data)
        avg += f;
    avg /= data.size();

	for (float &v: data)
		v -= avg;
}

void Lightcurve::push_value(float val)
{
    fluxes.emplace_back(val);
}

float Lightcurve::calculate_min()
{
    min = fluxes[0];
    for (int i = 1; i < fluxes.size(); i++)
        if (fluxes[i] < min)
            min = fluxes[i];
    return min;
}
float Lightcurve::calculate_max()
{
    max = fluxes[0];
    for (int i = 1; i < fluxes.size(); i++)
        if (fluxes[i] > max)
            max = fluxes[i];
    return max;
}

void Lightcurve::save_flux(const char *filename)
{
    FILE *out = fopen(filename, "w");
	vector<float> tmp_fluxes = fluxes;
	make_average_zero(tmp_fluxes);
    for (int i = 0; i < fluxes.size(); i++)
        fprintf(out, "%f\t%f\n", (float)i / tmp_fluxes.size(), tmp_fluxes[i]);
    fclose(out);
}
void Lightcurve::save_mag(const char *filename)
{
    FILE *out = fopen(filename, "w");
	vector<float> magnitudes = fluxes;
	for (float &m: magnitudes)
		m = -2.5 * log10(m);

	make_average_zero(magnitudes);

    for (int i = 0; i < magnitudes.size(); i++)
        fprintf(out, "%f\t%f\n", (float)i / fluxes.size(), magnitudes[i]);

    fclose(out);
}
