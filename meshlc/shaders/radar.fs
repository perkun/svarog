#version 330 core
layout(location = 0) out vec4 color;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

in vec3 normal_world;
in vec2 v_tex_coord;

in float radial_velocity;
in float depth;
//in vec4 pos;

uniform int u_has_texture;
uniform float u_factor;
uniform sampler2D u_texture;

void main()
{
	float d = dot( normal_world, vec3(0, 0, 1) );
	d = max(d, 0);

//	if (u_has_texture == 1)
//	{
//		color = texture2D(u_texture, v_tex_coord); color = vec4( color.xyz * d, color.w);
//	}
//	else if (u_has_texture == 0)
//	{
// 		color = vec4(d, d, d, 1.0);
//	}

	//vec4 pos_crf = u_view_matrix * u_model_matrix * pos;
	//float depth2 = pos_crf.z;

	// works with floating point framebuffer (GL_RGBA32F)
	//color = vec4(u_factor*radial_velocity, gl_FragCoord.z, 0., 1.);
	color = vec4(u_factor*radial_velocity, d, -depth, 1.);

}



