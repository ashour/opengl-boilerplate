#version 410 core

out vec4 o_color;

in vec2 v_uv;

uniform sampler2D u_screen_texture;

void main()
{
    vec3 screen_sample = texture(u_screen_texture, v_uv).rgb;

    float average = 0.2126 * screen_sample.r + 0.7152 * screen_sample.g + 0.0722 * screen_sample.b;

    o_color = vec4(average, average, average, 1.0);
}
