#version 330

in vec2 in_Vertex;
in vec2 in_Coord0;

out vec2 texCoord;

uniform mat4 projection;

uniform ivec2 offset;
uniform ivec2 scale;

void main()
{
	texCoord = in_Coord0;
	vec2 position = vec2(in_Vertex.x*scale.x,in_Vertex.y*scale.y);
    gl_Position = projection  * vec4(position+offset, 0.0, 1.0);
}

