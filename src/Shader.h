#ifndef SHADER_H_
#define SHADER_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <map>

#include "Material.h"

using namespace std;
using namespace glm;

class Shader
{
public:
	Shader(string vertex_fn, string fragment_fn);
	Shader(const Shader &other);
	~Shader();

// 	void set_uniforms(map<string, UniformData>&);
	void set_uniforms(Material&);

	void set_uniform_3fv(string name, vec3 data);
	void set_uniform_4fv(string name, vec4 data);
	void set_uniform_1f(string name, float data);
	void set_uniform_1i(string name, int data);
	void set_uniform_mat4f(string name, mat4 data);

	void bind();
	void unbind();

	void destroy();


	unsigned int program;
private:
	int get_uniform_location(string name);
	unsigned int compile_shader(unsigned int type, const char *source);
	void create_shader(const char *vertex_shader,
					   const char *fragment_shader);


	map<string, int> uniform_location_cache;

};

#endif /* SHADER_H_ */
