#version 410

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec3 a_color;

out vec3 v_color;

uniform vec2 offsets[100];

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = vec4(a_position + offset, 0.0, 1.0);
    v_color = a_color;
}
