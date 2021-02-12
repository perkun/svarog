#include "svpch.h"
#include "Lightcurve.h"

Lightcurve::Lightcurve(Entity &target, Entity &observer, unsigned int size)
	: Observation(target, observer)
{
    fluxes.reserve(size);
    magnitudes.reserve(size);
    inverse_magnitudes.reserve(size);
	ghost_color = vec4(32 / 256., 172 / 256., 64 / 256., 0.2);
}

Lightcurve::~Lightcurve()
{
}

void Lightcurve::shift_fluxes(float val)
{
    for (float &f : fluxes)
        f += val;
}

void Lightcurve::make_average_zero()
{
	make_average_zero(fluxes);
	make_average_zero(magnitudes);
	make_average_zero(inverse_magnitudes);
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

void Lightcurve::push_flux(float val)
{
    fluxes.emplace_back(val);
	magnitudes.emplace_back(-2.5 * log10(val));
	inverse_magnitudes.emplace_back(2.5 * log10(val));
}

float Lightcurve::calculate_min_flux()
{
    min = fluxes[0];
    for (int i = 1; i < fluxes.size(); i++)
        if (fluxes[i] < min)
            min = fluxes[i];
    return min;
}
float Lightcurve::calculate_max_flux()
{
    max = fluxes[0];
    for (int i = 1; i < fluxes.size(); i++)
        if (fluxes[i] > max)
            max = fluxes[i];
    return max;
}


float Lightcurve::calculate_min_mag()
{
    min = magnitudes[0];
    for (int i = 1; i < magnitudes.size(); i++)
        if (magnitudes[i] < min)
            min = magnitudes[i];
    return min;
}
float Lightcurve::calculate_max_mag()
{
    max = magnitudes[0];
    for (int i = 1; i < magnitudes.size(); i++)
        if (magnitudes[i] > max)
            max = magnitudes[i];
    return max;
}


float Lightcurve::calculate_min_inv_mag()
{
    min = inverse_magnitudes[0];
    for (int i = 1; i < inverse_magnitudes.size(); i++)
        if (inverse_magnitudes[i] < min)
            min = inverse_magnitudes[i];
    return min;
}
float Lightcurve::calculate_max_inv_mag()
{
    max = inverse_magnitudes[0];
    for (int i = 1; i < inverse_magnitudes.size(); i++)
        if (inverse_magnitudes[i] > max)
            max = inverse_magnitudes[i];
    return max;
}


void Lightcurve::save_flux(const char *filename)
{
    FILE *out = fopen(filename, "w");
// 	vector<float> tmp_fluxes = fluxes;
// 	make_average_zero(tmp_fluxes);
    for (int i = 0; i < fluxes.size(); i++)
        fprintf(out, "%f\t%f\n", (float)i / fluxes.size(), fluxes[i]);
    fclose(out);
}
void Lightcurve::save_mag(const char *filename)
{
    FILE *out = fopen(filename, "w");
// 	vector<float> magnitudes = fluxes;
// 	for (float &m: magnitudes)
// 		m = -2.5 * log10(m);
// 	make_average_zero(magnitudes);

    for (int i = 0; i < magnitudes.size(); i++)
        fprintf(out, "%f\t%f\n", (float)i / magnitudes.size(), magnitudes[i]);

    fclose(out);
}
