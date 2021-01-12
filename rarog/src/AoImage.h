#ifndef AOIMAGE_H_
#define AOIMAGE_H_

#include "Texture.h"
#include "Entity.h"

using namespace std;

class AoImage
{
public:
	AoImage(int width, int height);
	~AoImage();

	shared_ptr<Texture> texture;

	Entity ghost_observer, ghost_target;
	Entity target, observer;
// 	float camera_fov;

private:

};

#endif /* AOIMAGE_H_ */
