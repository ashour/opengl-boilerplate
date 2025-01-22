#version 410 core

layout(location = 0) in vec3 i_position;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec3 i_color;
layout(location = 3) in vec2 i_texture_coordinate;

out vec3 v_normal;
out vec3 v_color;
out vec2 v_texture_coordinate;

out vec3 v_frag_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(i_position, 1.0);
    v_frag_position = vec3(u_model * vec4(i_position, 1.0));
    v_normal = mat3(transpose(inverse(u_model))) * i_normal;
    v_color = i_color;
    v_texture_coordinate = i_texture_coordinate;
}
