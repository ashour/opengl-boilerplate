#version 410 core

out vec4 o_color;

in vec3 v_frag_position;
in vec3 v_normal;
in vec3 v_color;
in vec2 v_texture_coordinate;

uniform float u_ambient_strength = 0.2;
uniform vec3 u_ambient_color = vec3(1.0);

uniform vec3 u_directional_light_position = vec3(-100.0, 100.0, 0.0);
uniform vec3 u_directional_light_color = vec3(0.8);

const float MIN_TEXTURE_SCALE = 0.0001;
uniform sampler2D u_texture;
uniform float u_texture_scale = 1.0;

void main()
{
    // stretch the uvs: as the uvs expand, the texture effectively
    // shrinks, and vice versa.
    float inverted_texture_scale = 1.0 / max(u_texture_scale, MIN_TEXTURE_SCALE);
    vec4 texture_color = texture(u_texture, v_texture_coordinate * inverted_texture_scale);

    vec3 ambient_light_color = u_ambient_strength * u_ambient_color;

    vec3 light_direction = normalize(u_directional_light_position - v_frag_position);
    vec3 diffuse_light_color = max(dot(v_normal, light_direction), 0.0) * u_directional_light_color;

    o_color =
        texture_color * vec4(v_color, 1.0) * vec4(ambient_light_color + diffuse_light_color, 1.0);
}
