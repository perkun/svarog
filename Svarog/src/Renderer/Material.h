#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <iostream>
#include <stdio.h>
#include <map>
#include <glm/glm.hpp>
#include "Shader.h"

using namespace std;
using namespace glm;


enum UniformType {
	INT,
	FLOAT,
	VEC3,
	IVEC3,
	VEC4,
	IVEC4,
	MAT4
};


struct UniformData
{
	UniformType type;
	virtual void* get_data();
};

template<typename T>
struct Uniform
{
	Uniform(T d) : data(d) { }
	T get_data();
	T data;
};



class Material
{
public:
	Material();
    Material(Shader*);
	~Material();

	Shader *shader;
	void set_uniforms();
	void set_uniforms(Shader*);

	map<string, vec4> uniforms_vec4;
	map<string, vec3> uniforms_vec3;
	map<string, mat4> uniforms_mat4;
	map<string, int>  uniforms_int;
	map<string, float>  uniforms_float;

};

#endif /* MATERIAL_H_ */
