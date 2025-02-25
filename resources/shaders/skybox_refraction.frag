#version 410 core

out vec4 o_color;

in vec3 v_normal;
in vec3 v_position;

uniform vec3 u_camera_position;
uniform samplerCube u_skybox;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(v_position - u_camera_position);
    vec3 R = refract(I, normalize(v_normal), ratio);
    o_color = vec4(texture(u_skybox, R).rgb, 1.0);
}
