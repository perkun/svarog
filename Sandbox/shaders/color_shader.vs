#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;
// layout(location = 2) in vec2 tex_coord;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;
uniform vec3 u_light_direction;

out vec3 normal_world;
out vec2 v_tex_coord;
out vec3 light_direction;
out vec4 mesh_color;

// out VS_OUT {
//     vec2 texCoords;
// } vs_out;

void main()
{
	mesh_color = color;


	normal_world = vec3(u_view_matrix * u_model_matrix * vec4(normal, 0.0) );
	normal_world = normalize(normal_world);

	light_direction = normalize(vec3(u_view_matrix * vec4(u_light_direction, 0.0)));

	gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(position, 1.0);
}



