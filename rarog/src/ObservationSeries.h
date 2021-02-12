#ifndef OBSERVATIONSERIES_H_
#define OBSERVATIONSERIES_H_

#include "MainLayer.h"
#include "Observation.h"
#include "Lightcurve.h"
#include "AoImage.h"
#include "RadarImage.h"

using namespace std;

template<typename T>
class ObservationSeries
{
public:
	ObservationSeries() {}
	virtual ~ObservationSeries();
	int current_id = 0;

	void validate_current_id();
	void detach_all_ghosts();

	virtual void push(T* obs);
	T* get_current_obs();
	void delete_current_obs();

	int size();

protected:
	vector<T*> observations;

};

template<typename T>
ObservationSeries<T>::~ObservationSeries()
{
	for (int i = 0; i < observations.size(); i++)
		delete observations[i];
}


template<typename T>
void ObservationSeries<T>::detach_all_ghosts()
{
    for (T *obs : observations)
    {
        obs->ghost_target.detatch();
        obs->ghost_observer.detatch();
    }
}

template<typename T>
void ObservationSeries<T>::push(T *obs)
{
	observations.push_back(obs);
    current_id = observations.size() - 1;
}


template<typename T>
T* ObservationSeries<T>::get_current_obs()
{
	validate_current_id();

	if (size() > 0)
		return observations[current_id];
	else
		return NULL;
}


template<typename T>
void ObservationSeries<T>::delete_current_obs()
{
	T *current_obs = get_current_obs();
	current_obs->ghost_observer.destroy();
	current_obs->ghost_target.destroy();

	delete current_obs;

	observations.erase(observations.begin() + current_id);
	current_id--;
}

template<typename T>
int ObservationSeries<T>::size()
{
	return observations.size();
}


template<typename T>
void ObservationSeries<T>::validate_current_id()
{
    if (current_id < 0)
        current_id = 0;
    else if (current_id >= observations.size())
        current_id = observations.size() - 1;
}


class LightcurveSeries : public ObservationSeries<Lightcurve>
{
public:
	LightcurveSeries() {}
	~LightcurveSeries() {}

	void save_current_mag(const char *filename);
	void save_current_flux(const char *filename);
	virtual void push(Lightcurve*) override;

	float lcs_min = 1e38;
	float lcs_max = -1e38;

private:

};


class AoImageSeries : public ObservationSeries<AoImage>
{
public:
	AoImageSeries() {}
	~AoImageSeries() {}

	void save(const char *filename);

};

class RadarImageSeries : public ObservationSeries<RadarImage>
{
public:
	RadarImageSeries() {}
	~RadarImageSeries() {}

	void save(const char *filename);
};
#endif /* OBSERVATIONSERIES_H_ */
