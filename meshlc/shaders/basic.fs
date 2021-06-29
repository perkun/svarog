#version 330 core
layout(location = 0) out vec4 color;

in vec3 normal_world;
in vec2 v_tex_coord;

uniform int u_has_texture;
uniform sampler2D u_texture;

void main()
{

//  // Lambert's cosine law
//  float lambertian = max(dot(normal_world, vec3(0,0,1)), 0.0);
//  float specular = 0.0;
//  if(lambertian > 0.0) {
//    vec3 R = reflect(-L, N);      // Reflected light vector
//    vec3 V = normalize(-vertPos); // Vector to viewer
//    // Compute the specular term
//    float specAngle = max(dot(R, V), 0.0);
//    specular = pow(specAngle, shininessVal);
//  }
//
//  float Ka = 0.33;
//  float Kd = 0.46;
//  float Ks = 0.19;
//
//  float d = Ka * ambientColor +
//            Kd * lambertian * diffuseColor +
//            Ks * specular * specularColor;



	float d = dot( normal_world, vec3(0, 0, 1) );
	d = max(d, 0);

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



