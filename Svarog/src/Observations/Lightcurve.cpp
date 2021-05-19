#include "svpch.h"
#include "Lightcurve.h"

Lightcurve::Lightcurve(Entity &target, Entity &observer, unsigned int size)
	: Observation(target, observer)
{
    points.reserve(size);
	ghost_color = vec4(32 / 256., 172 / 256., 64 / 256., 0.2);
}

Lightcurve::~Lightcurve()
{
}

void Lightcurve::shift_fluxes(float val)
{
    for (LcPoint &p : points)
	{
        p.flux += val;
	}
}

void Lightcurve::make_average_zero()
{
	LcPoint avg;
	for (LcPoint &p: points)
	{
		avg.flux += p.flux;
		avg.mag += p.mag;
		avg.inv_mag += p.inv_mag;
	}
	avg.flux /= points.size();
	avg.mag /= points.size();
	avg.inv_mag /= points.size();

// 	printf("%.16lf %.16lf %.16lf\n", avg.flux, avg.mag, avg.inv_mag);

	for (LcPoint &p: points)
	{
		p.flux -= avg.flux;
		p.mag -= avg.mag;
		p.inv_mag -= avg.inv_mag;
	}
}

// void Lightcurve::make_average_zero(vector<float> &data)
// {
//     float avg = 0.;
//     for (float f : data)
//         avg += f;
//     avg /= data.size();
//
// 	for (float &v: data)
// 		v -= avg;
// }

void Lightcurve::push_flux(double phase, double flux)
{
	points.emplace_back(LcPoint(phase, flux));
//     fluxes.emplace_back(val);
// 	magnitudes.emplace_back(-2.5 * log10(val));
// 	inverse_magnitudes.emplace_back(2.5 * log10(val));
}

float Lightcurve::calculate_min_flux()
{
    float min = points[0].flux;
    for (int i = 1; i < points.size(); i++)
        if (points[i].flux < min)
            min = points[i].flux;
    return min;
}
float Lightcurve::calculate_max_flux()
{
    float max = points[0].flux;
    for (int i = 1; i < points.size(); i++)
        if (points[i].flux > max)
            max = points[i].flux;
    return max;
}


float Lightcurve::calculate_min_mag()
{
    float min = points[0].mag;
    for (int i = 1; i < points.size(); i++)
        if (points[i].mag < min)
            min = points[i].mag;
    return min;
}
float Lightcurve::calculate_max_mag()
{
    float max = points[0].mag;
    for (int i = 1; i < points.size(); i++)
        if (points[i].mag > max)
            max = points[i].mag;
    return max;
}


float Lightcurve::calculate_min_inv_mag()
{
    float min = points[0].inv_mag;
    for (int i = 1; i < points.size(); i++)
        if (points[i].inv_mag < min)
            min = points[i].inv_mag;
    return min;
}
float Lightcurve::calculate_max_inv_mag()
{
    float max = points[0].inv_mag;
    for (int i = 1; i < points.size(); i++)
        if (points[i].inv_mag > max)
            max = points[i].inv_mag;
    return max;
}


void Lightcurve::save_flux(string filename)
{
    FILE *out = fopen(filename.c_str(), "w");
// 	vector<float> tmp_fluxes = fluxes;
// 	make_average_zero(tmp_fluxes);
//     for (int i = 0; i < points.size(); i++)
	for (LcPoint &p: points)
        fprintf(out, "%f\t%f\n", p.phase, p.flux);
    fclose(out);
}
void Lightcurve::save_mag(string filename)
{
    FILE *out = fopen(filename.c_str(), "w");
// 	vector<float> magnitudes = fluxes;
// 	for (float &m: magnitudes)
// 		m = -2.5 * log10(m);
// 	make_average_zero(magnitudes);

//     for (int i = 0; i < magnitudes.size(); i++)
//         fprintf(out, "%f\t%f\n", (float)i / magnitudes.size(), magnitudes[i]);

	for (LcPoint &p: points)
        fprintf(out, "%f\t%f\n", p.phase, p.mag);

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
		sprintf(fn, "%s_flux_%03d.dat", base.c_str(), id);
		out << YAML::Key << "flux_data" << YAML::Value << fn;
		save_flux(fn);

		sprintf(fn, "%s_mag_%03d", base.c_str(), id);
		out << YAML::Key << "mag_data" << YAML::Value << fn;
		save_mag(fn);
	}


	out << YAML::EndMap;
}


void Lightcurve::sort()
{
    std::sort(points.begin(), points.end(),
         [](const LcPoint &a, const LcPoint &b) { return a.phase < b.phase; });
}
