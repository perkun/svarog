
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

	void shift(float val);
	void make_average_zero();
	void make_average_zero(vector<float> &data);
	void push_value(float val);
	float calculate_min();
	float calculate_max();
	void save_flux(const char *filename);
	void save_mag(const char *filename);
	size_t size() { return fluxes.size(); }
	float* data() { return fluxes.data(); }

	float min, max;

private:
	vector<float> fluxes;

};

