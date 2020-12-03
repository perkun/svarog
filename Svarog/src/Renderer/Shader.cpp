#include "Shader.h"

Shader::Shader()
{
}

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
    fseek(f, 0, SEEK_SET); /* same as rewind(f); */

    char *vertex_shader_source = (char *)malloc(fsize + 1);
    fread(vertex_shader_source, 1, fsize, f);
    vertex_shader_source[fsize] = 0;
    fclose(f);

    f = fopen(fragment_fn.c_str(), "rb");
    if (f == NULL)
    {
        cout << "could not open fragment shader file: " << fragment_fn << "\n";
        return;
    }

    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET); /* same as rewind(f); */

    char *fragment_shader_source = (char *)malloc(fsize + 1);
    fread(fragment_shader_source, 1, fsize, f);
    fragment_shader_source[fsize] = 0;
    fclose(f);

    create_shader(vertex_shader_source, fragment_shader_source);

    free(vertex_shader_source);
    free(fragment_shader_source);
}


Shader::Shader(const Shader &other)
{
	program = other.program;
}


Shader::~Shader()
{
	destroy();
}

void Shader::destroy()
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
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "Failed to compile "
             << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!"
             << endl;
        cout << message << endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

void Shader::create_shader(const char *vertex_shader_source,
                           const char *fragment_shader_source)
{
    program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
}


void Shader::create_shader(const char *vertex_shader_source,
						   const char *geometry_shader_source,
						   const char *fragment_shader_source)
{
    program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    unsigned int gs = compile_shader(GL_GEOMETRY_SHADER, geometry_shader_source);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(gs);
    glDeleteShader(fs);
}


int Shader::get_uniform_location(string name)
{
    if (uniform_location_cache.find(name) != uniform_location_cache.end())
        return uniform_location_cache[name];

    int location;
    location = glGetUniformLocation(program, name.c_str());
//     if (location == -1)
// 	{
// 		WARN("The name {} does not exist in shader", name);
// 	}

    uniform_location_cache[name] = location;

    return location;
}
void Shader::set_uniform_3fv(string name, vec3 data)
{
//     bind();
    glUniform3fv(get_uniform_location(name), 1, &data[0]);
    // 	unbind();
}

void Shader::set_uniform_4fv(string name, vec4 data)
{
//     bind();
    glUniform4fv(get_uniform_location(name), 1, &data[0]);
    // 	unbind();
}

void Shader::set_uniform_mat4f(string name, mat4 data)
{
//     bind();
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &data[0][0]);
    // 	unbind();
}

void Shader::set_uniform_1f(string name, float data)
{
//     bind();
    glUniform1f(get_uniform_location(name), data);
}

void Shader::set_uniform_1i(string name, int data)
{
//     bind();
    glUniform1i(get_uniform_location(name), data);
}


