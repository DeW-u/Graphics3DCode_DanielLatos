#version 460

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec2 a_texcoords;
layout(location=5) in vec3 a_vertex_normals;

out vec2 vertex_texcoords;
out vec3 vertex_normals_in_vs;
out vec3 vertex_coords_in_vs;

#if __VERSION__ > 410
layout(std140, binding=1) uniform Transformations {
#else
    layout(std140) uniform Transformations {
    #endif
    mat4 PVM;
    mat4 VM;
    mat3 N;
};



void main() {
    vertex_coords_in_vs = vec3(VM*a_vertex_position);
    vertex_normals_in_vs = normalize(N*a_vertex_normals);
    vertex_texcoords = a_texcoords;
    gl_Position =  PVM * a_vertex_position;
}
