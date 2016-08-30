#version 330

in vec3 in_Vertex;
in vec3 in_Color;
in vec2 in_Coord0;

out vec3 color;
out vec2 texCoord;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 transform;

void main()
{
    color = in_Color;
	texCoord = in_Coord0;
    gl_Position = projection * modelview *transform* vec4(in_Vertex, 1.0);
}
