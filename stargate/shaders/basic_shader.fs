#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_world;
in vec2 v_tex_coord;
// in float u1, u2;

uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
	vec2 tex_coord = v_tex_coord;
	// fix texture seam
// 	if ( fwidth(u1) <= fwidth(u2) )
// 		tex_coord.x = u1;
// 	else
// 		tex_coord.x = u2;


	float d = dot( normal_world, vec3(0, 0, 1) );
	d = max(d, 0);

 	color = d * texture2D(u_texture, tex_coord);
}



