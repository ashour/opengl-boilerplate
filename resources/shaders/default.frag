#version 410 core

in vec3 v_color;
in vec2 v_texture_coordinate;

out vec4 o_color;

uniform sampler2D u_texture;

void main() { o_color = texture(u_texture, v_texture_coordinate) * vec4(v_color, 1.0); }
