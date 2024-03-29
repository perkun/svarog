#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_world;
in vec2 v_tex_coord;

uniform int u_has_texture;
uniform sampler2D u_texture;

void main()
{
	float d = dot( normal_world, vec3(0, 0, 1) );
	d = max(d, 0);

	if (u_has_texture == 1)
	{
		color = texture2D(u_texture, v_tex_coord);
		color = vec4( color.xyz * d, color.w);
	}
	else if (u_has_texture == 0)
	{
 		color = vec4(d, d, d, 1.0);
	}
}



