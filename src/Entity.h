#ifndef ENTITY_H_
#define ENTITY_H_

#include <iostream>
#include <stdio.h>
#include "Scene.h"

#include "Renderer.h"


#include "vendor/entt/entt.hpp"

using namespace std;


struct Tag
{
	Tag(string tag) : tag(tag) {}
	string tag;
};


class Entity
{
public:
    Entity(entt::entity handle, Scene* scene);
	~Entity();

	void add_child(Entity*);
	void detatch();

	template<typename T>
	bool has_component() {
		return scene->registry.has<T>(entity_handle);
	}

	template<typename T, typename...  Args>
	T& add_component(Args&&... args)
	{
		return scene->registry.emplace<T>(entity_handle, std::forward<Args>(args)...);
	}

	template<typename T>
	T& get_component()
	{
		if (!has_component<T>())
			cout << "Entity does not have this component" << endl;
		return scene->registry.get<T>(entity_handle);
	}


	template<typename T>
	T& remove_component()
	{
		if (!has_component<T>())
			cout << "Entity does not have this component" << endl;
		scene->registry.remove<T>(entity_handle);
	}

	template<typename T>
	void replace_component(T new_component)
	{
		scene->registry.replace<T>(entity_handle, new_component);
	}

	operator bool() const { return entity_handle != entt::null; }

	bool operator==(const Entity& other) const
	{
		return entity_handle == other.entity_handle && scene == other.scene;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}


	Entity *parent = NULL;
	vector<Entity*> children;
private:
	entt::entity entity_handle{ entt::null };
	Scene *scene = NULL;

};

#endif /* ENTITY_H_ */
