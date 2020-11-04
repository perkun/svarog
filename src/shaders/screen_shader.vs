#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

uniform vec4 u_screen_pos;

out vec3 normal_world;

float frac(float x)
{
	return x - floor(x);
}

void main()
{

	//normal_world = vec3(u_view_matrix * u_model_matrix * vec4(normal, 0.0) );
// 	normal_world = vec3(u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(normal, 0.0) );

	//gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * vec4(vec3(position), 0.)


	mat4 model = u_model_matrix;
	mat4 view = u_view_matrix;
	view[3][0] = u_screen_pos.x;
	view[3][1] = u_screen_pos.y;
	view[3][2] = u_screen_pos.z;
// 	view[3][3] = 1.;


// 	model[3][0] = 0.;
// 	model[3][1] = 0.;
// 	model[3][2] = 0.;
// 	model[3][3] = 1.;


	normal_world = vec3(view * model *vec4(normal, 0.0) );
	normal_world = normalize(normal_world);
	gl_Position = u_perspective_matrix * view * model *  position;
}
