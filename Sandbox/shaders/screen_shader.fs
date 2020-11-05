#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_world;

uniform vec4 u_color;
//uniform int u_scattering ;

void main()
{

	float d = dot( normal_world, vec3(0, 0, 1) );
	d = max(d, 0);


	color =   d * u_color;

}
