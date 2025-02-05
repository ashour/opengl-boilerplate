#version 410 core

out vec4 o_color;

float near = 0.1;
float far = 100.0;

float linearize_depth(float non_linear_depth)
{
    float normalized_device_coordinate_z = non_linear_depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - normalized_device_coordinate_z * (far - near));
}

void main()
{
    float depth = linearize_depth(gl_FragCoord.z) / far;
    o_color = vec4(vec3(depth), 1.0);
}
