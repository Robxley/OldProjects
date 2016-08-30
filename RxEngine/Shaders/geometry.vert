#version 330

in vec3 in_Vertex;
in vec4 in_Color;

out vec4 color;

uniform mat4 projection;
uniform mat4 modelview;

void main()
{
    color = in_Color;
    gl_Position = projection * modelview * vec4(in_Vertex, 1.0);
}
