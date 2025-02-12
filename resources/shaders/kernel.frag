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

uniform vec2 u_screen_dimensions;
vec2 texel_size = 1.0 / u_screen_dimensions;

uniform float[9] u_kernel;

void main()
{
    vec2 texture_coordinate = gl_FragCoord.xy * texel_size;

    vec3 result = vec3(0.0);

    for (int y = 0; y < 3; y += 1)
    {
        for (int x = 0; x < 3; x += 1)
        {
            vec2 offset = vec2(float(x - 1), float(y - 1)) * texel_size;
            vec3 texture_sample = texture(u_material.diffuse_1, texture_coordinate + offset).rgb;

            int kernel_index = y * 3 + x;

            result += texture_sample * u_kernel[kernel_index];
        }
    }

    o_color = vec4(result, 1.0);
}
