#ifndef SCENESERIALIZER_H_
#define SCENESERIALIZER_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <yaml-cpp/yaml.h>
#include <fstream>
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
	Scene *scene;

};

#endif /* SCENESERIALIZER_H_ */
