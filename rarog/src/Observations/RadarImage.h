#ifndef RADARIMAGE_H_
#define RADARIMAGE_H_

#include "Observation.h"

using namespace std;

class RadarImage : public Image
{
public:
    RadarImage(Entity &target, Entity &observer, int width, int height)
        : Image(target, observer, width, height)
    {
		ghost_color = vec4(255 / 256., 209 / 255., 0., 0.2);
    }

    void construct_delay_doppler(float *radial_vel_buffer, float *depth_bufer,
                                 float *normal_buffer, int width, int height);

	virtual void serialize(YAML::Emitter &out) override;

};

#endif /* RADARIMAGE_H_ */
