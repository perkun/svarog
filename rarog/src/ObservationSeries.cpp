#include "ObservationSeries.h"










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
	get_current_obs()->save_mag(filename);
}

void LightcurveSeries::save_current_flux(const char *filename)
{
	get_current_obs()->save_flux(filename);
}


void AoImageSeries::save(const char *filename)
{
	get_current_obs()->texture->save(filename);
}



void RadarImageSeries::save(const char *filename)
{
	get_current_obs()->texture->save(filename);
}

