#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_world;
in vec2 v_tex_coord;

in vec3 light_position_world;
in vec3 vertex_position_world;
in vec4 vertex_position_light;

uniform int u_has_texture;
uniform sampler2D u_texture;
uniform sampler2D u_depth_map;


float evaluate_shadow(vec4 vertex_position_light)
{
	float epsilon = 0.05;

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

	vec3 light_direction = normalize(light_position_world - vertex_position_world);

	float d = dot( normal_world, light_direction );
	d = max(d, 0);
	d = d * clamp(shadow_factor, 0.0, 1.0);

	if (u_has_texture == 1)
	{
		color = texture2D(u_texture, v_tex_coord);
		color = vec4( color.xyz * d, color.w);
	}
	else if (u_has_texture == 0)
	{
 		color = vec4(d, d, d, 1.0);
	}
}

