#include "Shader.h"

Shader::Shader()
{
	string vs = R"(#version 450 core
layout(location = 0) in vec4 position;
layout(location = 2) in vec3 normal;

out vec3 normal_world;

void main() {
	normal_world = normal;
	gl_Position = position;
})";
//
	string fs = R"(#version 450 core
layout(location = 0) out vec4 color;
in vec3 normal_world;

uniform vec4 u_color;

void main() {
	float d = ( dot(normalize(normal_world), vec3(0, 0, 1))  );
	d = max(d, 0);
 	//color = u_color * d;
 	color = vec4(normal_world, 1.0);
})";


	create_shader(vs, fs);

}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::bind()
{
	glUseProgram(program);
}

void Shader::unbind()
{
	glUseProgram(0);
}

unsigned int Shader::compile_shader(unsigned int type, const string &source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);


	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		cout << "Failed to compile "
			 << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			 << " shader!" << endl;
		cout << message << endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::create_shader(const string &vertex_shader,
						   const string &fragment_shader)
{
	program = glCreateProgram();
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

int Shader::get_uniform_location(string name)
{
	if (uniform_lication_cache.find(name) != uniform_lication_cache.end())
		return uniform_lication_cache[name];

	int location;
	location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		cout << "The name \"" << name << "\" does not exist in shader!" << endl;

	uniform_lication_cache[name] = location;

	return location;
}

void Shader::set_uniform_4fv(string name, vec4 data)
{
	bind();
	glUniform4fv(get_uniform_location(name), 1, &data[0]);
	unbind();
}
