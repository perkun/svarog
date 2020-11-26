#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_perspective_matrix;

out vec3 normal_eye;


vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}


void main()
{
    normal_eye = vec3(u_perspective_matrix *
					  u_view_matrix *
					  u_model_matrix *
					  vec4(GetNormal(), 0.0) );

	normal_eye = normalize(normal_eye);

	gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix
					* gl_in[0].gl_Position;
	EmitVertex();

	gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix
					* gl_in[1].gl_Position;
	EmitVertex();

	gl_Position = u_perspective_matrix * u_view_matrix * u_model_matrix
					* gl_in[2].gl_Position;
	EmitVertex();
}
