#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec3 v_normal;
out vec3 v_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    v_normal = mat3(transpose(inverse(u_model))) * a_normal;
    v_position = vec3(u_model * vec4(a_position, 1.0));
    gl_Position = u_projection * u_view * vec4(v_position, 1.0);
}
