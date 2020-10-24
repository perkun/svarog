#include "Shader.h"

Shader::Shader(string vertex_fn, string fragment_fn)
{

	FILE *f = fopen(vertex_fn.c_str(), "rb");
	if (f == NULL)
	{
		cout << "could not open vertex shader file: " << vertex_fn << "\n";
		return;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

	char *vertex_shader = (char*)malloc(fsize + 1);
	fread(vertex_shader, 1, fsize, f);
	vertex_shader[fsize] = 0;
	fclose(f);


	f = fopen(fragment_fn.c_str(), "rb");
	if (f == NULL)
	{
		cout << "could not open fragment shader file: " << fragment_fn << "\n";
		return;
	}

	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

	char *fragment_shader = (char*)malloc(fsize + 1);
	fread(fragment_shader, 1, fsize, f);
	fragment_shader[fsize] = 0;
	fclose(f);

	create_shader(vertex_shader, fragment_shader);

	free(vertex_shader);
	free(fragment_shader);
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

unsigned int Shader::compile_shader(unsigned int type, const char *source)
{
	unsigned int id = glCreateShader(type);
// 	const char *src = source.c_str();
	glShaderSource(id, 1, &source, NULL);
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

void Shader::create_shader(const char *vertex_shader,
						   const char *fragment_shader)
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
	if (uniform_location_cache.find(name) != uniform_location_cache.end())
		return uniform_location_cache[name];

	int location;
	location = glGetUniformLocation(program, name.c_str());
	if (location == -1)
		cout << "The name \"" << name << "\" does not exist in shader!" << endl;

	uniform_location_cache[name] = location;

	return location;
}

void Shader::set_uniforms()
{
// 	bind();
// 	for (Uniform uni: uniforms)
// 	{
// 		if (uni.type == INT)
// 			set_uniform_1i(uni.name, *((int*)uni.data));
// 		else if (uni.type == FLOAT)
// 			set_uniform_1f(uni.name, *((float*)uni.data));
// 		else if (uni.type == VEC4)
// 			set_uniform_4fv(uni.name, *((vec4*)uni.data));
// 		else if (uni.type == MAT4)
// 			set_uniform_mat4f(uni.name, *((mat4*)uni.data));
// 	}

}

void Shader::set_uniform_4fv(string name, vec4 data)
{
	bind();
	glUniform4fv(get_uniform_location(name), 1, &data[0]);
// 	unbind();
}


void Shader::set_uniform_mat4f(string name, mat4 data)
{
	bind();
	glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &data[0][0]);
// 	unbind();
}

void Shader::set_uniform_1i(string name, int value)
{
	bind();
	glUniform1i(get_uniform_location(name), value);
}
