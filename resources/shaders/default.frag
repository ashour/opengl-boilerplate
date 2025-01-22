#version 410 core

in vec3 v_color;
in vec2 v_texture_coordinate;

out vec4 o_color;

const float MIN_TEXTURE_SCALE = 0.0001;
uniform sampler2D u_texture;
uniform float u_texture_scale = 1.0;

void main()
{
    float scale = 1.0 / max(u_texture_scale, MIN_TEXTURE_SCALE);
    o_color = texture(u_texture, v_texture_coordinate * scale) * vec4(v_color, 1.0);
}
