#ifndef LIGHTCURVE_H_
#define LIGHTCURVE_H_
#include "Observation.h"

class Lightcurve : public Observation
{
public:
    Lightcurve(Entity &target, Entity &observer, unsigned int size);
    ~Lightcurve();

	float& operator[](size_t i)
	{
		return fluxes[i];
	}

	virtual void serialize(YAML::Emitter &out, int id = 0, string filename = "") override;

	void shift_fluxes(float val);
	void make_average_zero();
	void make_average_zero(vector<float> &data);
	void push_flux(float val);

	float calculate_min_flux();
	float calculate_max_flux();
	float calculate_min_mag();
	float calculate_max_mag();
	float calculate_min_inv_mag();
	float calculate_max_inv_mag();

	void save_flux(string filename);
	void save_mag(string filename);
	size_t size() { return fluxes.size(); }

	float* fluxes_data() { return fluxes.data(); }
	float* mag_data() { return magnitudes.data(); }
	float* inv_mag_data() { return inverse_magnitudes.data(); }

	virtual string get_obs_type_string() override;

	float min, max;

private:
	vector<float> fluxes;
	vector<float> magnitudes;
	vector<float> inverse_magnitudes;

};

#endif
