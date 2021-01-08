#ifndef ASTEROIDCONTROLLER_H_
#define ASTEROIDCONTROLLER_H_

// #include "svpch.h"
#include "Event.h"
#include "Input.h"
#include "ScriptableEntity.h"
#include "Transform.h"

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
		t.rotation.z += t.rotation_speed * time_delta;

		if (t.rotation.z < 0.)
			t.rotation.z += 2*M_PI;
		if (t.rotation.z > 2*M_PI)
			t.rotation.z -= 2*M_PI;
	}

    virtual void on_event(Event &e) override
	{
	}
};

#endif /* ASTEROIDCONTROLLER_H_ */
