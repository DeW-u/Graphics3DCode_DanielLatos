#version 420

layout(location=0) out vec4 vFragColor;
in vec4 vertex_color;

layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3  color; 
};

void main() {
    vFragColor = vec4(strength*color[0]*vertex_color[0], strength*color[1]*vertex_color[1], strength*color[2]*vertex_color[2], 0.0);
}
