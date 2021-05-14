#include "svpch.h"
#include "ObservationSeries.h"

void ObservationSeries::serialize(YAML::Emitter &out)
{
	for (int i = 0; i < observations.size(); i++)
		observations[i]->serialize(out);
}

void ObservationSeries::serialize(YAML::Emitter &out, string storage_name)
{

    for (int i = 0; i < observations.size(); i++)
    {
        observations[i]->serialize(out, i, storage_name);
    }
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

Observation* ObservationSeries::get_obs(int id)
{
	if (id >= 0 && id < size())
		return observations[id];
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
    if (min < lcs_inv_mag_min)
        lcs_inv_mag_min = min;
    if (max > lcs_inv_mag_max)
        lcs_inv_mag_max = max;


    observations.push_back(lc);
    current_id = observations.size() - 1;
}


// void LightcurveSeries::save_current_mag(const char *filename)
// {
// 	static_cast<Lightcurve*>(get_current_obs())->save_mag(filename);
// }
//
// void LightcurveSeries::save_current_flux(const char *filename)
// {
// 	static_cast<Lightcurve*>(get_current_obs())->save_flux(filename);
// }
//
//
//
// void LightcurveSeries::save_all_flux(const char *filename)
// {
//     char fn[200];
//     string base = File::remove_extension(filename);
//     int nr = 0;
//     for (Observation *obs : observations)
//     {
//         sprintf(fn, "%s_flux_%03d.dat", base.c_str(), nr);
//         static_cast<Lightcurve *>(obs)->save_flux(fn);
//         nr++;
//     }
// }
//
//
// void LightcurveSeries::save_all_mag(const char *filename)
// {
//     char fn[200];
//     string base = File::remove_extension(filename);
//     int nr = 0;
//     for (Observation *obs : observations)
//     {
//         sprintf(fn, "%s_mag_%03d.dat", base.c_str(), nr);
//         static_cast<Lightcurve *>(obs)->save_mag(fn);
//         nr++;
//     }
// }



// void LightcurveSeries::serialize(YAML::Emitter &out)
// {
// 	for (int i = 0; i < observations.size(); i++)
// 		static_cast<Lightcurve*>(observations[i])->serialize(out);
// }





// void ImageSeries::save_png(const char *filename)
// {
// 	string base =  File::remove_extension(filename);
// 	string fn = base + ".png";
// 	static_cast<Image*>(get_current_obs())->save_png(fn.c_str());
// }
//
//
// void ImageSeries::save_fits_greyscale(const char *filename)
// {
//     string base = File::remove_extension(filename);
//     string fn = base + ".fits";
//     static_cast<Image *>(get_current_obs()) ->save_fits_greyscale(fn.c_str(),
//                                        get_current_obs()->get_fits_header());
// }
//
//
// void ImageSeries::save_all_png(const char *filename)
// {
//     char fn[200];
//     string base = File::remove_extension(filename);
//     int nr = 0;
//     for (Observation *obs : observations)
//     {
//         sprintf(fn, "%s_%s_%03d.png", base.c_str(), obs->get_obs_type().c_str(),
//                 nr);
//         static_cast<Image *>(obs)->save_png(fn);
//         nr++;
//     }
// }
//
// void ImageSeries::save_all_fits(const char *filename)
// {
//
//     char fn[200];
//     string base = File::remove_extension(filename);
//     int nr = 0;
//     for (Observation *obs : observations)
//     {
//         sprintf(fn, "%s_%s_%03d.fits", base.c_str(),
//                 obs->get_obs_type().c_str(), nr);
//         static_cast<Image *>(obs)->save_fits_greyscale(fn,
//                                                        obs->get_fits_header());
//         nr++;
//     }
// }

// void ImageSeries::serialize(YAML::Emitter &out)
// {
// 	for (int i = 0; i < observations.size(); i++)
// 		static_cast<Image*>(observations[i])->serialize(out);
// }

