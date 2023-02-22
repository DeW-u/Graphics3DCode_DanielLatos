#version 460

layout (location = 0) out vec4 vFragColor;
in vec2 vertex_texcoords;

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
    if (int(use_map_Kd) == 0) {
        vFragColor = Kd;
    } else if (int(use_map_Kd) == 1) {
        vFragColor = Kd*texture(map_Kd, vertex_texcoords);
    }
};