#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

// in VS_OUT {
//     vec2 texCoords;
// } gs_in[];

// out vec2 TexCoords;

uniform float time;


in vec3 normal_world[];
in vec2 v_tex_coord[];
in vec3 light_direction[];
in vec4 mesh_color[];


out vec3 gs_normal_world;
out vec2 gs_v_tex_coord;
out vec3 gs_light_direction;
out vec4 gs_mesh_color;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 5.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}


vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}


void main() {
    vec3 normal = GetNormal();
 	gs_normal_world = normal_world[0];
// 	gs_normal_world = normal;
	gs_mesh_color = mesh_color[0];


    gl_Position = explode(gl_in[0].gl_Position, normal);
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    EmitVertex();

    EndPrimitive();

    gl_Position = explode(gl_in[1].gl_Position + 2, normal);
    EmitVertex();
    gl_Position = explode(gl_in[0].gl_Position + 2, normal);
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position + 2, normal);
    EmitVertex();
//
    EndPrimitive();
}
