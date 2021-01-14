#ifndef ASTEROIDCONTROLLER_H_
#define ASTEROIDCONTROLLER_H_

// #include "svpch.h"
#include "Event.h"
#include "Input.h"
#include "ScriptableEntity.h"
#include "Transform.h"
#include "Components.h"

using namespace std;
using namespace glm;

class AsteroidController : public ScriptableEntity
{
public:
	    AsteroidController() {}
// 		~AsteroidController();
protected:

    virtual void on_create() override
    {

    }

    virtual void on_destroy() override
    {
    }

    virtual void on_update(double time_delta) override
    {
		Transform &t = get_component<Transform>();
		OrbitalComponent &oc = get_component<OrbitalComponent>();

		float rot_angle = oc.rotation_speed * time_delta;

		oc.rotation_phase += rot_angle;

		quat q;

		// rotate about z axis
		q.w = cos(rot_angle/2);
		q.x = 0;
		q.y = 0;
		q.z = sin(rot_angle/2);

		t.rotation = glm::eulerAngles(quat(t.rotation) * q);
	}

    virtual void on_event(Event &e) override
	{
	}
};

#endif /* ASTEROIDCONTROLLER_H_ */
