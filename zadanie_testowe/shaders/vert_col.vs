#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

out vec4 v_color;

void main()
{
	v_color = color;

	gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix * position;
}



