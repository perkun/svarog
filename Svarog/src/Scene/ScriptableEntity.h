#ifndef SCRIPTABLEENTITY_H_
#define SCRIPTABLEENTITY_H_

#include "Entity.h"
#include "Event.h"

using namespace std;

class ScriptableEntity
{
public:
	virtual ~ScriptableEntity() {}

	template<typename T>
	T& get_component()
	{
		return entity.get_component<T>();
	}

	template<typename T>
	bool has_component()
	{
		return entity.has_component<T>();
	}

	virtual void on_event(Event &e) {}
protected:
	virtual void on_create() {}
	virtual void on_update(double time_delta) {}
	virtual void on_destroy() {}

	Entity entity;
private:

	friend class Scene;

};

#endif /* SCRIPTABLEENTITY_H_ */
