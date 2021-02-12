#ifndef OBSERVATIONSERIES_H_
#define OBSERVATIONSERIES_H_

#include "MainLayer.h"
#include "Observation.h"
#include "Lightcurve.h"
#include "Utils/File.h"

using namespace std;

class ObservationSeries
{
public:
	ObservationSeries() {}
	virtual ~ObservationSeries();
	int current_id = 0;

	void validate_current_id();
	void detach_all_ghosts();

	void push(Observation* obs);
	Observation* get_current_obs();
	void delete_current_obs();
	void delete_all();

	int size();

protected:
	vector<Observation*> observations;

};



class LightcurveSeries : public ObservationSeries
{
public:
	LightcurveSeries() {}
	~LightcurveSeries() {}

	void save_current_mag(const char *filename);
	void save_current_flux(const char *filename);
	void push(Lightcurve*);

	float lcs_min = 1e38;
	float lcs_max = -1e38;

private:

};


class ImageSeries : public ObservationSeries
{
public:
	ImageSeries() {}
	~ImageSeries() {}

	void save(const char *filename);
	void save_all(const char *filename);


};

#endif /* OBSERVATIONSERIES_H_ */
