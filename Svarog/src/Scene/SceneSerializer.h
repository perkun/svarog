#ifndef SCENESERIALIZER_H_
#define SCENESERIALIZER_H_

#include "Scene.h"

using namespace std;

class SceneSerializer
{
public:
    SceneSerializer(Scene *sc);
	~SceneSerializer();

	void serialize(const string filepath);
	bool deserialize(const string filepath);

private:
	Entity deserialize_entity(YAML::detail::iterator_value);
	Scene *scene;

};

#endif /* SCENESERIALIZER_H_ */
