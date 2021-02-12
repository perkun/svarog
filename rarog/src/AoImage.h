#ifndef AOIMAGE_H_
#define AOIMAGE_H_

#include "Observation.h"
#include "Texture.h"

using namespace std;

class AoImage : public Observation
{
public:
	AoImage(int width, int height);
	~AoImage();

	shared_ptr<Texture> texture;
private:

};

#endif /* AOIMAGE_H_ */
