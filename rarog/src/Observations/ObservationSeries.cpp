#include "svpch.h"
#include "ObservationSeries.h"


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
		return NULL;
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


void ImageSeries::save(const char *filename)
{
	string base =  File::get_file_base(filename);

	string fn = base + ".png";

	static_cast<Image*>(get_current_obs())->texture->save(fn.c_str());
}


void ImageSeries::save_all(const char *filename)
{
	char fn[200];

	string base =  File::get_file_base(filename);

	int nr = 0;
	for (Observation *obs : observations)
	{
		sprintf(fn, "%s_%03d.png", base.c_str(), nr);
		static_cast<Image*>(obs)->texture->save(fn);
		nr++;
	}


}




