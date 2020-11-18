#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_eye;
in vec2 v_tex_coord;
in vec3 light_position_eye;
in vec3 vertex_position_eye;
in vec4 vertex_position_light;

// in float u1, u2;

uniform sampler2D u_texture;
uniform sampler2D u_depth_map;

// uniform vec4 u_color;

float evaluate_shadow(vec4 vertex_position_light) {
	float epsilon = 0.005;

	float shadow = texture2D(u_depth_map, vertex_position_light.xy).r;
	if (shadow + epsilon < vertex_position_light.z)						// jeżeli shadow jest dalej niż vertex_pos
		return 0.;
	return 1.;
}

void main()
{
	// shadow stuff
	vec4 vps = vertex_position_light;
	vps.xyz /= vertex_position_light.w;
	vps.xyz += 1.0;
	vps.xyz *= 0.5;
	float shadow_factor = evaluate_shadow(vps);


	vec2 tex_coord = v_tex_coord;
	// fix texture seam
// 	if ( fwidth(u1) <= fwidth(u2) )
// 		tex_coord.x = u1;
// 	else
// 		tex_coord.x = u2;

	vec3 light_direction = normalize(light_position_eye - vertex_position_eye);


// 	float d = dot(normal_eye, vec3(0., 0., 1.) );
	float d = dot(normal_eye, light_direction );
// 	d = clamp(d + 0.3, 0., 1.);
	d = clamp(d, 0., 1.);
 	color = texture2D(u_texture, tex_coord);
	color = vec4(d * shadow_factor * vec3(color), color.w);
}



