#version 410 core

out vec4 o_color;

in vec2 v_uv;

uniform sampler2D u_tex_diffuse_1;
uniform sampler2D u_tex_specular_1;
struct Material
{
    sampler2D diffuse_1;
    sampler2D specular_1;
    float shininess;
};
uniform Material u_material;

const float MIN_TEXTURE_SCALE = 0.0001;
uniform float u_texture_scale = 1.0;

void main()
{
    float inverted_texture_scale = 1.0 / max(u_texture_scale, MIN_TEXTURE_SCALE);
    vec2 scaled_uv = v_uv * inverted_texture_scale;
    vec3 diffuse_sample = texture(u_material.diffuse_1, scaled_uv).rgb;

    float average = (diffuse_sample.r + diffuse_sample.g + diffuse_sample.b) / 3.0;

    o_color = vec4(average, average, average, 1.0);
}
