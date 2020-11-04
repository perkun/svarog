#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;
uniform vec3 u_light_direction;

out vec3 normal_world;
out vec2 v_tex_coord;
out vec3 light_direction;
out float u1, u2;

float frac(float x)
{
	return x - floor(x);
}

void main()
{
	u1 = frac( tex_coord.x );
	u2 = frac( tex_coord.x + 0.5 ) -0.5;


	normal_world = vec3(u_view_matrix * u_model_matrix * vec4(normal, 0.0) );
	normal_world = normalize(normal_world);

	light_direction = normalize(vec3(u_view_matrix * vec4(u_light_direction, 0.0)));


	v_tex_coord = tex_coord;

	gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * position;
}
