#version 410 core

out vec4 o_color;

in vec3 v_normal;
in vec3 v_position;

uniform vec3 u_camera_position;
uniform samplerCube u_skybox;

void main()
{
    vec3 I = normalize(v_position - u_camera_position);
    vec3 R = reflect(I, normalize(v_normal));
    o_color = vec4(texture(u_skybox, R).rgb, 1.0);
}
