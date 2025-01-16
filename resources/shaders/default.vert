#version 410 core

layout(location = 0) in vec3 i_position;
layout(location = 1) in vec3 i_color;

out vec3 v_color;

void main()
{
    gl_Position = vec4(i_position.x, i_position.y, i_position.z, 1.0);
    v_color = i_color;
}
