#version 410 core

layout(location = 0) in vec3 a_position;

out vec3 v_uv;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    v_uv = a_position;
    vec4 clip_space_position = u_projection * u_view * vec4(a_position, 1.0);
    gl_Position = clip_space_position.xyww;
}
