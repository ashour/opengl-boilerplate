#version 410 core

out vec4 o_color;

in vec2 v_uv;

uniform sampler2D tex_diffuse_1;
uniform sampler2D tex_diffuse_2;

void main()
{
    vec4 diffuse_1 = texture(tex_diffuse_1, v_uv);
    vec4 diffuse_2 = texture(tex_diffuse_2, v_uv);

    o_color = mix(diffuse_1, diffuse_2, 0.5);
}
