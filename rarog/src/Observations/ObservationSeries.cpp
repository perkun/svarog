#include "svpch.h"
#include "ObservationSeries.h"

void ObservationSeries::serialize(YAML::Emitter &out)
{
}


ObservationSeries::~ObservationSeries()
{
	delete_all();
}

void ObservationSeries::delete_all()
{
	detach_all_ghosts();

	for (int i = 0; i < observations.size(); i++)
	{
		delete observations[i];
	}
}


void ObservationSeries::detach_all_ghosts()
{
    for (Observation *obs : observations)
    {
		if (!obs)
			continue;

        obs->ghost_target.detatch();
        obs->ghost_observer.detatch();
    }
}

void ObservationSeries::push(Observation *obs)
{
	observations.push_back(obs);
    current_id = observations.size() - 1;
}


Observation* ObservationSeries::get_current_obs()
{
	validate_current_id();

	if (size() > 0)
		return observations[current_id];
	else
	{
		return NULL;
	}
}


void ObservationSeries::delete_current_obs()
{
	Observation *current_obs = get_current_obs();

	delete current_obs;

	observations.erase(observations.begin() + current_id);
	current_id--;
}

int ObservationSeries::size()
{
	return observations.size();
}


void ObservationSeries::validate_current_id()
{
    if (current_id < 0)
        current_id = 0;
    else if (current_id >= observations.size())
        current_id = observations.size() - 1;
}





void LightcurveSeries::push(Lightcurve *lc)
{
    lc->make_average_zero();
    float min = lc->calculate_min_inv_mag();
    float max = lc->calculate_max_inv_mag();
    if (min < lcs_min)
        lcs_min = min;
    if (max > lcs_max)
        lcs_max = max;


    observations.push_back(lc);
    current_id = observations.size() - 1;
}


void LightcurveSeries::save_current_mag(const char *filename)
{
	static_cast<Lightcurve*>(get_current_obs())->save_mag(filename);
}

void LightcurveSeries::save_current_flux(const char *filename)
{
	static_cast<Lightcurve*>(get_current_obs())->save_flux(filename);
}


void LightcurveSeries::serialize(YAML::Emitter &out)
{
	for (int i = 0; i < observations.size(); i++)
		static_cast<Lightcurve*>(observations[i])->serialize(out);
}





void ImageSeries::save_png(const char *filename)
{
	string base =  File::remove_extension(filename);
	string fn = base + ".png";
	static_cast<Image*>(get_current_obs())->texture->save_png(fn.c_str());
}

FitsHeader ObservationSeries::make_fits_header(Observation *obs)
{
	FitsHeader header;

    header.push("obs_x", obs->observer_transform.position.x,
                "observer x position [au]");
    header.push("obs_y", obs->observer_transform.position.y,
                "observer y position [au]");
    header.push("obs_z", obs->observer_transform.position.z,
                "observer z position [au]");

    header.push("pos_x", obs->target_transform.position.x,
                "tarteg x position [au]");
    header.push("pos_y", obs->target_transform.position.y,
                "tarteg y position [au]");
    header.push("pos_z", obs->target_transform.position.z,
                "tarteg z position [au]");

    header.push("lambda", obs->target_orbital_component.lambda,
                "lambda angle [rad]");
    header.push("beta", obs->target_orbital_component.beta, "beta angle [rad]");
    header.push("gamma", obs->target_orbital_component.gamma,
                "gamma angle [rad]");

    header.push("phase", obs->target_orbital_component.rotation_phase,
                "rotation phase [rad]");

    header.push("period", obs->target_orbital_component.rot_period,
                "rotation period [h]");

    header.push("jd_0", obs->target_orbital_component.jd_0,
                "reference eopch for gamma=0 [julian day]");

    header.push("jd", obs->julian_day, "image epoch");

	return header;
}

void ImageSeries::save_fits_greyscale(const char *filename)
{
    FitsHeader header = make_fits_header(get_current_obs());

    string base = File::remove_extension(filename);
    string fn = base + ".fits";
    static_cast<Image *>(get_current_obs())
        ->texture->save_fits_greyscale(fn.c_str(), header);
}


void ImageSeries::save_all_png(const char *filename)
{
	char fn[200];
	string base =  File::remove_extension(filename);
	int nr = 0;
	for (Observation *obs : observations)
	{
		sprintf(fn, "%s_%03d.png", base.c_str(), nr);
		static_cast<Image*>(obs)->texture->save_png(fn);
		nr++;
	}
}

void ImageSeries::save_all_fits(const char *filename)
{

	char fn[200];
	string base =  File::remove_extension(filename);
	int nr = 0;
	for (Observation *obs : observations)
	{
    	FitsHeader header = make_fits_header(obs);
		sprintf(fn, "%s_%03d.fits", base.c_str(), nr);
		static_cast<Image*>(obs)->texture->save_fits_greyscale(fn, header);
		nr++;
	}
}

void ImageSeries::serialize(YAML::Emitter &out)
{
	for (int i = 0; i < observations.size(); i++)
		static_cast<Image*>(observations[i])->serialize(out);
}

