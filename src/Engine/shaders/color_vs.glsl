#version 460

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec2 a_texcoords;

out vec2 vertex_texcoords;

#if __VERSION__ > 410
layout(std140, binding=1) uniform Transformations {
#else
    layout(std140) uniform Transformations {
    #endif
    mat4 PVM;
};



void main() {
    vertex_texcoords = a_texcoords;
    gl_Position =  PVM * a_vertex_position;
}
