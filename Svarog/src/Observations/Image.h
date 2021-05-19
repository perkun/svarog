#ifndef IMAGE_H_
#define IMAGE_H_

#include "Observation.h"
#include "Texture.h"

using namespace std;

class Image : public Observation
{
public:
    Image(Entity &target, Entity &observer, int width, int height,
          bool create_texture = true);
    ~Image();
    virtual void serialize(YAML::Emitter &out, string storage_name, int id = 0,
                           string filename = "") override;
    void update_data(float *dr, float *dg, float *db);
    void update_texture();

    void save_png(string filename);
    void save_fits_greyscale(string filename);

	virtual string get_obs_type_string() override;

	virtual FitsHeader get_fits_header() = 0;

    int width, height;
    int texture_format;
    float *data_r, *data_g, *data_b;

	bool texture_created = false;
    shared_ptr<Texture> texture;

protected:
	void push_basic_header_info(FitsHeader &header);
    TextureSpec texture_specs;
};


class AoImage : public Image
{
public:
	AoImage(Entity &target, Entity &observer, int width, int height,
			bool create_texture = true);
	~AoImage();
	virtual FitsHeader get_fits_header() override;
	virtual string get_obs_type_string() override;
private:

};


class RadarImage : public Image
{
public:
    RadarImage(Entity &target, Entity &observer, int width, int height,
               float min_depth, float max_depth, float min_vr, float max_vr,
			   bool create_texture = true);

    float min_depth, max_depth, min_vr, max_vr;

    void construct_delay_doppler(float *radial_vel_buffer, float *depth_bufer,
                                 float *normal_buffer, int width, int height);

    virtual FitsHeader get_fits_header() override;
	virtual string get_obs_type_string() override;
};


#endif /* IMAGE_H_ */
