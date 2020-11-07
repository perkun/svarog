#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_world;
in vec2 v_tex_coord;
in vec3 light_direction;
in float u1, u2;

uniform vec4 u_color;
//uniform int u_scattering ;

void main()
{
	vec2 tex_coord = v_tex_coord;


 	float d = dot( normal_world, vec3(0, 0, 1) );
//	float d = dot( normal_world, light_direction );
	d += 0.3;
	d = max(d, 0);

	//if ( u_scattering == 0)
		//color = u_color;
	//else
		color = d*d * u_color;

}


