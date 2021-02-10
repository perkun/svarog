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
// 	shared_ptr<Texture> radial;
// 	shared_ptr<Texture> depth;

};

#endif /* RADARIMAGE_H_ */
