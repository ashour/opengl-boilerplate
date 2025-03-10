#version 410

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_uv;

out vec2 v_uv;

uniform mat4 u_model;

void main()
{
    v_uv = a_uv;
    gl_Position = u_model * vec4(a_position, 1.0);
}
