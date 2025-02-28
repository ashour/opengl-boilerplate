#version 410 core

out vec4 o_color;

in vec3 f_color;

void main() { o_color = vec4(f_color, 1.0); }
