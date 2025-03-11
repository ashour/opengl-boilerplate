#version 410 core

out vec4 o_color;

in VS_OUT
{
    vec3 frag_position;
    vec2 uv;
    vec3 normal;
    vec4 frag_position_light_space;
}
fs_in;

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
uniform vec3 u_light_position;

uniform vec3 u_view_position;

uniform sampler2D u_tex_diffuse_1;
uniform sampler2D u_tex_specular_1;
uniform sampler2D u_tex_shadow_map;
struct Material
{
    sampler2D diffuse_1;
    sampler2D specular_1;
    float shininess;
};
uniform Material u_material;

const float MIN_TEXTURE_SCALE = 0.0001;
uniform float u_texture_scale = 1.0;

vec3 blinn_phong_shading(Light light,
                         vec3 normal,
                         vec3 view_direction,
                         vec3 light_direction,
                         vec3 diffuse_sample,
                         vec3 specular_sample,
                         float shininess)
{
    float diffuse_shading = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse_color * diffuse_shading * diffuse_sample;

    vec3 halfway_direction = normalize(light_direction + view_direction);
    float specular_shading = pow(max(dot(view_direction, halfway_direction), 0.0), shininess);
    vec3 specular = light.specular_color * specular_sample * specular_shading;

    return diffuse + specular;
}

vec3 directional_light_component(DirectionalLight light,
                                 vec3 normal,
                                 vec3 view_direction,
                                 vec3 light_direction,
                                 vec3 diffuse_sample,
                                 vec3 specular_sample,
                                 float shininess)
{
    return blinn_phong_shading(light.base,
                               normal,
                               view_direction,
                               light_direction,
                               diffuse_sample,
                               specular_sample,
                               shininess);
}

float shadow(vec4 frag_position_light_space, vec3 normal, vec3 light_direction)
{
    vec3 projected_coords =
        (frag_position_light_space.xyz / frag_position_light_space.w) * 0.5 + 0.5;

    if (projected_coords.z > 1.0)
    {
        return 0.0;
    }

    float closest_depth = texture(u_tex_shadow_map, projected_coords.xy).r;
    float current_depth = projected_coords.z;
    float bias = max(0.05 * (1.0 - dot(normal, light_direction)), 0.0025);

    return current_depth - bias > closest_depth ? 1.0 : 0.0;
}

void main()
{
    vec3 view_direction = normalize(u_view_position - fs_in.frag_position);

    // stretch the uvs: as the uvs expand, the texture effectively
    // shrinks, and vice versa.
    float inverted_texture_scale = 1.0 / max(u_texture_scale, MIN_TEXTURE_SCALE);
    vec2 scaled_uv = fs_in.uv * inverted_texture_scale;
    vec3 diffuse_sample = texture(u_material.diffuse_1, scaled_uv).rgb;
    vec3 specular_sample = texture(u_material.specular_1, scaled_uv).rgb;

    vec3 ambient = u_directional_light.base.ambient_color * diffuse_sample;

    vec3 light_direction = normalize(u_light_position - fs_in.frag_position);

    vec3 diffuse_and_specular = directional_light_component(u_directional_light,
                                                            fs_in.normal,
                                                            view_direction,
                                                            light_direction,
                                                            diffuse_sample,
                                                            specular_sample,
                                                            64);

    float shadow = shadow(fs_in.frag_position_light_space, fs_in.normal, light_direction);

    o_color = vec4((ambient + (1.0 - shadow) * diffuse_and_specular), 1.0);
    // o_color = vec4(vec3(shadow), 1.0);
}
