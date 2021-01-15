#ifndef RADARIMAGE_H_
#define RADARIMAGE_H_

#include "Texture.h"
#include "Entity.h"

using namespace std;

class RadarImage
{
public:
	RadarImage();
	RadarImage(int, int);
	~RadarImage();

	Entity target, observer, ghost_target, ghost_observer;

	shared_ptr<Texture> texture;

};

#endif /* RADARIMAGE_H_ */
