#ifndef OBSERVATIONSERIES_H_
#define OBSERVATIONSERIES_H_

#include "Observation.h"
#include "Image.h"
#include "Lightcurve.h"
#include "Utils/File.h"
#include "Texture.h"

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

	virtual void serialize(YAML::Emitter &out);

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
	virtual void serialize(YAML::Emitter &out) override;

	float lcs_min = 1e38;
	float lcs_max = -1e38;


private:

};


class ImageSeries : public ObservationSeries
{
public:
	ImageSeries() {}
	~ImageSeries() {}

	void save_png(const char *filename);
	void save_fits_greyscale(const char *filename);
	void save_all_png(const char *filename);
	void save_all_fits(const char *filename);
	virtual void serialize(YAML::Emitter &out) override;

};

#endif /* OBSERVATIONSERIES_H_ */
