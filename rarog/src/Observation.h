#ifndef OBSERVATION_H_
#define OBSERVATION_H_

#include "Texture.h"
#include "Material.h"
#include "Components.h"
#include "IndexedModel.h"
#include "Application.h"
#include "Transform.h"
#include "Entity.h"

using namespace std;

class Observation
{
public:
	Observation(Entity& target, Entity &observer);
	virtual ~Observation();
	void add_ghosts(Entity& ghost_target, Entity &ghost_observer);

	Entity target, observer, ghost_target, ghost_observer;

	vec3 target_pos, observer_pos;
	double julian_day;

	vec4 ghost_color = vec4(1,1,1,1);
};


class Image : public Observation
{
public:
	Image(Entity& target, Entity &observer, int width, int height);
	~Image();

	shared_ptr<Texture> texture;

protected:
	TextureSpec texture_specs;

};

#endif /* OBSERVATION_H_ */
