#version 330

in vec2 in_Vertex;
in vec2 in_Coord0;

out vec2 texCoord;

uniform mat4 projection;
uniform ivec2 offset;


void main()
{
	texCoord = in_Coord0;
    gl_Position = projection  * vec4(in_Vertex+offset, 1.0, 1.0);
}

