#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <iostream>
#include <stdio.h>
#include "VertexArrayObject.h"
#include "Texture.h"
#include "Shader.h"

using namespace std;

class MeshComponent
{
public:
    MeshComponent(VertexArrayObject*);
	~MeshComponent();

	VertexArrayObject *vao = NULL;
};


class TextureComponent
{
public:
	TextureComponent(Texture*);
	~TextureComponent();

	Texture *texture;
};


class ShaderComponent
{
public:
	ShaderComponent(Shader*);
	~ShaderComponent();

	Shader *shader;

};

class SceneStatus
{
public:
	SceneStatus();
    SceneStatus(bool, bool r = true);
	~SceneStatus();

	bool active;
	bool render = true;
};



#endif /* SCENE/COMPONENTS_H_ */
