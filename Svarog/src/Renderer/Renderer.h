#ifndef RENDERER_H_
#define RENDERER_H_

#include "Application.h"
#include "Shader.h"
#include "VertexArrayObject.h"
#include "Camera.h"
#include "EditorCamera.h"
#include "Material.h"
#include "Entity.h"

using namespace std;
using namespace glm;

enum class  Rendererflags {
	render_to_framebuffer = 0b1,
	shadow_map = 0b10
};


namespace Renderer
{
	void init();
    void draw(shared_ptr<VertexArrayObject> vao, Shader *shader);
	void clear();
	void clear(vec4);
	void clear(float, float, float, float);
	void set_viewport(int, int, int, int);
	void set_line_width(float width);
	void bind_default_framebuffer();

	void enable_blend();
	void disable_blend();

	void begin_scene(EditorCamera &camera);
	void begin_scene(const shared_ptr<Camera> &camera);
	void set_dir_light(const shared_ptr<Camera>);
	void end_scene();
	void submit(const shared_ptr<VertexArrayObject> &vao, Material &material);

	extern int flags;
};

#endif /* RENDERER_H_ */
