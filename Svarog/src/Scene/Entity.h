#ifndef ENTITY_H_
#define ENTITY_H_

#include "entt.hpp"
#include "Core.h"

using namespace std;


class Entity
{
public:
    Entity() : entity_handle(entt::null), scene_registry_handle(NULL)
    {
    }
    Entity(entt::entity handle, entt::registry *registry_handle)
        : entity_handle(handle), scene_registry_handle(registry_handle)
    {
    }


    ~Entity();

    void add_child(Entity&);
    void detatch();
	void destroy();
	Entity& get_parent();
	vector<Entity>& get_children();

    template <typename T>
	bool has_component()
    {
        return scene_registry_handle->has<T>(entity_handle);
    }

    template <typename T, typename... Args>
	T &add_component(Args &&... args)
    {
        return scene_registry_handle->emplace<T>(entity_handle,
                                                 std::forward<Args>(args)...);
    }

    template <typename T>
	T &get_component()
    {
        if (!has_component<T>())
		{
            CORE_ERROR("Entity does not have this component");
		}
        return scene_registry_handle->get<T>(entity_handle);
    }

    template <typename T>
	void remove_component()
    {
        if (!has_component<T>())
            cout << "Entity does not have this component" << endl;
        scene_registry_handle->remove<T>(entity_handle);
    }

    template <typename T>
	T& replace_component(T new_component)
    {
        scene_registry_handle->replace<T>(entity_handle, new_component);
        return scene_registry_handle->get<T>(entity_handle);
    }

    operator bool() const
    {
        return entity_handle != entt::null;
    }

    bool operator==(const Entity &other) const
    {
        return entity_handle == other.entity_handle &&
               scene_registry_handle == other.scene_registry_handle;
    }

    bool operator!=(const Entity &other) const
    {
        return !(*this == other);
    }

//     Entity *parent = NULL;
//     vector<Entity *> children;

	entt::entity get_handle()
	{
		return entity_handle;
	}

	uint32_t get_uuid()
	{
		return (uint32_t)entity_handle;
	}

    // 	bool active = false;
private:
    entt::entity entity_handle = entt::null;
    entt::registry *scene_registry_handle;
};

#endif /* ENTITY_H_ */
