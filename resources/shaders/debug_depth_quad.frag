#version 410

out vec4 o_color;

in vec2 v_uv;

uniform sampler2D u_depth_map;
uniform float u_near_plane;
uniform float u_far_plane;

// required when using a perspective projection matrix
float linearize_perspective_depth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * u_near_plane * u_far_plane) /
           (u_far_plane + u_near_plane - z * (u_far_plane - u_near_plane));
}

void main()
{
    float depth_value = texture(u_depth_map, v_uv).r;
    // o_color = vec4(vec3(linearize_perspective_depth(depth_value))); // perspectice
    o_color = vec4(vec3(depth_value), 1.0); // orthographic
}
