#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec3 v_normal;
out vec2 v_uv;
out vec3 v_frag_position;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    v_uv = a_uv;
    v_normal = a_normal;
    v_frag_position = a_position;
    gl_Position = u_projection * u_view * vec4(a_position, 1.0);
}
