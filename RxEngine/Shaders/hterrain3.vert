#version 330

in vec3 in_Vertex;
in vec4 in_Color;
in vec2 in_Coord0;
in vec3 in_Normal;
in vec3 in_Data1;   //mélange texture
in vec3 in_Data0;  //info index texture


out vec4 color;
out vec2 texCoord;
out vec3 normal;
out vec3 data1;
flat out vec3 data0;


uniform mat4 projection;
uniform mat4 modelview;


void main()
{
	mat4 normalMatrix = modelview;
	normalMatrix[3] = vec4(0.0,0.0,0.0,1.0);

	color = in_Color;

	data0 = in_Data0;
	data1 = in_Data1;

	// Determine the normal of the vertex
	normal = normalize((normalMatrix * vec4(in_Normal,0.0f)).xyz);

	// Textures coordinates
	texCoord = in_Coord0;

	// The position of the vertex
	gl_Position = projection * modelview * vec4(in_Vertex,1.0);

}

