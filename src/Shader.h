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


using namespace std;
using namespace glm;


enum type {
	INT,
	FLOAT,
	VEC3,
	IVEC3,
	VEC4,
	IVEC4,
	MAT4
};


class Uniform
{
public:
	Uniform(string name, int type, void *data)
		: name(name), type(type), data(data) {}

	void set_data(void *d) { data = d; }

	string name;
	int type;
	void* data;
};


class Shader
{
public:
	Shader(string vertex_fn, string fragment_fn);
	~Shader();

	void set_uniforms();

	void set_uniform_4fv(string name, vec4 data);
	void set_uniform_1f(string name, float value);
	void set_uniform_1i(string name, int value);
	void set_uniform_mat4f(string name, mat4 data);

	void bind();
	void unbind();

	vector<Uniform> uniforms;

private:
	int get_uniform_location(string name);
	unsigned int compile_shader(unsigned int type, const char *source);
	void create_shader(const char *vertex_shader,
					   const char *fragment_shader);

	unsigned int program;

	map<string, int> uniform_location_cache;

};

#endif /* SHADER_H_ */
