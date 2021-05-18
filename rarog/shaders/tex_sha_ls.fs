#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_eye;
in vec2 v_tex_coord;

in vec3 light_position_eye;
in vec3 vertex_position_eye;
in vec4 vertex_position_light;

uniform int u_has_texture;

uniform sampler2D u_texture;
uniform sampler2D u_depth_map;


float evaluate_shadow(vec4 vertex_position_light)
{
	float epsilon = 0.001;

	float shadow = texture(u_depth_map, vertex_position_light.xy).r;
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

	vec3 light_direction = normalize(light_position_eye - vertex_position_eye);

	float mi_0 = dot(normal_eye, light_direction );
	mi_0 = max(mi_0, 0);

	float mi = dot(normal_eye, vec3(0., 0., 1.));
	mi = max(mi, 0);

	float LSL = 0.1;
	float flux = (1.0 - LSL) * mi_0 / (mi_0 + mi);
	flux += LSL*mi_0;
	flux *= shadow_factor;
	flux = clamp(flux, 0.0, 1.0);

	if (u_has_texture == 1)
	{
		color = texture(u_texture, v_tex_coord);
		color = vec4( color.xyz * flux, color.w);
	}
	else if (u_has_texture == 0)
	{
 		color = vec4(flux, flux, flux, 1.0);
	}
}

