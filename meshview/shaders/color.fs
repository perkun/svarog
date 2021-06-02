#version 330 core
layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec3 normal_world;
in vec2 v_tex_coord;

uniform int u_entity_id;
uniform vec4 u_color;

void main()
{
	float d = dot( normal_world, vec3(0, 0, 1) );
	d = max(d, 0);

	color = vec4(u_color.xyz * d, u_color.w);
	color2 = u_entity_id;
}

