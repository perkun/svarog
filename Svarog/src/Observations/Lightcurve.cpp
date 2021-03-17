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


void Lightcurve::save_flux(string filename)
{
	filename = File::remove_extension(filename) + ".dat";
    FILE *out = fopen(filename.c_str(), "w");
// 	vector<float> tmp_fluxes = fluxes;
// 	make_average_zero(tmp_fluxes);
    for (int i = 0; i < fluxes.size(); i++)
        fprintf(out, "%f\t%f\n", (float)i / fluxes.size(), fluxes[i]);
    fclose(out);
}
void Lightcurve::save_mag(string filename)
{
	filename = File::remove_extension(filename) + ".dat";
    FILE *out = fopen(filename.c_str(), "w");
// 	vector<float> magnitudes = fluxes;
// 	for (float &m: magnitudes)
// 		m = -2.5 * log10(m);
// 	make_average_zero(magnitudes);

    for (int i = 0; i < magnitudes.size(); i++)
        fprintf(out, "%f\t%f\n", (float)i / magnitudes.size(), magnitudes[i]);

    fclose(out);
}


string Lightcurve::get_obs_type_string()
{
	return string("lc");
}

void Lightcurve::serialize(YAML::Emitter &out, int id, string filename)
{
	out << YAML::BeginMap;

	out << YAML::Key << "jd" << YAML::Value << julian_day;

	out.SetSeqFormat(YAML::Flow);
	out << YAML::Key << "target_position" << YAML::BeginSeq;
	out << YAML::Value << target_transform.position.x;
	out << YAML::Value << target_transform.position.y;
	out << YAML::Value << target_transform.position.z << YAML::EndSeq;

	out << YAML::Key << "observer_position" << YAML::BeginSeq;
	out << YAML::Value << observer_transform.position.x;
	out << YAML::Value << observer_transform.position.y;
	out << YAML::Value << observer_transform.position.z << YAML::EndSeq;
	out.SetSeqFormat(YAML::Block);

	out << YAML::Key << "lc_num_points" << YAML::Value << size();

	out << YAML::Key << "type";
	out << YAML::BeginSeq <<  YAML::Value << get_obs_type_string() << YAML::EndSeq;

	if (filename != "")
	{
		char fn[200];
		string base = File::remove_extension(filename);
		sprintf(fn, "%s_flux_%03d", base.c_str(), id);
		out << YAML::Key << "flux_data" << YAML::Value << filename + ".dat";
		save_flux(fn);

		sprintf(fn, "%s_mag_%03d", base.c_str(), id);
		out << YAML::Key << "mag_data" << YAML::Value << filename + ".dat";
		save_mag(fn);
	}


	out << YAML::EndMap;
}

