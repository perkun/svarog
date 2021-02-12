#ifndef RADARIMAGE_H_
#define RADARIMAGE_H_

#include "Observation.h"
#include "Texture.h"

using namespace std;

class RadarImage : public Observation
{
public:
	RadarImage();
	RadarImage(int, int);
	~RadarImage();

	shared_ptr<Texture> texture;
// 	shared_ptr<Texture> radial;
// 	shared_ptr<Texture> depth;

};

#endif /* RADARIMAGE_H_ */
