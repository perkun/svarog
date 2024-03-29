#include "svpch.h"
#include "Renderer.h"
#include "Core.h"


namespace Renderer {

// extern variables definitions
extern int flags;

// private:
bool blend = false;
Material scene_material;
// vec4 clear_color = vec4(0., 0., 0., 1.);
////


void init()
{
	glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glPointSize(10.0f);

    // 	glEnable(GL_BLEND);
    // 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void set_line_width(float width)
{
	glLineWidth(width);
}

void enable_blend()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void disable_blend()
{
	glDisable(GL_BLEND);
}

void set_blend(bool mesh_blend)
{
	if (mesh_blend)
	{
		if (!blend)
		{
			enable_blend();
			blend = true;
		}
	}
	else
	{
		if (blend)
		{
			disable_blend();
			blend = false;
		}
	}
}


void begin_scene(EditorCamera &camera)
{
    scene_material.uniforms_mat4["u_view_matrix"] = camera.get_view();
    scene_material.uniforms_mat4["u_perspective_matrix"] =
        camera.get_perspective();
}


void begin_scene(const shared_ptr<Camera> &camera)
{
    scene_material.uniforms_mat4["u_view_matrix"] = camera->get_view();
    scene_material.uniforms_mat4["u_perspective_matrix"] =
        camera->get_perspective();
}

void set_dir_light(const shared_ptr<Camera> lc)
{
	scene_material.uniforms_mat4["u_light_perspective_matrix"] = lc->get_perspective();
	scene_material.uniforms_mat4["u_light_view_matrix"] = lc->get_view();
	scene_material.uniforms_vec3["u_light_position"] = lc->position;
	scene_material.uniforms_int["u_depth_map"] = 1;
}


void end_scene()
{
}

void submit(const shared_ptr<VertexArrayObject> &vao, Material &material)
{
	material.set_uniforms();
	scene_material.shader = material.shader;
	scene_material.set_uniforms();

	set_blend(vao->blend);

	glBindVertexArray(vao->vao_id);
	glDrawElements(vao->draw_type, vao->num_draw_elements, GL_UNSIGNED_INT, NULL);
}


void draw(shared_ptr<VertexArrayObject> vao, Shader *shader)
{
	shader->bind();
	set_blend(vao->blend);

	glBindVertexArray(vao->vao_id);
	glDrawElements(vao->draw_type, vao->num_draw_elements, GL_UNSIGNED_INT, NULL);
}

void clear()
{
// 	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b ,a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clear(vec4 color)
{
	glClearColor(color.r, color.g, color.b ,color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void set_viewport(int x, int y, int size_x,
                  int size_y)
{
	glViewport(x, y, size_x, size_y);
}


void bind_default_framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Renderer
