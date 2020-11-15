#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_world;
in vec2 v_tex_coord;
in vec3 light_direction;
in vec4 mesh_color;
//uniform vec4 u_color;

void main()
{
	// TODO: change normal_world name to sth else (eye_, view_, ...)
 	float d = dot(normal_world, vec3(0, 0, 1) );
//	float d = dot( normal_world, light_direction );
	d += 0.3;
	d = max(d, 0);

	color = d * mesh_color;
// 	color = vec4(1, 1, 1, 1);
}


