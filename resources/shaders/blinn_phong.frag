#version 410 core

out vec4 o_color;

in vec3 v_frag_position;
in vec3 v_normal;
in vec2 v_uv;

uniform sampler2D u_texture;
uniform vec3 u_light_position;
uniform vec3 u_view_position;
uniform bool u_use_blinn;

void main()
{
    vec3 color = texture(u_texture, v_uv).rgb;

    vec3 ambient = 0.05 * color;

    vec3 light_direction = normalize(u_light_position - v_frag_position);
    vec3 normal = normalize(v_normal);
    float difference = max(dot(light_direction, normal), 0.0);
    vec3 diffuse = difference * color;

    vec3 view_direction = normalize(u_view_position - v_frag_position);
    float specular_contribution = 0.0;
    if (u_use_blinn)
    {
        vec3 halfway_direction = normalize(light_direction + view_direction);
        specular_contribution = pow(max(dot(normal, halfway_direction), 0.0), 32.0);
    }
    else
    {
        vec3 reflection_direction = reflect(-light_direction, normal);
        specular_contribution = pow(max(dot(normal, reflection_direction), 0.0), 8.0);
    }
    vec3 specular = vec3(0.3) * specular_contribution;

    o_color = vec4(ambient + diffuse + specular, 1.0);
}
