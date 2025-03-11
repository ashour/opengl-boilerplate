#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out VS_OUT
{
    vec3 frag_position;
    vec2 uv;
    vec3 normal;
    vec4 frag_position_light_space;
}
vs_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_light_space_matrix;

void main()
{
    vs_out.uv = a_uv;
    vs_out.normal = transpose(inverse(mat3(u_model))) * a_normal;
    vs_out.frag_position = vec3(u_model * vec4(a_position, 1.0));
    vs_out.frag_position_light_space = u_light_space_matrix * vec4(vs_out.frag_position, 1.0);
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
