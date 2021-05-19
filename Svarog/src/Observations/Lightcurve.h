#ifndef LIGHTCURVE_H_
#define LIGHTCURVE_H_
#include "Observation.h"

struct LcPoint
{
    LcPoint()
    {
        phase = 0.;
        flux = mag = inv_mag = 0.;
    }

    LcPoint(double phase, double flux)
    {
        this->phase = phase;
        this->flux = flux;
        if (flux == 0)
        {
            mag = 0;
            inv_mag = 0;
        }
        else
        {
            mag = -2.5 * log10(flux);
            inv_mag = 2.5 * log10(flux);
        }
    }

    double phase;              // phase of observation (in radians)
    double flux, mag, inv_mag; // value
};

class Lightcurve : public Observation
{
public:
    Lightcurve(Entity &target, Entity &observer, unsigned int size);
    ~Lightcurve();

    LcPoint &operator[](size_t i)
    {
        return points[i];
    }

    virtual void serialize(YAML::Emitter &out, string storage_name, int id = 0,
                           string filename = "") override;

    void shift_fluxes(float val);
    void make_average_zero();
    // 	void make_average_zero(vector<float> &data);
    void push_flux(double phase, double flux);

    float calculate_min_flux();
    float calculate_max_flux();
    float calculate_min_mag();
    float calculate_max_mag();
    float calculate_min_inv_mag();
    float calculate_max_inv_mag();

    void sort();

    void save_flux(string filename);
    void save_mag(string filename);
    size_t size()
    {
        return points.size();
    }

    // 	float* fluxes_data() { return fluxes.data(); }
    // 	float* mag_data() { return magnitudes.data(); }
    // 	float* inv_mag_data() { return inverse_magnitudes.data(); }
    // 	vector<LcPoint>* get_points() { return &points; }

    virtual string get_obs_type_string() override;

private:
    vector<LcPoint> points;
};

#endif
