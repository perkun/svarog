#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_world;
in vec2 v_tex_coord;

uniform sampler2D u_texture;

void main()
{
	float d = dot( normal_world, vec3(0, 0, 1) );
	d = max(d, 0);
 	color = texture2D(u_texture, v_tex_coord);
	//tex_color *= d;
 	//color = vec4(d, d, d, 1.0);
}



