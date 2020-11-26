#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_eye;

uniform sampler2D u_texture;

void main()
{
	float d = dot(normal_eye, vec3(0, 0, 1) );
	d = max(d, 0);
 	color = vec4(vec3(d), 1.0);
}

