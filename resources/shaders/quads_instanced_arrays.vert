#version 410

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_offset;

out vec3 v_color;

void main()
{
    vec2 scaled_position = a_position * (gl_InstanceID / 100.0);
    gl_Position = vec4(scaled_position + a_offset, 0.0, 1.0);
    v_color = a_color;
}
