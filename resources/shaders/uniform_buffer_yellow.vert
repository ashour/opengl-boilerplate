#version 410 core

layout(location = 0) in vec3 a_pos;

layout(std140) uniform Matrices
{
    mat4 u_projection;
    mat4 u_view;
};

uniform mat4 u_model;

void main() { gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0); }
