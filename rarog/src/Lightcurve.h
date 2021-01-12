#include "Entity.h"

class Lightcurve
{
public:
	Lightcurve();
    Lightcurve(unsigned int size);
    ~Lightcurve();

	float& operator[](size_t i)
	{
		return fluxes[i];
	}

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

	void save_flux(const char *filename);
	void save_mag(const char *filename);
	size_t size() { return fluxes.size(); }

	float* fluxes_data() { return fluxes.data(); }
	float* mag_data() { return magnitudes.data(); }
	float* inv_mag_data() { return inverse_magnitudes.data(); }

	float min, max;

	Entity ghost_observer, ghost_target;
	Entity target;
// 	float camera_fov;


private:
	vector<float> fluxes;
	vector<float> magnitudes;
	vector<float> inverse_magnitudes;

};

