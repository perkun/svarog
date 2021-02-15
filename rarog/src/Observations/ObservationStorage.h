#ifndef OBSERVATIONSTORAGE_H_
#define OBSERVATIONSTORAGE_H_

#include "ObservationSeries.h"

using namespace std;

class ObservationStorage
{
public:
	ObservationStorage();
	~ObservationStorage();

	void save();
	void load(string filename);

	string filename = "untitled.yaml";

	LightcurveSeries lightcurves;
	ImageSeries ao_images;
	ImageSeries radar_images;


};

#endif /* OBSERVATIONSTORE_H_ */
