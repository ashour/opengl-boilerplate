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

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight u_point_light;

struct SpotLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 direction;
    float inner_cutoff;
    float outer_cutoff;
};
uniform SpotLight u_spot_light;

uniform vec3 u_view_position;

uniform sampler2D u_diffuse_texture;
uniform sampler2D u_specular_texture;
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
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

vec3 point_light_component(PointLight light,
                           vec3 frag_position,
                           vec3 normal,
                           vec3 view_direction,
                           vec3 diffuse_sample,
                           vec3 specular_sample,
                           float shininess)
{
    vec3 light_direction = normalize(light.position - frag_position);

    float distance = length(light.position - v_frag_position);
    float attentuation =
        1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return attentuation * phong_shading(light.base,
                                        light_direction,
                                        normal,
                                        view_direction,
                                        diffuse_sample,
                                        specular_sample,
                                        shininess);
}

vec3 spot_light_component(SpotLight light,
                          vec3 frag_position,
                          vec3 normal,
                          vec3 view_direction,
                          vec3 diffuse_sample,
                          vec3 specular_sample,
                          float shininess)
{
    vec3 light_to_frag_direction = normalize(light.position - frag_position);
    float theta = dot(light_to_frag_direction, normalize(-light.direction));
    float epsilon = light.inner_cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    float distance = length(light.position - v_frag_position);
    float attentuation =
        1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return attentuation * phong_shading(light.base,
                                        light_to_frag_direction,
                                        normal,
                                        view_direction,
                                        diffuse_sample * intensity,
                                        specular_sample * intensity,
                                        shininess);
}

void main()
{
    vec3 view_direction = normalize(u_view_position - v_frag_position);

    // stretch the uvs: as the uvs expand, the texture effectively
    // shrinks, and vice versa.
    float inverted_texture_scale = 1.0 / max(u_texture_scale, MIN_TEXTURE_SCALE);
    vec2 scaled_uv = v_uv * inverted_texture_scale;
    vec3 diffuse_sample = texture(u_material.diffuse, scaled_uv).rgb;
    vec3 specular_sample = texture(u_material.specular, scaled_uv).rgb;

    vec3 directional_light = directional_light_component(u_directional_light,
                                                         v_normal,
                                                         view_direction,
                                                         diffuse_sample,
                                                         specular_sample,
                                                         u_material.shininess);

    vec3 point_light = point_light_component(u_point_light,
                                             v_frag_position,
                                             v_normal,
                                             view_direction,
                                             diffuse_sample,
                                             specular_sample,
                                             u_material.shininess);

    vec3 spot_light = spot_light_component(u_spot_light,
                                           v_frag_position,
                                           v_normal,
                                           view_direction,
                                           diffuse_sample,
                                           specular_sample,
                                           u_material.shininess);

    o_color = vec4(directional_light + point_light + spot_light, 1.0);
}
