#version 410 core

layout(location = 0) in vec3 a_pos;
layout(location = 2) in vec2 a_uv;

out vec2 v_uv;

void main()
{
    gl_Position = vec4(a_pos.x, a_pos.y, 0.0, 1.0);
    v_uv = a_uv;
}
