
attribute vec2 in_Vertex;
attribute vec2 in_Coord0;

uniform mat4 projection;
uniform ivec2 offset;

varying vec2 texCoord;

void main()
{
	texCoord = in_Coord0;
    gl_Position = projection  * vec4(in_Vertex+vec2(offset), 1.0, 1.0);
}

