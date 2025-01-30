#version 410 core

out vec4 o_color;

in vec3 v_frag_position;
in vec3 v_normal;
in vec2 v_uv;

struct Light
{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};
uniform DirectionalLight u_directional_light;

uniform vec3 u_view_position;

uniform sampler2D tex_diffuse_1;

vec3 phong_shading_no_specular(
    Light light, vec3 light_direction, vec3 normal, vec3 view_direction, vec3 diffuse_sample)
{
    vec3 ambient = light.ambient_color * diffuse_sample;

    float diffuse_shading = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse_color * diffuse_shading * diffuse_sample;

    return ambient + diffuse;
}

vec3 directional_light_component(DirectionalLight light,
                                 vec3 normal,
                                 vec3 view_direction,
                                 vec3 diffuse_sample)
{
    return phong_shading_no_specular(
        light.base, normalize(-light.direction), normal, view_direction, diffuse_sample);
}

void main()
{
    vec3 view_direction = normalize(u_view_position - v_frag_position);

    vec3 diffuse_sample = texture(tex_diffuse_1, v_uv).rgb;

    vec3 directional_light =
        directional_light_component(u_directional_light, v_normal, view_direction, diffuse_sample);

    o_color = vec4(directional_light, 1.0);
}
