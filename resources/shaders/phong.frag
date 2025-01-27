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

void main()
{
    // stretch the uvs: as the uvs expand, the texture effectively
    // shrinks, and vice versa.
    float inverted_texture_scale = 1.0 / max(u_texture_scale, MIN_TEXTURE_SCALE);
    vec4 diffuse_texture_color = texture(u_material.diffuse, v_uv * inverted_texture_scale);

    vec3 ambient_frag_color = u_directional_light.base.ambient_color * diffuse_texture_color.rgb;

    vec3 light_direction = normalize(-u_directional_light.direction);

    vec3 diffuse_frag_color = u_directional_light.base.diffuse_color *
                              max(dot(v_normal, light_direction), 0.0) * diffuse_texture_color.rgb;

    vec3 view_direction = normalize(u_view_position - v_frag_position);
    vec3 reflection_direction = reflect(-light_direction, v_normal);
    float shine = pow(max(dot(view_direction, reflection_direction), 0.0), u_material.shininess);
    vec3 specular_frag_color =
        vec3(texture(u_material.specular, v_uv)) * shine * u_directional_light.base.specular_color;

    o_color = vec4(ambient_frag_color + diffuse_frag_color + specular_frag_color, 1.0);
}
