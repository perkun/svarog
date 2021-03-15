#ifndef IMAGE_H_
#define IMAGE_H_

#include "Observation.h"
#include "FitsHeader.h"
#include "Texture.h"

using namespace std;

class Image : public Observation
{
public:
	Image(Entity& target, Entity &observer, int width, int height);
	~Image();
	virtual void serialize(YAML::Emitter &out) override;
	virtual FitsHeader get_fits_header() override;

	int width, height;
	int texture_format;
	float *data_r, *data_g, *data_b;

	shared_ptr<Texture> texture;

	void update_data(float *dr, float *dg, float *db);
	void update_texture();

	void save_png(const char *filename);
	void save_fits_greyscale(const char *filename, const FitsHeader& header);

protected:
	TextureSpec texture_specs;

};


class AoImage : public Image
{
public:
	AoImage(Entity &target, Entity &observer, int width, int height);
	~AoImage();
	virtual void serialize(YAML::Emitter &out) override;
	virtual FitsHeader get_fits_header() override;

private:

};


class RadarImage : public Image
{
public:
    RadarImage(Entity &target, Entity &observer, int width, int height,
               float min_depth, float max_depth, float min_vr, float max_vr)
        : Image(target, observer, width, height), min_depth(min_depth),
          max_depth(max_depth), min_vr(min_vr), max_vr(max_vr)
    {
        ghost_color = vec4(255 / 256., 209 / 255., 0., 0.2);
    }

    float min_depth, max_depth, min_vr, max_vr;

    void construct_delay_doppler(float *radial_vel_buffer, float *depth_bufer,
                                 float *normal_buffer, int width, int height);

    virtual void serialize(YAML::Emitter &out) override;

    virtual FitsHeader get_fits_header() override;
};


#endif /* IMAGE_H_ */
