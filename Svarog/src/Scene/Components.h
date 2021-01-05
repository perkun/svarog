#ifndef COMPONENTS_H_
#define COMPONENTS_H_

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
    MeshComponent(shared_ptr<VertexArrayObject>);
	~MeshComponent();

	shared_ptr<VertexArrayObject> vao;
};


class TextureComponent
{
public:
	TextureComponent(shared_ptr<Texture>);
	~TextureComponent();

	shared_ptr<Texture> texture;
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
	CameraComponent(shared_ptr<Camera>);
	~CameraComponent();

	shared_ptr<Camera> camera;
};

class FramebufferComponent
{
public:
	FramebufferComponent(shared_ptr<Framebuffer>);
	~FramebufferComponent();

	shared_ptr<Framebuffer> framebuffer;
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

class LightComponent
{
public:
	LightComponent() {}
	~LightComponent() {}

	glm::vec4 color = vec4(1.);
};


#endif /* SCENE/COMPONENTS_H_ */
