#version 330 core
layout(location = 0) out vec4 color;

in vec3 gs_normal_world;
in vec2 gs_v_tex_coord;
in vec3 gs_light_direction;
in vec4 gs_mesh_color;
//uniform vec4 u_color;

void main()
{
	// TODO: change normal_world name to sth else (eye_, view_, ...)
 	float d = abs(dot(gs_normal_world, vec3(0, 0, 1) ));
//	float d = dot( normal_world, light_direction );
	d += 0.3;
	d = max(d, 0);

// 	if (gl_FrontFacing)
		color = vec4(d * vec3(gs_mesh_color), 1.0);
// 	else
// 		color = vec4(1, 0, 0, 1);


// 	color = vec4(1, 1, 1, 1);
}


