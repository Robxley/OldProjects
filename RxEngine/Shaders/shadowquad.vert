#version 330

in vec3 in_Vertex;
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(in_Vertex, 1.0);
}
