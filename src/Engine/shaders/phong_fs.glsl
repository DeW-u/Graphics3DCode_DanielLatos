#version 460

layout (location = 0) out vec4 vFragColor;
in vec2 vertex_texcoords;
in vec3 vertex_normals_in_vs;

uniform sampler2D map_Kd;

#if __VERSION__ > 410
layout(std140, binding=0) uniform Modifiers {
#else
    layout(std140) uniform Color {
    #endif
    vec4  Kd;
    bool use_map_Kd;
};


void main() {
    vec3 normal = normalize(vertex_normals_in_vs);
    if (use_map_Kd == false) {
        vFragColor = Kd;
    } else {
        vFragColor = Kd*texture(map_Kd, vertex_texcoords);
    }
};