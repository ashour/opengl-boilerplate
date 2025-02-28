#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float u_time;

in VS_OUT { vec2 uv; }
gs_in[];

out vec2 v_uv;

vec3 normal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(u_time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}

void main()
{
    vec3 normal = normal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    v_uv = gs_in[0].uv;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    v_uv = gs_in[1].uv;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    v_uv = gs_in[2].uv;
    EmitVertex();
    EndPrimitive();
}
