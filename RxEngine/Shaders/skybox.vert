#version 330

in vec3 in_Vertex;
in vec2 in_Coord0;


out vec2 texCoord;

uniform mat4 projection;
uniform mat4 modelview;


void main()
{

	mat4 transform = modelview;
	transform[3] = vec4(0.0f,0.0f,0.0f,1.0f);
	texCoord = in_Coord0;
    gl_Position = projection * transform * vec4(in_Vertex, 1.0);
}
