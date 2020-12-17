#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include "svpch.h"
#include "VertexArrayObject.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "ScriptableEntity.h"
#include "Entity.h"

using namespace std;


struct TagComponent
{
    TagComponent(string tag) : tag(tag)
    {
    }
    string tag;
};



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

class SceneGraphComponent
{
public:
	Entity parent = Entity();
	vector<Entity> children;
};


class CameraComponent
{
public:
	CameraComponent(Camera*);
	~CameraComponent();

	Camera *camera;
};

class FramebufferComponent
{
public:
	FramebufferComponent(Framebuffer*);
	~FramebufferComponent();

	Framebuffer *framebuffer;
};

class NativeScriptComponent
{
public:
    ScriptableEntity *instance = NULL;

    ScriptableEntity *(*instantiate_script)();
    void (*destroy_script)(NativeScriptComponent *);

    template <typename T> void bind()
    {
        instantiate_script = []() {
            return static_cast<ScriptableEntity *>(new T());
        };
        destroy_script = [](NativeScriptComponent *nsc) {
            delete nsc->instance;
            nsc->instance = NULL;
        };
    }
};

#endif /* SCENE/COMPONENTS_H_ */
