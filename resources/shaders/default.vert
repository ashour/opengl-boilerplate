#version 410 core

layout(location = 0) in vec3 i_position;
layout(location = 1) in vec3 i_color;

out vec3 v_color;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(i_position, 1.0);
    v_color = i_color;
}
