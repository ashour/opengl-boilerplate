#version 410 core

out vec4 o_color;

uniform vec4 u_color = vec4(0.8, 0.5, 0.2, 1.0);

void main() { o_color = u_color; }
