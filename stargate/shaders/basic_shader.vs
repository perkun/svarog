#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

out vec3 normal_world;
out vec2 v_tex_coord;
// out float u1, u2;

float frac(float x)
{
	return x - floor(x);
}

void main()
{
// 	u1 = frac( tex_coord.x );
// 	u2 = frac( tex_coord.x + 0.5 ) -0.5;


	normal_world = vec3(u_view_matrix * u_model_matrix * vec4(normal, 0.0) );
	normal_world = normalize(normal_world);

	v_tex_coord = tex_coord;

	gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * position;
}


