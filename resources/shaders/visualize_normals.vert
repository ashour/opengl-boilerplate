#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

out VS_OUT { vec3 normal; }
vs_out;

uniform mat4 u_model;
uniform mat4 u_view;

void main()
{
    gl_Position = u_view * u_model * vec4(a_position, 1.0);

    mat3 normal_matrix = mat3(transpose(inverse(u_view * u_model)));
    vs_out.normal = normalize(vec3(vec4(normal_matrix * a_normal, 0.0)));
}
