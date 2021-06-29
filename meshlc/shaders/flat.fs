#version 330 core
layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec3 normal_world;
in vec2 v_tex_coord;

uniform vec4 u_color;
uniform int u_entity_id;

void main()
{
	color = u_color;
	color2 = u_entity_id;
}


