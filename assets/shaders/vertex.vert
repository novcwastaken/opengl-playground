#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertex_color;
uniform float horizontal_offset = 0.5;

void main() {
    gl_Position = vec4(aPos.x + horizontal_offset, aPos.yz, 1.0);
    vertex_color = aColor;
}