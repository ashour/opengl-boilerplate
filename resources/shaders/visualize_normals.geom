#version 410

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT { vec3 normal; }
gs_in[];

const float MAGNITUDE = 0.4;

uniform mat4 u_projection;
uniform mat4 u_view;

void generate_line(int index)
{
    gl_Position = u_projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position =
        u_projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    generate_line(0);
    generate_line(1);
    generate_line(2);
}
