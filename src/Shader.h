#ifndef SHADER_H_
#define SHADER_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <map>


using namespace std;
using namespace glm;

class Shader
{
public:
	Shader();
	~Shader();

	void bind();
	void unbind();
	void set_uniform_4fv(string name, vec4 data);

private:
	int get_uniform_location(string name);
	unsigned int compile_shader(unsigned int type, const string &source);
	void create_shader(const string &vertex_shader,
							   const string &fragment_shader);

	unsigned int program;

	map<string, int> uniform_lication_cache;

};

#endif /* SHADER_H_ */
