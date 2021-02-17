#ifndef AOIMAGE_H_
#define AOIMAGE_H_

#include "Observation.h"
#include "Texture.h"

using namespace std;

class AoImage : public Observation
{
public:
	AoImage(Entity &target, Entity &observer, int width, int height);
	~AoImage();
	virtual void serialize(YAML::Emitter &out) override;

	shared_ptr<Texture> texture;

private:

};

#endif /* AOIMAGE_H_ */
