#ifndef OBSERVATION_H_
#define OBSERVATION_H_

#include "Material.h"
#include "Components.h"
#include "IndexedModel.h"
#include "Application.h"
#include "Transform.h"
#include "Entity.h"
#include "FitsHeader.h"

using namespace std;

class Observation
{
public:
	Observation(Entity& target, Entity &observer);
	virtual ~Observation();
	void add_ghosts(Entity& ghost_target, Entity &ghost_observer, string shader_name);


	virtual void serialize(YAML::Emitter &out, string storage_name, int id = 0,
						   string filename = "") = 0;

	Entity target, observer, ghost_target, ghost_observer;
	Transform target_transform;
	Transform observer_transform;
	OrbitalComponent target_orbital_component;

	double julian_day;

	vec4 ghost_color = vec4(1,1,1,1);

	virtual string get_obs_type_string() = 0;

protected:

};


#endif /* OBSERVATION_H_ */
