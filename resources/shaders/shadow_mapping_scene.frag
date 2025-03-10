#version 410 core

out vec4 o_color;

in vec3 v_frag_position;
in vec3 v_normal;
in vec3 v_color;
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

vec3 phong_shading(Light light,
                   vec3 light_direction,
                   vec3 normal,
                   vec3 view_direction,
                   vec3 diffuse_sample,
                   vec3 specular_sample,
                   float shininess)
{
    vec3 ambient = light.ambient_color * diffuse_sample;

    float diffuse_shading = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse_color * diffuse_shading * diffuse_sample;

    vec3 reflection_direction = reflect(-light_direction, normal);
    float specular_shading = pow(max(dot(view_direction, reflection_direction), 0.0), shininess);
    vec3 specular = light.specular_color * specular_sample * specular_shading;

    return ambient + diffuse + specular;
}

float attentuation(
    vec3 light_position, vec3 frag_position, float constant, float linear, float quadratic)
{
    float distance = length(light_position - frag_position);
    return 1.0 / (constant + linear * distance + quadratic * distance * distance);
}

vec3 directional_light_component(DirectionalLight light,
                                 vec3 normal,
                                 vec3 view_direction,
                                 vec3 diffuse_sample,
                                 vec3 specular_sample,
                                 float shininess)
{
    return phong_shading(light.base,
                         normalize(-light.direction),
                         normal,
                         view_direction,
                         diffuse_sample,
                         specular_sample,
                         shininess);
}

void main()
{
    vec3 view_direction = normalize(u_view_position - v_frag_position);

    // stretch the uvs: as the uvs expand, the texture effectively
    // shrinks, and vice versa.
    float inverted_texture_scale = 1.0 / max(u_texture_scale, MIN_TEXTURE_SCALE);
    vec2 scaled_uv = v_uv * inverted_texture_scale;
    vec3 diffuse_sample = texture(u_material.diffuse_1, scaled_uv).rgb;
    vec3 specular_sample = texture(u_material.specular_1, scaled_uv).rgb;

    vec3 lit_diffuse_sample = directional_light_component(
        u_directional_light, v_normal, view_direction, diffuse_sample, specular_sample, 32);

    o_color = vec4(lit_diffuse_sample, 1.0);
}
