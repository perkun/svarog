#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

uniform float u_displacement_factor;

uniform sampler3D u_texture;
uniform float u_texture_z;

// out vec3 normal_world;
// out vec2 v_tex_coord;
// out float u1, u2;

// uniform sampler2D u_texture;

float frac(float x)
{
	return x - floor(x);
}

void main()
{
// 	float u1 = frac( tex_coord.x );
// 	float u2 = frac( tex_coord.x + 0.5 ) -0.5;
// 	vec2 v_tex_coord = tex_coord;
// 	// fix texture seam
// 	if ( fwidth(u1) <= fwidth(u2) )
// 		v_tex_coord.x = u1;
// 	else
// 		v_tex_coord.x = u2;

	float displacement = texture(u_texture, vec3(tex_coord.x, tex_coord.y, u_texture_z)).r;

	vec3 pos = position + normalize(position) * displacement * u_displacement_factor;

	pos /= (u_displacement_factor + 1.0);

// 	vec3 pos = position * u_displacement_factor;

	gl_Position = vec4(pos, 1.0);

// 	normal_world = vec3(u_view_matrix * u_model_matrix * vec4(normal, 0.0) );
// 	normal_world = normalize(normal_world);
}

